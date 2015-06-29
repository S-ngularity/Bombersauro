#ifndef EVENTWINDOWRESIZED
#define EVENTWINDOWRESIZED

class WindowResized
{
	public:
		int w, h;
		
		WindowResized(int w, int h) : w(w), h(h){}
};

#endif