#ifndef GAME
#define GAME

#include "Map/Map.h"
#include "Player.h"
#include "GlClasses/GlObject.h"
#include "Geometry/NormalShader.h"

#include <list>

const int mapW = 129, mapH = 129;
const int maxMapH = 20;

class Game
{
	private:
		static Game gameInstance;
		Game() : worldMap(mapW, mapH) {} // private so that it can't be called
		~Game()
		{
			delete player;
			
			for(GlObject* o : objectList)
				delete o;
		}
		Game(Game const&) = delete;
		Game& operator=(Game const&) = delete;

		Map worldMap;
		
		Player *player;

		NormalShader *normalShader;

		std::list<GlObject*> objectList;

	public:
		static Game& Instance() {return gameInstance;}
		void initGame();

		void addObject(GlObject *o);
		void removeObject(GlObject *o);

		void render();

		Map& getMap();
		Player& getPlayer();

		NormalShader* getNormalShader();

		void resetMap();
};


#endif