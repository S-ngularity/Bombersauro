#ifndef POS
#define POS

class Pos
{
	private:
		int x;
		int y;

	public:
		Pos();
		Pos(int initX, int initY);

		void setPos(int newX, int newY);
		int getX();
		int getY();
};

#endif