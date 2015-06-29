#ifndef EVENTCODE
#define EVENTCODE

class EventCode
{
	public:
		EventCode(int c) { code = c;}
		int code;
};

#define UIEVT_CONTENTSCHANGED 1
#define UIEVT_RUNNOISEUPDATE 2

#define RENDER_HAPPENED 3
#define WINDOW_RESIZED 4

#endif