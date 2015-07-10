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
	dia = false;

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

				case SDLK_l:
				{
					if(!dia)
					{
						Game::Instance().lightPos.x = Game::Instance().getMap().getMapWidth() + 90;
						Game::Instance().lightPos.y = 390;
						Game::Instance().lightPos.z = Game::Instance().getMap().getMapHeight() + 190;
						Game::Instance().lightIntensity.x = 1;
						Game::Instance().lightIntensity.y = 1;
						Game::Instance().lightIntensity.z = 1;

						Game::Instance().removeObject(lua);
						Game::Instance().addObject(sol);

						dia = true;

						glClearColor(34.f/255, 114.f/255, 171/255.f, 1.f);
					}

					else
					{
						Game::Instance().lightPos.x = -90;
						Game::Instance().lightPos.y = 390;
						Game::Instance().lightPos.z = -190;
						Game::Instance().lightIntensity.x = 0.5;
						Game::Instance().lightIntensity.y = 0.5;
						Game::Instance().lightIntensity.z = 0.5;

						Game::Instance().removeObject(sol);
						Game::Instance().addObject(lua);

						dia = false;

						glClearColor(15.f/255, 15.f/255, 30/255.f, 1.f);
					}
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

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	loadOBJ("Geometry/Objects/sphere.obj", vertices, uvs, normals);

	std::vector<GLfloat> vColor;
	for(int i=0; i < (int)vertices.size(); i++){
		vColor.push_back(0.95f);
		vColor.push_back(0.95f);
		vColor.push_back(0.95f);
	}

	std::vector<GLfloat> vPos;
	for(int i=0; i < (int)vertices.size(); i++){
		vPos.push_back(vertices[i].x);
		vPos.push_back(vertices[i].y);
		vPos.push_back(vertices[i].z);
	}

	std::vector<GLfloat> vNormals;
	for(int i=0; i < (int)normals.size(); i++){
		vNormals.push_back(normals[i].x);
		vNormals.push_back(normals[i].y);
		vNormals.push_back(normals[i].z);
	}

	lua = new GlObject(Game::Instance().getNormalShader(), vertices.size(), &vPos[0], &vColor[0], normals.size(), &vNormals[0]);
	lua->setModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(-100, 400, -200)));

	Game::Instance().addObject(lua);

	std::vector<GLfloat> vSolColor;
	std::vector<GLfloat> vSolPos(vPos);
	std::vector<GLfloat> vSolNormals(vNormals);
	for(int i=0; i < (int)vertices.size(); i++){
		vSolColor.push_back(1.f);
		vSolColor.push_back(0.95f);
		vSolColor.push_back(0.1f);
	}

	sol = new GlObject(Game::Instance().getNormalShader(), vertices.size(), &vSolPos[0], &vSolColor[0], normals.size(), &vSolNormals[0]);
	sol->setModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(Game::Instance().getMap().getMapWidth() + 100, 400, Game::Instance().getMap().getMapHeight() + 200)));

	//Game::Instance().addObject(sol);

	Game::Instance().lightPos.x = -90;
	Game::Instance().lightPos.y = 390;
	Game::Instance().lightPos.z = -190;
	Game::Instance().lightIntensity.x = 0.5;
	Game::Instance().lightIntensity.y = 0.5;
	Game::Instance().lightIntensity.z = 0.5;
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
	
	loadOBJ("Geometry/Objects/cube.obj", vertices, uvs, normals);

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
			glm::mat4 m = glm::scale(glm::mat4(1.0f), glm::vec3(1, Game::Instance().getMap().getH(i, j), 1)) * glm::translate(glm::mat4(1.0f), glm::vec3(i, 0, j));
			
			for(int k = 0; k < (int)vertices.size(); k++)
			{
				glm::vec4 v = glm::vec4(vertices[k].x, vertices[k].y, vertices[k].z, 1);

				v = m * v;	

				vectorPos[posAtual] = v.x;
				if(k < 24 || k > 29)
					vectorColor[posAtual] = 140/255.f;//0.396;
				else
					vectorColor[posAtual] = 175/255.f;
				++posAtual;

				vectorPos[posAtual] = v.y;
				if(k < 24 || k > 29)
					vectorColor[posAtual] = 95/255.f;//0.262;
				else
					vectorColor[posAtual] = 175/255.f;
				++posAtual;

				vectorPos[posAtual] = v.z;
				if(k < 24 || k > 29)
					vectorColor[posAtual] = 67/255.f;//0.129;
				else
					vectorColor[posAtual] = 175/255.f;
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
