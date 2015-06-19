#ifndef EVENTWINDOWRESIZED
#define EVENTWINDOWRESIZED

class EventWindowResized
{
	public:
		int w, h;
		
		EventWindowResized(int w, int h) : w(w), h(h){}
};

#endif