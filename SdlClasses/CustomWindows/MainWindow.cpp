#include "SdlClasses/CustomWindows/MainWindow.h"

#include "Map.h"

#include "Events/EventAggregator.h"
#include "Events/CustomEvents/EventCodes.h"

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

	angle = glm::radians(135.f);
	angleY = glm::radians(25.f);
	deltaAngle = 0.0;
	x=-18.0f;
	y = worldMap->Tile(0, 0).getH() + 35;
	z=-18.0f;
	lx = 1.0f;
	ly = -0.7f;
	lz = 1.0f;
	deltaMove=0.0;
	deltaX=-1000;

	proj = glm::perspective(glm::radians(75.f), getWindowWidth()/(float)getWindowHeight(), 0.1f, 500.f);

	mousePressed = false;
	boolAngle = false;
	boolMove = false;

	EventAggregator::Instance()->getEvent<EventCodes>().subscribe(
															[&](EventCodes &c){ contentsChanged(c); });

	//createGui();

	setWindowSdlEvtHandler([&](SDL_Event &e){return handleInternalSdlEvent(e);});

	signalRefresh();
}

MainWindow::~MainWindow()
{
	EventAggregator::Instance()->getEvent<EventCodes>().unsubscribe(
															[&](EventCodes &c){ contentsChanged(c); });
}

void MainWindow::contentsChanged(EventCodes &c)
{
	if(c.code == UIEVT_CONTENTSCHANGED || c.code == RENDER_HAPPENED)
		signalRefresh();

	if(c.code == WINDOW_RESIZED)
		proj = glm::perspective(glm::radians(75.f), getWindowWidth()/(float)getWindowHeight(), 0.1f, 500.f);
}

glm::mat4 MainWindow::getProj()
{
	return proj;
}

glm::mat4 MainWindow::getView()
{
	return glm::lookAt(	glm::vec3(x, y, z), glm::vec3(
						x + 10 * lx, 
						y + 10 * ly, 
						z + 10 * lz),
						glm::vec3(0.0f, 1.0f, 0.0f));
}

bool MainWindow::handleInternalSdlEvent(SDL_Event& event)
{
	int mouseX, mouseY;
	switch(event.type)
	{
		case SDL_KEYDOWN:
			switch(event.key.keysym.sym)
			{
				case SDLK_a:
				case SDLK_LEFT:
				{
					boolAngle = true;
					deltaAngle = -0.0015f;
				}
				break;
				case SDLK_d:
				case SDLK_RIGHT:
				{
					boolAngle = true;
					deltaAngle = 0.0015f;
				}
				break;
				case SDLK_w:
				case SDLK_UP: 
				{
					boolMove = true;
					deltaMove = 0.5;
				}
				break;
				case SDLK_s:
				case SDLK_DOWN: 
				{
					boolMove = true;
					deltaMove = -0.5;
				}		
				break;
			}
		break;
		case SDL_KEYUP:
			switch(event.key.keysym.sym)
			{
				case SDLK_a:
				case SDLK_LEFT:
					if (deltaAngle < 0.0f)
					{
						boolAngle = false;
						deltaAngle = 0.0f;
					}
				break;
				case SDLK_d:
				case SDLK_RIGHT:
					if (deltaAngle > 0.0f)
					{
						boolAngle = false;
						deltaAngle = 0.0f;
					}
				break;
				case SDLK_w:
				case SDLK_UP:
					if (deltaMove > 0) 
					{
						boolMove = false;
						deltaMove = 0;
					}
				break;
				case SDLK_s:
				case SDLK_DOWN: 			
					if (deltaMove < 0)
					{
						boolMove = false;
						deltaMove = 0;
					}
				break;
			}
		break;

		case SDL_MOUSEBUTTONDOWN:
			SDL_GetMouseState(&mouseX, &mouseY);

			deltaX = mouseX;
			deltaY = mouseY;

			mousePressed = true;
		break;
		case SDL_MOUSEBUTTONUP:
			angleY = angle2Y;
			angle = angle2;

			mousePressed = false;
		break;

		case SDL_MOUSEMOTION:
			if(mousePressed)
			{
				SDL_GetMouseState(&mouseX, &mouseY);

				angle2 = angle + (mouseX-deltaX)*0.005;
				angle2Y = angleY + (mouseY-deltaY) * 0.005;
				if (angle2Y > 1.57)
					angle2Y = 1.57;
				else if (angle2Y < -1.57)
					angle2Y = -1.57;
				lx = cos(angle2Y)*sin(angle2);
				lz = -cos(angle2Y)*cos(angle2);
				ly = -sin(angle2Y);
			}
		break;
	}

	if (boolMove)
		moveMeFlat(deltaMove);

	if (boolAngle)
	{
		angle += deltaAngle;
		orientMe(angle);
	}

	return true;
}

void MainWindow::orientMe(float ang)
{
	lx = sin(ang);
	lz = -cos(ang);
}

void MainWindow::moveMeFlat(float i)
{
	x = x + i*lx;
	z = z + i*lz;
	y = y + i*ly;
}

void MainWindow::createGui()
{
	//gui->addChild(bgUi);
}