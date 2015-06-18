#include "Pos.h"

Pos::Pos()
{
	x = 0;
	y = 0;
}

Pos::Pos(int initX, int initY)
{
	x = initX;
	y = initY;
}

int Pos::getX()
{
	return x;
}

int Pos::getY()
{
	return y;
}

void Pos::setPos(int newX, int newY)
{
	x = newX;
	y = newY;
}