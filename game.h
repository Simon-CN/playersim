#include <map>
#include "player.h"
#include "commandhandler.h"

class Game
{
public:
	Game();
	~Game();

	void run();

private:
	Player mPlayer;
	CommandHandler mCmdHandler;
};

