#include "item.h"
#include <iostream>

Item::~Item()
{
}

void Item::dump()
{
	std::cout << mUid << ", " << mDesc << ", " << mPrice << ", " << mRecPrice << ", " << mFun << std::endl;
}
