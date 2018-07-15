#include<string>
class Item
{
public:
	Item(int uid,std::string desc, int price, int rec, std::string fun) :
		mUid(uid), mPrice(price), mRecPrice(rec), mDesc(desc), mFun(fun)
	{
	}
	~Item();

	int uid() { return mUid; }
	int price() { return mPrice; }
	int recyclePrice(){ return mRecPrice; }
	std::string description() { return mDesc; }
	std::string function() { return mFun; }
	void dump();

private:
	int mUid, mPrice, mRecPrice;
	std::string mDesc, mFun;
};

