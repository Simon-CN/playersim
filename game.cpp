#include<iostream>
#include <sstream>
#include<string>
#include "game.h"
#include "item.h"
#include "itemmanager.h"

Game::Game() :mPlayer(Player::MAX_HP, Player::MAX_MP, 0, 8)
{
	ItemManager::getInstance()->loadItems("item.txt");
}


Game::~Game()
{
}

void Game::run()
{
	std::string line;
	std::cout << ">";
	while (std::getline(std::cin, line))
		if (mCmdHandler.handlerCommandLine(line, mPlayer) == 1)
			break;
}
