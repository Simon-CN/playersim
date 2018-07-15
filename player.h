#include <iostream>
#include "backpack.h"
class Player
{
public:
	static const int MAX_HP = 10000;
	static const int MAX_MP = 10000;
	static const int MAX_MONEY = 2000000000;

	Player(int hp, int mp, int money, int capacity) :mHp(hp), mMp(mp), mMoney(money), mBackPack(capacity) {};
	~Player() {};

	int hp() { return mHp; }
	int mp() { return mMp; }
	int money() { return mMoney; }
	void setHp(int hp) { mHp = hp; }
	void setMp(int mp) { mMp = mp; }
	void setMoney(int money) { mMoney = money; }
	void dump()
	{
		std::cout << "HP\t" << mHp << std::endl;
		std::cout << "MP\t" << mMp << std::endl;
		std::cout << "Money\t" << mMoney << std::endl;
		mBackPack.dump();
	}

	BackPack* backpack() { return &mBackPack; }

private:
	int mHp, mMp, mMoney;
	BackPack mBackPack;
};

