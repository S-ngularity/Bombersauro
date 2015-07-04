#ifndef NOISE
#define NOISE

#include "Map/Map.h"

#ifndef NOISESTATES
#define NOISESTATES
typedef enum {running = 0, done} State;
#endif

class Noise
{
	public:
		virtual ~Noise() {};
		
		virtual void setMap(Map *m) = 0;

		virtual void reset() = 0;
		
		virtual void runOnce() = 0;

		virtual int getPercentComplete() = 0;
};

#endif
