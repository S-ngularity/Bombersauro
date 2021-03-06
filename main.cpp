#include <iostream>
#include <iomanip>

#include <vector>

#include <stdlib.h>
#include <time.h>

#include "Game.h"

#include "SdlClasses/CustomWindows/MainWindow.h"

#include "Events/EventAggregator.h"
#include "Events/CustomEvents/Tick.h"

#include <SDL2/SDL.h>

#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <GL/gl.h>
#include <GL/glu.h>

SDL_Event event;

MainWindow *mainWindow;

// precisa ser (2^n + 1) pro algorítmo de ruído: 
// 65, 129, 257, 513, 1025... (mais de 257 começa a demorar bastante, 1025 usa 900mb de ram)


//funções SDL
bool SDLStart();
void SDLClose();

//opengl
void initGlSettings();

int main(int argc, char* args[])
{
	srand(time(NULL));

	if(!SDLStart())
	{
		std::cout << "ERRO AO INICIALIZAR SDL" << std::endl;

		return -1;
	}

	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	mainWindow = new MainWindow();

	initGlSettings();

	Game::Instance().initGame();
	mainWindow->initScene();
	while(mainWindow->isShown())
	{
		while(SDL_PollEvent(&event))
		{
			mainWindow->handleSdlEvent(event);
		}

		EventAggregator::Instance().getEvent<Tick>().publishEvent(Tick());

		if(!mainWindow->isMinimized())
			mainWindow->renderScene();

		mainWindow->doRefresh();
	}

	delete mainWindow;

	SDLClose();

	return 0;
}

bool SDLStart()
{
	//Initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;

		return false;
	}

	return true;
}

void SDLClose()
{
	//Quit SDL subsystems
	SDL_Quit();
}

void initGlSettings()
{
	glewExperimental = GL_TRUE; 
	GLenum glewError = glewInit();
	if(glewError != GLEW_OK)
	{
		std::cout <<  "Error initializing GLEW! " << glewGetErrorString(glewError);
		mainWindow->hide();
	}

	if(SDL_GL_SetSwapInterval(1) < 0)
		std::cout << "Warning: Unable to set VSync! SDL Error: " << SDL_GetError() << std::endl;

	glClearColor(15.f/255, 15.f/255, 30/255.f, 1.f);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glEnable (GL_DEPTH_TEST);
}
