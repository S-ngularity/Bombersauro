#include <iostream>
#include <iomanip>

#include <vector>

#include <stdlib.h>
#include <time.h>

#include "Map.h"
#include "Noises/DiamSqNoise.h"

#include "SdlClasses/CustomWindows/MainWindow.h"

#include "Events/EventAggregator.h"
#include "Events/CustomEvents/EventCodes.h"

#include "Geometry/GlObject.h"

#include <SDL2/SDL.h>

#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

SDL_Event event;

MainWindow *mainWindow;

// precisa ser (2^n + 1) pro algorítmo de ruído: 
// 65, 129, 257, 513, 1025... (mais de 257 começa a demorar bastante, 1025 usa 900mb de ram)
const int mapW = 129, mapH = 129;
Map worldMap(mapW, mapH);
DiamSqNoise dsNoise(&worldMap);

GlObject *cube;

void renderAll();

//funções SDL
bool SDLStart();
void SDLClose();

//opengl
void initGlSettings();

void initObjects();

int main(int argc, char* args[])
{
	srand(time(NULL));

	while(dsNoise.getPercentComplete() < 100)
	{
		dsNoise.runOnce();
	}
	worldMap.normalize(25); // normaliza altura máxima do mapa, originalmente é 255

	if(!SDLStart())
	{
		std::cout << "ERRO AO INICIALIZAR SDL" << std::endl;

		return -1;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	mainWindow = new MainWindow(&worldMap);

	initGlSettings();

	initObjects();

	while(mainWindow->isShown())
	{
		while(SDL_PollEvent(&event))
		{
			mainWindow->handleSdlEvent(event);
		}

		renderAll();
		mainWindow->doRefresh();
	}

	delete mainWindow;

	SDLClose();

	return 0;
}

void renderAll()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	cube->render(mainWindow->getProj(), mainWindow->getView());

	EventAggregator::Instance()->getEvent<EventCodes>().publishEvent(EventCodes(RENDER_HAPPENED));
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

	glClearColor(128.f/255, 200.f/255, 1.f, 1.f);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glEnable (GL_DEPTH_TEST);
}

const int nVerts = 36;
GLfloat vPos[3*nVerts] = {
    // baixo
    0, 0, 0,
    0, 1, 0,
    1, 0, 0,
    1, 0, 0,
    0, 1, 0,
    1, 1, 0,

    // frente
    0, 0, 0,
    1, 0, 0,
    0, 0, 1,
    0, 0, 1,
    1, 0, 0,
    1, 0, 1,

    // lado direito
    1, 0, 0,
    1, 1, 0,
    1, 0, 1,
    1, 0, 1,
    1, 1, 0,
    1, 1, 1,

    // cima
    1, 0, 1,
    1, 1, 1,
    0, 0, 1,
    0, 0, 1,
    1, 1, 1,
    0, 1, 1,

    // lado esquerdo
    0, 0, 0,
    0, 0, 1,
    0, 1, 0,
    0, 1, 0,
    0, 0, 1,
    0, 1, 1,

    // fundo
    0, 1, 1,
    1, 1, 1,
    0, 1, 0,
    0, 1, 0,
    1, 1, 1,
    1, 1, 0
};

GLfloat vColor[3*nVerts] = {
    0.396, 0.262, 0.129,
    0.396, 0.262, 0.129,
    0.396, 0.262, 0.129,
    0.396, 0.262, 0.129,
    0.396, 0.262, 0.129,
    0.396, 0.262, 0.129,

    0.396, 0.262, 0.129,
    0.396, 0.262, 0.129,
    0.396, 0.262, 0.129,
    0.396, 0.262, 0.129,
    0.396, 0.262, 0.129,
    0.396, 0.262, 0.129,

    0.396, 0.262, 0.129,
    0.396, 0.262, 0.129,
    0.396, 0.262, 0.129,
    0.396, 0.262, 0.129,
    0.396, 0.262, 0.129,
    0.396, 0.262, 0.129,

    0.396, 0.262, 0.129,
    0.396, 0.262, 0.129,
    0.396, 0.262, 0.129,
    0.396, 0.262, 0.129,
    0.396, 0.262, 0.129,
    0.396, 0.262, 0.129,

    0.396, 0.262, 0.129,
    0.396, 0.262, 0.129,
    0.396, 0.262, 0.129,
    0.396, 0.262, 0.129,
    0.396, 0.262, 0.129,
    0.396, 0.262, 0.129,

    0.0, 0.7, 0.0,
    0.0, 0.65, 0.0,
    0.0, 0.6, 0.0,
    0.0, 0.6, 0.0,
    0.0, 0.65, 0.0,
    0.0, 0.6, 0.0
};

void initObjects()
{
	std::vector<GLfloat> vectorPos(3 * nVerts * mapW * mapH);
	std::vector<GLfloat> vectorColor(3 * nVerts * mapW * mapH);

	// percorre cada posição do mapa preenchendo um único vetor de vértices 
	// com versões (das posições) do cubo original, 
	// transladadas pela posição do mapa e escaladas pela altura do mapa naquela posição
	int posAtual = 0; // <- preguiça de calcular de maneira bonita a posição no vetor
	for(int i = 0; i < worldMap.getMapWidth(); i++)
		for(int j = 0; j < worldMap.getMapHeight(); j++)
			for(int k = 0; k < nVerts; k++)
			{
				glm::vec4 v = glm::vec4(vPos[k*3], vPos[k*3+1], vPos[k*3+2], 1);
				glm::mat4 m = glm::scale(glm::mat4(1.0f), glm::vec3(1, worldMap.Tile(i, j).getH(), 1)) * glm::translate(glm::mat4(1.0f), glm::vec3(i, 0, j));

				v = m * v;	

				vectorPos[posAtual] = v[0];
				vectorColor[posAtual] = vColor[k*3];
				posAtual++;

				vectorPos[posAtual] = v[1];
				vectorColor[posAtual] = vColor[k*3 + 1];
				posAtual++;

				vectorPos[posAtual] = v[2];
				vectorColor[posAtual] = vColor[k*3 + 2];
				posAtual++;
			}

	cube = new GlObject(new CubeShader(), nVerts*mapW*mapH, &vectorPos[0], &vectorColor[0]);
	cube->setModelMatrix(glm::mat4(1.0f));
}
