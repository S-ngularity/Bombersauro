#include "Game.h"

Game Game::gameInstance;

void Game::initGame()
{
	player = new Player();
}

void Game::addObject(GlObject *o)
{
	objectList.push_back(o);
}

void Game::render()
{
	player->render(player->getCamera().getProj(), player->getCamera().getView());

	for(GlObject *o : objectList)
	{
		if(o != nullptr)
			o->render(player->getCamera().getProj(), player->getCamera().getView());
	}
}

Map& Game::getMap()
{
	return worldMap;
}

Player& Game::getPlayer()
{
	return *player;
}
