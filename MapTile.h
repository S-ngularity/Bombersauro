#ifndef MAPTILE
#define MAPTILE

#define MULTIPLIER 1

class MapTile
{
	private:
		int h;
		int chance;
		bool isSeed;
		bool error;

	public:
		MapTile();

		int getH();
		void setH(int newH);

		int getChance();
		void setBaseChance(); // set chance de próximos tiles manterem altura
		void lowerChance(MapTile prevTile); // diminui a chance desse tile baseada na chance do tile passado

		bool getIsSeed();
		void setIsSeed(bool newIsSeed);

		bool getError();
		void setError(bool newError);
};

#endif