#ifndef MAP
#define MAP

#include "Pos.h"
#include "MapTile.h"

class Pos;
class MapTile;

#define MAX_H 255 //170//120//70

#define INIT_H ((MAX_H / 2) - 1)

#define SEA_LEVEL ((MAX_H / 2) - 1)

class Map
{
	private:
		int mapHeight, mapWidth;
		MapTile **map;

		int highestH, lowestH;
		int seaLevel;

	public:
		Map(int w, int h);
		~Map();

		MapTile& Tile(Pos p);
		MapTile& Tile(int x, int y);

		void normalize(int maxH);

		bool isPosInsideWrap(Pos p);
		bool isPosInsideWrap(int x, int y);

		bool isPosInsideNoWrap(Pos p);
		bool isPosInsideNoWrap(int x, int y);

		int getMapHeight();
		int getMapWidth();

		int getHighestH();
		void setHighestH(int h);

		int getLowestH();
		void setLowestH(int h);

		int getSeaLevel();
		void setSeaLevel(int lvl);
		void increaseSeaLevel();
		void decreaseSeaLevel();
};

#endif
