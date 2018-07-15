#include <sstream>
#include <algorithm>
#include "itemfunctionhelper.h"
#include "player.h"
#include "item.h"
#include "itemmanager.h"
#include "numutil.h"
#include "printutil.h"

ItemFunctionHelper::ItemFunctionHelper()
{
	initDriverTable();
}


ItemFunctionHelper::~ItemFunctionHelper()
{
}

int ItemFunctionHelper::execItemFunction(int uid, int count, Player & player)
{
	Item* item = ItemManager::getInstance()->getItem(uid);
	if (item == NULL)
	{
		PrintUtil::printResult("Invalid item id");
		return -1;//item not found
	}

	if (item->function() == "*")
	{
		PrintUtil::printResult("Unavailable item");
		return -2;//item unavailable
	}

	int funv[7];//type,operation,target,effect,function,count
	std::fill(funv, funv + 7, 0);
	funv[6] = count;
	parseItemFunc(item->function(), funv, player);
	if (!isValidItemFunc(funv))
	{
		PrintUtil::printResult("Invalid item function");
		return -3;
	}
	if (this->mTable[funv[0] > 0 ? 1 : 0][funv[1]][funv[2]][funv[3]] != NULL)
		return (this->*mTable[funv[0]][funv[1]][funv[2]][funv[3]])(funv, player);//used item count
	else
	{
		PrintUtil::printResult("Invalid item function");
		return	-3;//invalid fun
	}
}

void ItemFunctionHelper::initDriverTable()
{
	for (int i = 0; i < 2; ++i)
		for (int j = 0; j < 2; ++j)
			for (int k = 0; k < 2; ++k)
				for (int l = 0; l < 3; ++l)
					mTable[i][j][k][l] = NULL;
	mTable[0][0][1][0] = &ItemFunctionHelper::fun1;
	mTable[1][0][1][0] = &ItemFunctionHelper::fun2;
	mTable[1][1][0][0] = &ItemFunctionHelper::fun3;
	mTable[0][0][1][1] = &ItemFunctionHelper::fun4;
	mTable[1][0][1][1] = &ItemFunctionHelper::fun5;
	mTable[1][1][0][1] = &ItemFunctionHelper::fun6;
	mTable[1][0][0][2] = &ItemFunctionHelper::fun7;
}

void ItemFunctionHelper::parseItemFunc(std::string fun, int * args, Player & player)
{
	std::istringstream iss(fun);
	for (int i = 0; i < 5; ++i)
		iss >> args[i];
	if (args[2] == 0)
		iss >> args[5];
}

bool ItemFunctionHelper::isValidItemFunc(int * args)
{
	if (args[0] < 0 || (args[1] != 0 && args[1] != 1) || (args[2] != 0 && args[2] != 1) || args[3] < 0 || args[3]>2 || args[4] < 0)
		return false;
	if (args[0] == 0 && args[5] < 0)
		return false;
	return true;
}

//0010 hp3
int ItemFunctionHelper::fun1(int * args, Player & player)
{
	int maxEff = args[6] * args[4];
	int diff = Player::MAX_HP - player.hp();
	if (diff >= maxEff)
	{
		player.setHp(player.hp() + maxEff);
		return args[6];
	}
	else
	{
		int count = (diff - 1) / args[4] + 1;
		player.setHp(Player::MAX_HP);
		return count;
	}
}

//1010 hp1 hp2
int ItemFunctionHelper::fun2(int * args, Player & player)
{
	if (player.hp() == Player::MAX_HP)
		return 0;
	int total = player.hp() + args[4] * args[0];
	if (total >= Player::MAX_HP)
		player.setHp(Player::MAX_HP);
	else
		player.setHp(total);
	return args[0];
}

//1100 posion1
int ItemFunctionHelper::fun3(int * args, Player & player)
{
	if (player.hp() == 0)
		return 0;
	if (player.hp() <= args[4])
	{
		player.setHp(0);
		return args[0];
	}
	double percent = (double)args[5] / 100;
	int resHp = player.hp();
	for (int i = 0; i < args[0]; ++i)
		resHp *= percent;
	int diff = player.hp() - resHp;
	if (diff < args[4])
		player.setHp(player.hp() - args[4] * args[0]);
	else
		player.setHp(resHp);
	return args[0];
}

//0011 mp3
int ItemFunctionHelper::fun4(int * args, Player & player)
{
	int maxEff = args[6] * args[4];
	int diff = Player::MAX_MP - player.mp();
	if (diff >= maxEff)
	{
		player.setMp(player.mp() + maxEff);
		return args[6];
	}
	else
	{
		int count = (diff - 1) / args[4] + 1;
		player.setMp(Player::MAX_MP);
		return count;
	}
}

//1011 mp1 mp2
int ItemFunctionHelper::fun5(int * args, Player & player)
{
	if (player.mp() == Player::MAX_MP)
		return 0;
	int total = player.mp() + args[4] * args[0];
	if (total >= Player::MAX_MP)
		player.setMp(Player::MAX_MP);
	else
		player.setMp(total);
	return args[0];
}

//1101 posion2
int ItemFunctionHelper::fun6(int * args, Player & player)
{
	if (player.mp() <= args[4])
	{
		player.setMp(0);
		return args[0];
	}
	double percent = (double)args[5] / 100;
	int resMp = player.mp();
	for (int i = 0; i < args[0]; ++i)
		resMp *= percent;
	int diff = player.mp() - resMp;
	if (diff < args[4])
		player.setMp(player.mp() - args[4] * args[0]);
	else
		player.setMp(resMp);
	return args[0];
}

//0012 gold
int ItemFunctionHelper::fun7(int * args, Player & player)
{
	if (player.money() == Player::MAX_MONEY || NumUtil::isIntegerMulOverflow(args[0], args[4])
		|| NumUtil::isIntegerPlusOverflow(player.money(), args[0] * args[4])
		|| player.money() + args[0] * args[4] > Player::MAX_MONEY)
		return 0;
	player.setMoney(player.money() + args[0] * args[4]);
	return args[0];
}
