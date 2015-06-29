#include "SdlClasses/SdlWindow.h"

#include "Events/EventAggregator.h"
#include "Events/CustomEvents/WindowResized.h"

#include <iostream>

#include "glsupport.h"

SdlWindow::SdlWindow(char const *title, int x, int y, int w, int h, int resW, int resH, Uint32 windowFlags)
{
	evtHandler = nullptr;

	originalWndWidth = w;
	originalWndHeight = h;
	windowWidth = w;
	windowHeight = h;
	resolutionWidth = resW;
	resolutionHeight = resH;

	mouseFocus = false;
	keyboardFocus = false;
	minimized = false;
	shown = false;

	refreshSignaled = false;

	window = SDL_CreateWindow(	title,
								x,
								y,
								w,
								h,
								windowFlags);
	
	if(window == NULL)
		std::cout << title << " could not be created! SDL Error: " << SDL_GetError() << std::endl;

	if((windowFlags & SDL_WINDOW_SHOWN) == SDL_WINDOW_SHOWN)
		shown = true;

	windowID = SDL_GetWindowID(window);

	glContext = SDL_GL_CreateContext(window);
	if(glContext == NULL)
		std::cout << "OpenGL context could not be created on window /'" << title << "'! SDL Error: " << SDL_GetError() << std::endl;
	SDL_GL_MakeCurrent(window, glContext);
	glViewport(0, 0, windowWidth, windowHeight);

	//gui = new UiObject(wndRenderer, 0, 0, resW, resH); // create base gui object
}

SdlWindow::~SdlWindow()
{
	if(glContext != NULL)
		SDL_GL_DeleteContext(glContext);

	if(window != NULL)
		SDL_DestroyWindow(window);

	window = NULL;
}

void SdlWindow::setWindowSdlEvtHandler(std::function<bool(SDL_Event& e)> evth)
{
	evtHandler = evth;
}

bool SdlWindow::handleSdlEvent(SDL_Event& e)
{
	// If an event was detected for this window
	if(e.window.windowID == windowID)
	{
		if(e.type == SDL_WINDOWEVENT)
		{
			switch(e.window.event)
			{
				// Window appeared
				case SDL_WINDOWEVENT_SHOWN:
					shown = true;
				break;

				// Window disappeared
				case SDL_WINDOWEVENT_HIDDEN:
					shown = false;
				break;

				// Get new dimensions and repaint
				case SDL_WINDOWEVENT_RESIZED:
					windowWidth = e.window.data1;
					windowHeight = e.window.data2;

					glViewport(0, 0, windowWidth, windowHeight);

					//gui->setWindowScale(getWindowWidthScale(), getWindowHeightScale());
					EventAggregator::Instance()->getEvent<WindowResized>().publishEvent(WindowResized(getWindowWidth(), getWindowHeight()));

					signalRefresh();
				break;

				// Repaint on expose
				case SDL_WINDOWEVENT_EXPOSED:
					signalRefresh();
				break;

				// Mouse enter
				case SDL_WINDOWEVENT_ENTER:
					mouseFocus = true;
				break;
				
				// Mouse exit
				case SDL_WINDOWEVENT_LEAVE:
					mouseFocus = false;
				break;

				// Keyboard focus gained
				case SDL_WINDOWEVENT_FOCUS_GAINED:
					keyboardFocus = true;
				break;
				
				// Keyboard focus lost
				case SDL_WINDOWEVENT_FOCUS_LOST:
					keyboardFocus = false;
				break;

				case SDL_WINDOWEVENT_MINIMIZED:
					minimized = true;
				break;

				case SDL_WINDOWEVENT_MAXIMIZED:
					minimized = false;
				break;
				
				case SDL_WINDOWEVENT_RESTORED:
					minimized = false;
				break;

				// Hide on close
				case SDL_WINDOWEVENT_CLOSE:
					hide();
				break;

				default:
				break;
			}
		}

		else
		{
			// handle other events with the implemented gui handler
			//gui->handleSdlEvent(e);

			// and with the implemented sdl handler
			if(evtHandler)
				evtHandler(e);
		}

		return true;
	} // event had this window's windowId

	return false;
}

void SdlWindow::show()
{
	if(!shown)
	{
		shown = true;
		SDL_ShowWindow(window);
	}

	// always move window forward
	SDL_RaiseWindow(window);
}

void SdlWindow::hide()
{
	shown = false;

	SDL_HideWindow(window);
}

void SdlWindow::signalRefresh()
{
	refreshSignaled = true;
}

void SdlWindow::doRefresh()
{
	if(refreshSignaled)
	{
		if(!minimized && shown)
		{
			//gui->render(0, 0);
			SDL_GL_SwapWindow(window);
			checkGlErrors();
		}

		refreshSignaled = false;
	}
}

int SdlWindow::getWindowWidth()
{
	return windowWidth;
}

int SdlWindow::getWindowHeight()
{
	return windowHeight;
}

double SdlWindow::getWindowWidthScale()
{
	return (double)windowWidth / (double)originalWndWidth;
}

double SdlWindow::getWindowHeightScale()
{
	return (double)windowHeight / (double)originalWndHeight;
}

bool SdlWindow::hasMouseFocus()
{
	return mouseFocus;
}

bool SdlWindow::hasKeyboardFocus()
{
	return keyboardFocus;
}

bool SdlWindow::isMinimized()
{
	return minimized;
}

bool SdlWindow::isShown()
{
	return shown;
}
