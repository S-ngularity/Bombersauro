#include "MapTile.h"

#include <stdlib.h>

#define NULO -1

MapTile::MapTile()
{
	h = 0;
	chance = 0;
	isSeed = false;
	error = false;
}

// h
int MapTile::getH()
{
	return h;
}

void MapTile::setH(int newH)
{
	h = newH;
}


// chance
int MapTile::getChance()
{
	return chance;
}

void MapTile::setBaseChance()
{
	chance = 50 + rand() % 51;
}

void MapTile::lowerChance(MapTile prevTile)
{
	chance = (float)prevTile.getChance() * (float)((float)(rand() % 101) / (float)100) * MULTIPLIER;
	//chance = 0;
}

// isSeed
bool MapTile::getIsSeed()
{
	return isSeed;
}

void MapTile::setIsSeed(bool newIsSeed)
{
	isSeed = newIsSeed;
}

// error
bool MapTile::getError()
{
	return error;
}

void MapTile::setError(bool newError)
{
	error = newError;
}