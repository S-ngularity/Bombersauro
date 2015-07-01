#include "SdlClasses/CustomWindows/MainWindow.h"

#include "Game.h"

#include "Events/EventAggregator.h"
#include "Events/CustomEvents/EventCode.h"

#include <vector>

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

MainWindow::MainWindow() : 
	SdlWindow(	"Bombersauro", 20, 40, 
				SCREEN_WIDTH, SCREEN_HEIGHT, // window size 
				SCREEN_WIDTH, SCREEN_HEIGHT, // window resolution
				SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE) // superclass window constructor
{
	EventAggregator::Instance().getEvent<EventCode>().subscribe(
															[&](EventCode &c){ contentsChanged(c); });

	//createGui();

	setWindowSdlEvtHandler([&](SDL_Event &e){return handleInternalSdlEvent(e);});

	signalRefresh();
}

MainWindow::~MainWindow()
{
	EventAggregator::Instance().getEvent<EventCode>().unsubscribe(
															[&](EventCode &c){ contentsChanged(c); });
}

void MainWindow::contentsChanged(EventCode &c)
{
	if(c.code == UIEVT_CONTENTSCHANGED)
		signalRefresh();
}

bool MainWindow::handleInternalSdlEvent(SDL_Event& event)
{
	Game::Instance().getPlayer().handleSdlEvent(event);

	switch(event.type)
	{
		case SDL_KEYDOWN:
			switch(event.key.keysym.sym)
			{
				case SDLK_r:
				{
					Game::Instance().resetMap();
					Game::Instance().removeObject(mapObject);
					delete mapObject;

					initMapObject();
				}
				break;
			}
		break;
	}
	
	return true;
}

void MainWindow::createGui()
{
	//gui->addChild(bgUi);
}

void MainWindow::initScene()
{
	Game::Instance().getPlayer().getCamera().adjustProjection(getWindowWidth(), getWindowHeight());

	initMapObject();
}

void MainWindow::renderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Game::Instance().render();

	signalRefresh();
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

void MainWindow::initMapObject()
{
	const int mapW = Game::Instance().getMap().getMapWidth();
	const int mapH = Game::Instance().getMap().getMapHeight();

	std::vector<GLfloat> vectorPos(3 * nVerts * mapW * mapH);
	std::vector<GLfloat> vectorColor(3 * nVerts * mapW * mapH);

	// percorre cada posição do mapa preenchendo um único vetor de vértices 
	// com versões (das posições) do cubo original, 
	// transladadas pela posição do mapa e escaladas pela altura do mapa naquela posição
	int posAtual = 0; // <- preguiça de calcular de maneira bonita a posição no vetor
	for(int i = 0; i < mapW; i++)
		for(int j = 0; j < mapH; j++)
			for(int k = 0; k < nVerts; k++)
			{
				glm::vec4 v = glm::vec4(vPos[k*3], vPos[k*3+1], vPos[k*3+2], 1);
				glm::mat4 m = glm::scale(glm::mat4(1.0f), glm::vec3(1, Game::Instance().getMap().Tile(i, j).getH(), 1)) * glm::translate(glm::mat4(1.0f), glm::vec3(i, 0, j));

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

	mapObject = new GlObject(new CubeShader(), nVerts * mapW * mapH, &vectorPos[0], &vectorColor[0]);
	mapObject->setModelMatrix(glm::mat4(1.0f));
	Game::Instance().addObject(mapObject);
}
