#include "SdlClasses/CustomWindows/MainWindow.h"

#include "Map.h"

#include "Events/EventAggregator.h"
#include "Events/CustomEvents/EventCode.h"

#include <vector>

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

MainWindow::MainWindow(Map* m) : 
	SdlWindow(	"Bombersauro", 20, 40, 
				SCREEN_WIDTH, SCREEN_HEIGHT, // window size 
				SCREEN_WIDTH, SCREEN_HEIGHT, // window resolution
				SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE) // superclass window constructor
{
	worldMap = m;
	mapObject = nullptr;

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

	if(mapObject != nullptr)
		delete mapObject;
}

void MainWindow::contentsChanged(EventCode &c)
{
	if(c.code == UIEVT_CONTENTSCHANGED)
		signalRefresh();
}

bool MainWindow::handleInternalSdlEvent(SDL_Event& event)
{
	player->handleSdlEvent(event);
	
	return true;
}

void MainWindow::createGui()
{
	//gui->addChild(bgUi);
}

void MainWindow::initScene()
{
	camera = new GlCamera(getWindowWidth(), getWindowHeight());
	player = new Player(worldMap, camera);

	initMapObject();
}

void MainWindow::renderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(mapObject != nullptr)
		mapObject->render(camera->getProj(), camera->getView());
	
	player->render(camera->getProj(), camera->getView());

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
	std::vector<GLfloat> vectorPos(3 * nVerts * worldMap->getMapWidth() * worldMap->getMapHeight());
	std::vector<GLfloat> vectorColor(3 * nVerts * worldMap->getMapWidth() * worldMap->getMapHeight());

	// percorre cada posição do mapa preenchendo um único vetor de vértices 
	// com versões (das posições) do cubo original, 
	// transladadas pela posição do mapa e escaladas pela altura do mapa naquela posição
	int posAtual = 0; // <- preguiça de calcular de maneira bonita a posição no vetor
	for(int i = 0; i < worldMap->getMapWidth(); i++)
		for(int j = 0; j < worldMap->getMapHeight(); j++)
			for(int k = 0; k < nVerts; k++)
			{
				glm::vec4 v = glm::vec4(vPos[k*3], vPos[k*3+1], vPos[k*3+2], 1);
				glm::mat4 m = glm::scale(glm::mat4(1.0f), glm::vec3(1, worldMap->Tile(i, j).getH(), 1)) * glm::translate(glm::mat4(1.0f), glm::vec3(i, 0, j));

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

	mapObject = new GlObject(new CubeShader(), nVerts * worldMap->getMapWidth() * worldMap->getMapHeight(), &vectorPos[0], &vectorColor[0]);
	mapObject->setModelMatrix(glm::mat4(1.0f));
}
