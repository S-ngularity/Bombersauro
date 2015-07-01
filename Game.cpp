#include "Game.h"

#include "Noises/DiamSqNoise.h"

Game Game::gameInstance;

void Game::initGame()
{
	DiamSqNoise dsNoise(&worldMap);
	
	while(dsNoise.getPercentComplete() < 100)
	{
		dsNoise.runOnce();
	}
	worldMap.normalize(maxMapH); // normaliza altura máxima do mapa, originalmente é 255
	
	player = new Player();
}

void Game::addObject(GlObject *o)
{
	objectList.push_back(o);
}

void Game::removeObject(GlObject *o)
{
	std::list<GlObject*>::iterator i;

	for (i = objectList.begin(); i != objectList.end(); ++i)
	{
		if(*i == o)
		{
			objectList.erase(i);

			break;
		}
	}
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

void Game::resetMap()
{
	DiamSqNoise dsNoise(&worldMap);

	while(dsNoise.getPercentComplete() < 100)
	{
		dsNoise.runOnce();
	}
	worldMap.normalize(maxMapH); // normaliza altura máxima do mapa, originalmente é 255

	player->resetPos();
}
