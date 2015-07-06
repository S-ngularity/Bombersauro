#include "Game.h"

#include "Map/Noises/DiamSqNoise.h"

Game Game::gameInstance;

Game::Game() : worldMap(mapW, mapH) {} // private so that it can't be called

Game::~Game()
{
	delete player;
	
	for(GlObject* o : objectList)
		delete o;
}

void Game::initGame()
{
	DiamSqNoise dsNoise(&worldMap);
	
	while(dsNoise.getPercentComplete() < 100)
	{
		dsNoise.runOnce();
	}
	worldMap.normalize(maxMapH); // normaliza altura máxima do mapa, originalmente é 255
	
	normalShader = new NormalShader();

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

NormalShader* Game::getNormalShader()
{
	return normalShader;
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
