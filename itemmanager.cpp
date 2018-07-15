#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "itemmanager.h"
#include "item.h"

ItemManager* ItemManager::instance = NULL;

ItemManager::ItemManager()
{
}

ItemManager::~ItemManager()
{
	std::map<int, Item*>::iterator iter = mItemMap.begin();
	while (iter != mItemMap.end())
	{
		if (iter->second != NULL)
		{
			delete iter->second;
			iter->second = NULL;
		}
		++iter;
	}
}

void ItemManager::loadItems(const char * path)
{
	std::ifstream in(path);
	int uid, price, recPrice;
	std::string line, tmp;
	while (std::getline(in, line))
	{
		std::istringstream iss(line);
		std::string desc, fun;
		iss >> uid;
		iss >> desc;
		iss >> price;
		iss >> recPrice;
		while (iss >> tmp)
			fun += tmp + " ";
		fun.erase(fun.end()-1);
		Item* newItem = new Item(uid, desc, price, recPrice, fun);
		mItemMap[uid] = newItem;
	}
	in.close();
}

Item * ItemManager::getItem(int uid)
{
	std::map<int, Item*>::iterator iter = mItemMap.find(uid);
	return iter == mItemMap.end() ? NULL : iter->second;
}

void ItemManager::dump()
{
	std::map<int, Item*>::iterator iter = mItemMap.begin();
	while (iter != mItemMap.end())
	{
		iter->second->dump();
		++iter;
	}

}
