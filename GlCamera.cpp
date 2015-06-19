#include "GlCamera.h"

#include "Events/EventAggregator.h"
#include "Events/CustomEvents/EventWindowResized.h"

#define MOUSE_MOTION_FRACTION 0.004

GlCamera::GlCamera(int w, int h, int yHeight)
{
	angleX = glm::radians(135.f);
	angleY = glm::radians(25.f);
	deltaAngle = 0.0;
	x=-18.0f;
	y = yHeight;
	z=-18.0f;
	lx = 1.0f;
	ly = -0.7f;
	lz = 1.0f;
	deltaMove=0.0;
	deltaX=-1000;

	projection = glm::perspective(glm::radians(CAMERA_FOV), w/(float)h, CAMERA_NEAR, CAMERA_FAR);

	mousePressed = false;
	boolAngle = false;
	boolMove = false;

	EventAggregator::Instance()->getEvent<EventWindowResized>().subscribe(
															[&](EventWindowResized &e){ adjustProjection(e.w, e.h); });

}

GlCamera::~GlCamera()
{

}

void GlCamera::adjustProjection(int w, int h)
{
	projection = glm::perspective(glm::radians(CAMERA_FOV), w/(float)h, CAMERA_NEAR, CAMERA_FAR);
}

glm::mat4 GlCamera::getView()
{
	return glm::lookAt(	glm::vec3(x, y, z), glm::vec3(
						x + 10 * lx, 
						y + 10 * ly, 
						z + 10 * lz),
						glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 GlCamera::getProj()
{
	return projection;
}

void GlCamera::tick()
{
	if (boolMove)
		moveMeFlat(deltaMove);

	if (boolAngle)
	{
		if(!mousePressed)
		{
			angleX += deltaAngle;
			orientMe(angleX, angle2Y);
		}

		else
		{
			int mouseX, mouseY;
			SDL_GetMouseState(&mouseX, &mouseY);

			angleX += deltaAngle;
			angle2X = angleX + (mouseX - deltaX) * MOUSE_MOTION_FRACTION;
			orientMe(angle2X, angle2Y);
		}
	}
}

void GlCamera::setSdlEventHandler(std::function<bool(SDL_Event& e)> evth)
{
	evtHandler = evth;
}

bool GlCamera::handleSdlEvent(SDL_Event& event)
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
					deltaAngle = -0.03f;
				}
				break;
				case SDLK_d:
				case SDLK_RIGHT:
				{
					boolAngle = true;
					deltaAngle = 0.03f;
				}
				break;
				case SDLK_w:
				case SDLK_UP: 
				{
					boolMove = true;
					deltaMove = 0.6;
				}
				break;
				case SDLK_s:
				case SDLK_DOWN: 
				{
					boolMove = true;
					deltaMove = -0.6;
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
			angleX = angle2X;

			mousePressed = false;
		break;

		case SDL_MOUSEMOTION:
			if(mousePressed)
			{
				SDL_GetMouseState(&mouseX, &mouseY);

				angle2X = angleX + (mouseX - deltaX) * MOUSE_MOTION_FRACTION; // ângulo atual aumenta baseado no ângulo original e na diferença da pos original e a pos atual do mouse
				angle2Y = angleY + (mouseY - deltaY) * MOUSE_MOTION_FRACTION;

				// evita que camera rode de cabeça pra baixo e flipe 
				// (trava quando olhando completamente pra baixo e tenta ir mais)
				if(angle2Y > 1.57)
					angle2Y = 1.57;

				else if(angle2Y < -1.57)
					angle2Y = -1.57;
				
				lx = cos(angle2Y)*sin(angle2X);
				lz = -cos(angle2Y)*cos(angle2X);
				ly = -sin(angle2Y);
			}
		break;
	}

	if(evtHandler)
		evtHandler(event);

	return true;
}

void GlCamera::orientMe(float angX, float angY)
{
	lx = cos(angY)*sin(angX);
	lz = -cos(angY)*cos(angX);
	ly = -sin(angY);
}

void GlCamera::moveMeFlat(float i)
{
	x = x + i*lx;
	z = z + i*lz;
	y = y + i*ly;
}
