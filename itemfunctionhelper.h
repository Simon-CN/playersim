#include <string>

class Player;

class ItemFunctionHelper
{
	typedef int (ItemFunctionHelper::*pf)(int*, Player&);

public:
	ItemFunctionHelper();
	~ItemFunctionHelper();

	int execItemFunction(int uid, int count, Player& player);

private:
	void initDriverTable();
	void parseItemFunc(std::string fun, int* args, Player& player);
	bool isValidItemFunc(int* args);

	//0010
	int fun1(int* args, Player& player);

	//1010
	int fun2(int* args, Player& player);

	//1100
	int fun3(int* args, Player& player);

	//0011
	int fun4(int* args, Player& player);

	//1011
	int fun5(int* args, Player& player);

	//1101
	int fun6(int* args, Player& player);

	//0012
	int fun7(int* args, Player& player);

	//table drive
	//num: unfixed 0, fixed 1
	//operation: + 0, - 1
	//effect: percent 0, fixed 1
	//target: HP 0, MP 1, Money2
	pf mTable[2][2][2][3];
};

