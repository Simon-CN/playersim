#include <map>
class Item;

class ItemManager
{
public:
	static ItemManager* getInstance()
	{
		if (instance == NULL)
			instance = new ItemManager();
		return instance;
	}
	~ItemManager();

	void loadItems(const char* path);
	Item* getItem(int uid);
	void dump();

private:
	ItemManager();

	static ItemManager * instance;
	std::map<int, Item*> mItemMap;
};

