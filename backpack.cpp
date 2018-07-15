#include <iostream>
#include <algorithm>
#include "backpack.h"

BackPack::BackPack(int capacity) :mCapacity(capacity)
{
	for (int i = 0; i < MAX_CAPACITY; ++i)
		mGrids[i] = NULL;
	for (int i = 0; i < capacity; ++i)
		mFreeGrids.push_front(i);
};


BackPack::~BackPack()
{
	for (int i = 0; i < mCapacity; ++i)
	{
		if (mGrids[i] != NULL)
		{
			delete mGrids[i];
			mGrids[i] = NULL;
		}
	}
}

void BackPack::add(int uid, int num)
{
	int rest = addExistingItem(uid, num);
	return addNewItem(uid, rest);
}

void BackPack::remove(int index, int num)
{
	mGrids[index]->count -= num;
	if (mGrids[index]->count == 0)
	{
		mFreeGrids.push_back(index);
		delete mGrids[index];
		mGrids[index] = NULL;
	}
}

void BackPack::exchange(int index1, int index2)
{
	PackGrid* pg = mGrids[index1];
	mGrids[index1] = mGrids[index2];
	mGrids[index2] = pg;
}

void BackPack::expand(int cap)
{
	for (int i = mCapacity; i < cap; ++i)
		mFreeGrids.push_front(i);
	mCapacity = cap;
}

void BackPack::moveToEmptyGrid(int index1, int index2, int num)
{
	mGrids[index2] = new PackGrid(mGrids[index1]->uid, num);
	std::list<int>::iterator iter = std::find(mFreeGrids.begin(), mFreeGrids.end(), index2);
	if (iter != mFreeGrids.end())
		mFreeGrids.erase(iter);
	remove(index1, num);
}

void BackPack::moveToExistGrid(int index1, int index2, int num)
{
	mGrids[index2]->count += num;
	remove(index1, num);
}

PackGrid * BackPack::getGridItem(int index)
{
	return mGrids[index];
}

int BackPack::getItemCapacity(int uid)
{
	int res = 0;
	for (int i = 0; i < mCapacity; ++i)
		if (mGrids[i] != NULL && mGrids[i]->uid == uid)
			res += MAX_ITEM_COUNT - mGrids[i]->count;
	res += mFreeGrids.size()*MAX_ITEM_COUNT;
	return res;
}

void BackPack::dump()
{
	std::cout << "Capacity\t" << mCapacity << std::endl;
	for (int i = 0; i < mCapacity; ++i)
		if (mGrids[i] != NULL)
			std::cout << i << "\t" << mGrids[i]->uid << "\t" << mGrids[i]->count << std::endl;
}

int BackPack::addExistingItem(int uid, int num)
{
	int total;
	for (int i = 0; i < mCapacity; ++i)
	{
		if (mGrids[i] != NULL && mGrids[i]->uid == uid && mGrids[i]->count < MAX_ITEM_COUNT)
		{
			total = mGrids[i]->count + num;
			if (total < MAX_ITEM_COUNT)
			{
				mGrids[i]->count = total;
				num = 0;
			}
			else
			{
				mGrids[i]->count = MAX_ITEM_COUNT;
				num = total - MAX_ITEM_COUNT;
			}
		}
	}
	return num;
}

void BackPack::addNewItem(int uid, int num)
{
	int index, count = 0;
	while (num > 0)
	{
		count = num > MAX_ITEM_COUNT ? MAX_ITEM_COUNT : num;
		index = mFreeGrids.back();
		mFreeGrids.pop_back();
		mGrids[index] = new PackGrid(uid, count);
		num -= count;
	}
}
