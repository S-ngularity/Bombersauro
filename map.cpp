#include "Map.h"

#include "Pos.h"
#include "MapTile.h"

Map::Map(int w, int h)
{
	mapWidth = w;
	mapHeight = h;

	map = new MapTile*[mapWidth];

	for(int i = 0; i < mapWidth; i++)
		map[i] = new MapTile[mapHeight];

	for(int y = 0; y < mapHeight; y++)
		for(int x = 0; x < mapWidth; x++)
			Tile(x, y).setH(INIT_H);

	highestH = INIT_H;
	lowestH = INIT_H;
	seaLevel = SEA_LEVEL;
}

Map::~Map()
{
	for(int i = 0; i < mapWidth; i++)
		delete [] map[i];

	delete [] map;
}

MapTile& Map::Tile(Pos p)
{
	return Tile(p.getX(), p.getY());
}

MapTile& Map::Tile(int x, int y)
{
	if(isPosInsideWrap(x, y))
	{
		if(x < 0)
			x = mapWidth + x % mapWidth;

		else if(x >= mapWidth)
			x = x % mapWidth;
	}

/*if(isPosInsideWrap(x, y)) // WRAP IN Y (mudar isPosInsideWrap para só true)
{
	if(y < 0)
		y = mapHeight + y % mapHeight;

	else if(y >= mapHeight)
		y = y % mapHeight;
}//*/

	if(isPosInsideNoWrap(x, y))
		return map[x][y];

	else
		return map[0][0];
}

void Map::normalize(int maxH)
{
	for(int y = 0; y < mapHeight; y++)
		for(int x = 0; x < mapWidth; x++)
		{
			Tile(x, y).setH(
				((Tile(x, y).getH() - lowestH) / (float)(highestH - lowestH)) * maxH);
		}

	setHighestH(maxH); // there will be always a highest point normalized from the original highest point
	setLowestH(0);
}

bool Map::isPosInsideWrap(Pos p)
{
	if(p.getY() >= 0 && p.getY() < mapHeight)
		return true;

	else
		return false;
}

bool Map::isPosInsideWrap(int x, int y)
{
	if(y >= 0 && y < mapHeight)
		return true;

	else
		return false;
}

bool Map::isPosInsideNoWrap(Pos p)
{
	if(p.getX() >= 0 && p.getY() >= 0 && p.getX() < mapWidth && p.getY() < mapHeight)
		return true;

	else
		return false;
}	

bool Map::isPosInsideNoWrap(int x, int y)
{
	if(x >= 0 && y >= 0 && x < mapWidth && y < mapHeight)
		return true;

	else
		return false;
}

int Map::getMapHeight()
{
	return mapHeight;
}

int Map::getMapWidth()
{
	return mapWidth;
}

int Map::getHighestH()
{
	return highestH;
}

void Map::setHighestH(int h)
{
	highestH = h;
}

int Map::getLowestH()
{
	return lowestH;
}

void Map::setLowestH(int h)
{
	lowestH = h;
}

int Map::getSeaLevel()
{
	return seaLevel;
}

void Map::setSeaLevel(int lvl)
{
	seaLevel = lvl;
}

void Map::increaseSeaLevel()
{
	seaLevel++;
}

void Map::decreaseSeaLevel()
{
	seaLevel--;
}
