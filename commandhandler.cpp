#include <sstream>
#include <limits.h>
#include "commandhandler.h"
#include "player.h"
#include "item.h"
#include "itemmanager.h"


CommandHandler::CommandHandler()
{
	initCmdMap();
}

CommandHandler::~CommandHandler() {}

int CommandHandler::handlerCommandLine(std::string line, Player& player)
{
	int argv[4], count = 0;
	std::istringstream iss(line);
	std::string cmd;
	iss >> cmd;

	if (mCmdMap.find(cmd) == mCmdMap.end())
	{
		printResult("Command not found");
		return 0;
	}
	if (cmd == "exit")
	{
		printResult();
		return 1;
	}
	while (iss >> argv[count++]);
	if (count != mCmdMap[cmd.c_str()] + 1)
	{
		printResult("Incorrect number of arguments");
		return 0;
	}
	executeCommand(cmd, count - 1, argv, player);
	return 0;
}

void CommandHandler::initCmdMap()
{
	mCmdMap["dump"] = 0;
	mCmdMap["buy"] = 2;
	mCmdMap["sell"] = 2;
	mCmdMap["exchange"] = 2;
	mCmdMap["move"] = 3;
	mCmdMap["money"] = 1;
	mCmdMap["capacity"] = 1;
	mCmdMap["use"] = 1;
	mCmdMap["exit"] = 0;
}

void CommandHandler::executeCommand(std::string cmd, int argc, int * argv, Player& player)
{
	if (cmd == "buy")
	{
		buy(argv[0], argv[1], player);
		return;
	}
	if (cmd == "sell")
	{
		sell(argv[0], argv[1], player);
		return;
	}
	if (cmd == "exchange")
	{
		exchange(argv[0], argv[1], player);
		return;
	}
	if (cmd == "move")
	{
		move(argv[0], argv[1], argv[2], player);
		return;
	}
	if (cmd == "capacity")
	{
		capacity(argv[0], player);
		return;
	}
	if (cmd == "use")
	{
		use(argv[0], player);
		return;
	}
	if (cmd == "money")
	{
		money(argv[0], player);
		return;
	}
	if (cmd == "dump")
	{
		dump(player);
		return;
	}
}

void CommandHandler::dump(Player& player)
{
	player.dump();
	printResult();
}

void CommandHandler::buy(int uid, int num, Player& player)
{
	Item* item = ItemManager::getInstance()->getItem(uid);
	if (item == NULL)
	{
		printResult("Invalid item id");
		return;
	}
	if (num <= 0 || isIntegerMulOverflow(item->price(), num))
	{
		printResult("Invalid item num");
		return;
	}
	int cost = item->price()*num;
	if (cost > player.money())
	{
		printResult("No enough money");
		return;
	}
	int cap = player.backpack()->getItemCapacity(uid);
	if (num > cap)
	{
		printResult("Insufficient backpack space");
		return;
	}
	player.backpack()->add(uid, num);
	player.setMoney(player.money() - cost);
	printResult();
}

void CommandHandler::sell(int index, int num, Player& player)
{
	if (!isIndexValid(index, player))
		return;
	PackGrid* grid = player.backpack()->getGridItem(index);
	if (grid == NULL)
	{
		printResult("No item in the grid");
		return;
	}
	if (num <= 0 || grid->count < num)
	{
		printResult("Invalid item count");
		return;
	}
	int rec = ItemManager::getInstance()->getItem(player.backpack()->getGridItem(index)->uid)->recyclePrice();
	int total = rec * num;
	if (isIntegerMulOverflow(num, rec) || isIntegerPlusOverflow(total, player.money()) || player.money() + total > Player::MAX_MONEY)
	{
		printResult("Money overflow");
		return;
	}
	player.setMoney(player.money() + total);
	player.backpack()->remove(index, num);
	printResult();
}

void CommandHandler::use(int index, Player& player)
{
	if (!isIndexValid(index, player))
		return;
	PackGrid* grid = player.backpack()->getGridItem(index);
	if (grid == NULL)
	{
		printResult("No item in the grid");
		return;
	}
	int usedItem = mItmFunHelper.execItemFunction(grid->uid, grid->count, player);
	if (usedItem > 0)
	{
		player.backpack()->remove(index, usedItem);
		printResult();
	}
	else if (usedItem == 0)
		printResult("No need to use the item");
}

void CommandHandler::exchange(int index1, int index2, Player& player)
{
	if (!isIndexValid(index1, player) || !isIndexValid(index2, player))
		return;
	PackGrid* grid1, *grid2;
	grid1 = player.backpack()->getGridItem(index1);
	grid2 = player.backpack()->getGridItem(index2);
	if (grid1 == NULL || grid2 == NULL)
	{
		printResult("No item in the grid");
		return;
	}
	player.backpack()->exchange(index1, index2);
	printResult();
}

void CommandHandler::move(int index1, int index2, int num, Player& player)
{
	if (!isIndexValid(index1, player) || !isIndexValid(index2, player))
		return;
	PackGrid* grid1, *grid2;
	grid1 = player.backpack()->getGridItem(index1);
	grid2 = player.backpack()->getGridItem(index2);
	if (grid1 == NULL)
	{
		printResult("No item in index1");
		return;
	}
	if (num <= 0 || num > grid1->count)
	{
		printResult("Invalid item count");
		return;
	}
	if (grid2 == NULL)
		player.backpack()->moveToEmptyGrid(index1, index2, num);
	else if (grid1->uid != grid2->uid)
	{
		printResult("Inconsistent item id");
		return;
	}
	else if (num + grid2->count > BackPack::MAX_ITEM_COUNT)
	{
		printResult("Invalid item count");
		return;
	}
	else
		player.backpack()->moveToExistGrid(index1, index2, num);
	printResult();
}

void CommandHandler::capacity(int cap, Player& player)
{
	if (cap <= player.backpack()->capacity() || cap > BackPack::MAX_CAPACITY)
	{
		printResult("Invalid new capacity");
		return;
	}
	player.backpack()->expand(cap);
	printResult();
}

void CommandHandler::money(int num, Player& player)
{
	int sum = num + player.money();
	if (num <= 0 || sum > Player::MAX_MONEY || isIntegerPlusOverflow(num, player.money()))
	{
		printResult("Invalid amount of money");
		return;
	}
	player.setMoney(sum);
	printResult();
}

bool CommandHandler::isIndexValid(int index, Player& player)
{
	bool res = index >= 0 && index < player.backpack()->capacity();
	if (!res)
		printResult("Index out of bounds");
	return res;
}

void CommandHandler::printResult(const char * errStr)
{
	std::cout << "-ERR\t" << errStr << std::endl;
	std::cout << ">";
}

void CommandHandler::printResult()
{
	std::cout << "+OK" << std::endl;
	std::cout << ">";
}

bool CommandHandler::isIntegerPlusOverflow(int num1, int num2)
{
	int sum = num1 + num2;
	return 	((sum ^ num1) < 0 && (sum^num2) < 0);
}

bool CommandHandler::isIntegerMulOverflow(int a, int b)
{
	if (a >= 0 && b >= 0)
	{
		return INT_MAX / a < b;
	}
	else if (a < 0 && b < 0)
	{
		return INT_MAX / a > b;
	}
	else if (a * b == INT_MIN)
	{
		return 0;
	}
	else
	{
		return a < 0 ? isIntegerMulOverflow(-a, b) : isIntegerMulOverflow(a, -b);
	}
}
