#ifndef SDLWINDOW
#define SDLWINDOW

#include "Events/UiObject.h"

#include <list>
#include <functional>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <GL/gl.h>
#include <GL/glu.h>

class SdlWindow
{
	private:
		// OpenGL context
		SDL_GLContext glContext;

		// Custom event handler
		std::function<bool(SDL_Event& e)> evtHandler;

		// Resolution independent from window size
		int resolutionWidth;
		int resolutionHeight;

		// Window dimensions
		int windowWidth;
		int windowHeight;

		// Original window dimensions
		int originalWndWidth;
		int originalWndHeight;

		// Window focus
		bool mouseFocus;
		bool keyboardFocus;
		//bool fullScreen;
		bool minimized;
		bool shown;

		// Refreshing flag
		bool refreshSignaled;

	protected:
		//UiObject *gui;

		SDL_Window* window;
		unsigned int windowID;

		void setWindowSdlEvtHandler(std::function<bool(SDL_Event& e)> evth);

	public:
		SdlWindow(char const *title, int x, int y, int w, int h, int resW, int resH, Uint32 windowFlags);
		virtual ~SdlWindow();

		bool handleSdlEvent(SDL_Event& e);

		void show();
		void hide();

		// Shows windows contents
		void signalRefresh();
		void doRefresh();

		int getWindowWidth();
		int getWindowHeight();

		double getWindowWidthScale();
		double getWindowHeightScale();
		
		// Window focii
		bool hasMouseFocus();
		bool hasKeyboardFocus();
		bool isMinimized();
		bool isShown();
};

#endif
