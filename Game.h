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
		Game(); // private so that it can't be called
		~Game();
		Game(Game const&) = delete;
		Game& operator=(Game const&) = delete;

		NormalShader *normalShader;
		std::list<GlObject*> objectList;

		Map worldMap;
		Player *player;

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