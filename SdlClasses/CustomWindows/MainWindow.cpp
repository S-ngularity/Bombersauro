#include "SdlClasses/CustomWindows/MainWindow.h"

#include "Game.h"

#include "Events/EventAggregator.h"
#include "Events/CustomEvents/EventCode.h"

#include "Geometry/objloader.h"

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



void MainWindow::initMapObject()
{
	const int mapW = Game::Instance().getMap().getMapWidth();
	const int mapH = Game::Instance().getMap().getMapHeight();

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	
	loadOBJ("Geometry/cube.obj", vertices, uvs, normals);

	std::vector<GLfloat> vectorPos(3 * vertices.size() * mapW * mapH);
	std::vector<GLfloat> vectorColor(3 * vertices.size() * mapW * mapH);
	std::vector<GLfloat> vectorNormals(3 * normals.size() * mapW * mapH);

	// percorre cada posição do mapa preenchendo um único vetor de vértices 
	// com versões (das posições) do cubo original, 
	// transladadas pela posição do mapa e escaladas pela altura do mapa naquela posição
	int posAtual = 0; // <- preguiça de calcular de maneira bonita a posição no vetor
	int posAtualNorms = 0;
	for(int i = 0; i < mapW; i++)
		for(int j = 0; j < mapH; j++)
		{
			glm::mat4 m = glm::scale(glm::mat4(1.0f), glm::vec3(1, Game::Instance().getMap().Tile(i, j).getH(), 1)) * glm::translate(glm::mat4(1.0f), glm::vec3(i, 0, j));
			
			for(int k = 0; k < (int)vertices.size(); k++)
			{
				glm::vec4 v = glm::vec4(vertices[k].x, vertices[k].y, vertices[k].z, 1);

				v = m * v;	

				vectorPos[posAtual] = v.x;
				vectorColor[posAtual] = 0.396;
				++posAtual;

				vectorPos[posAtual] = v.y;
				vectorColor[posAtual] = 0.262;
				++posAtual;

				vectorPos[posAtual] = v.z;
				vectorColor[posAtual] = 0.129;
				++posAtual;
			}

			for(int k = 0; k < (int)normals.size(); k++)
			{
				glm::vec4 vN = glm::vec4(normals[k].x, normals[k].y, normals[k].z, 0);

				vN = m * vN;

				vectorNormals[posAtualNorms] = vN.x;
				++posAtualNorms;
				vectorNormals[posAtualNorms] = vN.y;
				++posAtualNorms;
				vectorNormals[posAtualNorms] = vN.z;
				++posAtualNorms;
			}
		}

	mapObject = new GlObject(Game::Instance().getNormalShader(), 
								vertices.size()  * mapW * mapH, 
								&vectorPos[0], 
								&vectorColor[0], 
								normals.size()  * mapW * mapH, &vectorNormals[0]);

	Game::Instance().addObject(mapObject);
}
