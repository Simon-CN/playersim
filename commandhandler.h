#include<string>
#include <map>
#include "itemfunctionhelper.h"
class Player;

class CommandHandler
{
public:
	CommandHandler();
	~CommandHandler();

	int handlerCommandLine(std::string line, Player& player);

private:
	void initCmdMap();
	void dump(Player& player);
	void buy(int uid, int num, Player& player);
	void sell(int index, int num, Player& player);
	void use(int index, Player& player);
	void exchange(int index1, int index2, Player& player);
	void move(int index1, int index2, int num, Player& player);
	void capacity(int cap, Player& player);
	void money(int num, Player& player);

	bool isIndexValid(int index, Player& player);
	void printResult(const char* errStr);
	void printResult();
	bool isIntegerPlusOverflow(int a, int b);
	bool isIntegerMulOverflow(int a, int b);
	void executeCommand(std::string cmd, int argc, int* argv, Player& player);

	std::map<std::string, int> mCmdMap;
	ItemFunctionHelper mItmFunHelper;
};

