#include <list>

class PackGrid
{
public:
	PackGrid(int u, int n) :uid(u), count(n) {}
	int uid, count;
};

class BackPack
{

public:
	static const int MAX_CAPACITY = 32;
	static const int MAX_ITEM_COUNT = 5000;

	BackPack(int capacity);
	~BackPack();

	void setCapacity(int cap) { mCapacity = cap; }
	int capacity() { return mCapacity; }

	void add(int uid, int num);
	void remove(int index, int num);
	void exchange(int index1, int index2);
	void expand(int cap);
	void moveToEmptyGrid(int index1, int index2, int num);
	void moveToExistGrid(int index1, int index2, int num);
	PackGrid* getGridItem(int index);
	int getItemCapacity(int uid);
	void dump();

private:
	int mCapacity;
	PackGrid* mGrids[MAX_CAPACITY];
	std::list<int> mFreeGrids;

	int addExistingItem(int uid, int num);
	void addNewItem(int uid, int num);
};
