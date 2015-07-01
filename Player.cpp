#include "Player.h"

#include "Events/EventAggregator.h"
#include "Events/CustomEvents/Tick.h"

#include "Geometry/objloader.h"

#include <vector>

#include "Game.h"

#define MOUSE_SENSIBILITY 0.004

#define GRAVITY 0.004
#define JUMP_START_SPEED 0.1

#define MOVEMENT_SPEED 0.1
#define TURN_SPEED 0.03f

const int nVerts = 36;
GLfloat vAvatarPos[3*nVerts] = {
    // baixo
    -0.5, -0.5, -0.5,
    -0.5, 0.5, -0.5,
    0.5, -0.5, -0.5,
    0.5, -0.5, -0.5,
    -0.5, 0.5, -0.5,
    0.5, 0.5, -0.5,

    // frente
    -0.5, -0.5, -0.5,
    0.5, -0.5, -0.5,
    -0.5, -0.5, 0.5,
    -0.5, -0.5, 0.5,
    0.5, -0.5, -0.5,
    0.5, -0.5, 0.5,

    // lado direito
    0.5, -0.5, -0.5,
    0.5, 0.5, -0.5,
    0.5, -0.5, 0.5,
    0.5, -0.5, 0.5,
    0.5, 0.5, -0.5,
    0.5, 0.5, 0.5,

    // cima
    0.5, -0.5, 0.5,
    0.5, 0.5, 0.5,
    -0.5, -0.5, 0.5,
    -0.5, -0.5, 0.5,
    0.5, 0.5, 0.5,
    -0.5, 0.5, 0.5,

    // lado esquerdo
    -0.5, -0.5, -0.5,
    -0.5, -0.5, 0.5,
    -0.5, 0.5, -0.5,
    -0.5, 0.5, -0.5,
    -0.5, -0.5, 0.5,
    -0.5, 0.5, 0.5,

    // fundo
    -0.5, 0.5, 0.5,
    0.5, 0.5, 0.5,
    -0.5, 0.5, -0.5,
    -0.5, 0.5, -0.5,
    0.5, 0.5, 0.5,
    0.5, 0.5, -0.5
};

GLfloat vAvatarColor[3*nVerts] = {
    0.2, 0.2, 0.2,
    0.2, 0.2, 0.2,
    0.2, 0.2, 0.2,
    0.2, 0.2, 0.2,
    0.2, 0.2, 0.2,
    0.2, 0.2, 0.2,

    0.2, 0.2, 0.2,
    0.2, 0.2, 0.2,
    0.2, 0.2, 0.2,
    0.2, 0.2, 0.2,
    0.2, 0.2, 0.2,
    0.2, 0.2, 0.2,

    0.2, 0.2, 0.2,
    0.2, 0.2, 0.2,
    0.2, 0.2, 0.2,
    0.2, 0.2, 0.2,
    0.2, 0.2, 0.2,
    0.2, 0.2, 0.2,

    0.2, 0.2, 0.2,
    0.2, 0.2, 0.2,
    0.2, 0.2, 0.2,
    0.2, 0.2, 0.2,
    0.2, 0.2, 0.2,
    0.2, 0.2, 0.2,

    0.2, 0.2, 0.2,
    0.2, 0.2, 0.2,
    0.2, 0.2, 0.2,
    0.2, 0.2, 0.2,
    0.2, 0.2, 0.2,
    0.2, 0.2, 0.2,

    0.2, 0.2, 0.2,
    0.2, 0.2, 0.2,
    0.2, 0.2, 0.2,
    0.2, 0.2, 0.2,
    0.2, 0.2, 0.2,
    0.2, 0.2, 0.2
};


Player::Player() : angleTempX(glm::radians(135.f)), angleTempY(glm::radians(25.f)), mouseXPosOriginal(0), mouseYPosOriginal(0)
{
	mousePressed = false;
	boolKeyboardAngle = false;
	boolMove = false;

	boolOnTheGround = false;
	ySpeed = 0;

	angleX = glm::radians(135.f);
	angleY = glm::radians(25.f);
	orientMe(angleX, angleY);

	x = 0.5f;
	y = Game::Instance().getMap().Tile(0, 0).getH() + 0.5f; // +0.5 para avatar não ficar metade dentro do chão
	z = 0.5f;

	deltaAngle = 0.0;
	deltaMove=0.0;

	//playerAvatar = new GlObject(new CubeShader(), nVerts, &vAvatarPos[0], &vAvatarColor[0]);

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals; // Won't be used at the moment.

	loadOBJ("Geometry/dinosaur.obj", vertices, uvs, normals);

	std::vector<GLfloat> vColor;
	for(int i=0; i < (int)vertices.size(); i++){
		vColor.push_back(0.1);
		vColor.push_back(0.5);
		vColor.push_back(0.1);
	}

	std::vector<GLfloat> vPos;
	for(int i=0; i < (int)vertices.size(); i++){
		vPos.push_back(vertices[i].x);
		vPos.push_back(vertices[i].y);
		vPos.push_back(vertices[i].z);
	}

	playerAvatar = new GlObject(new CubeShader(), vertices.size(), &vPos[0], &vColor[0]);
	
	updateAvatarAndCamera();

	EventAggregator::Instance().getEvent<Tick>().subscribe( [&](Tick &e){ tick(); });
}

Player::~Player()
{
	EventAggregator::Instance().getEvent<Tick>().unsubscribe( [&](Tick &e){ tick(); });
}

void Player::render(glm::mat4 projMatrix, glm::mat4 viewMatrix)
{
	playerAvatar->render(projMatrix, viewMatrix);
}

void Player::updateAvatarAndCamera()
{
	float angX, angY;

	if(!mousePressed)
	{
		angX = angleX;
		angY = angleY;
	}
	else
	{
		angX = angleTempX;
		angY = angleTempY;
	}

	playerAvatar->setModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z)) 
									* glm::rotate(glm::mat4(1.0f), -angX + glm::radians(180.f), glm::vec3(0, 1, 0))
									* glm::translate(glm::mat4(1.0f), glm::vec3(0.75, -0.42, 0))
									* glm::scale(glm::mat4(1.0f), glm::vec3(0.6, 0.6, 0.6)) );
									//* glm::rotate(glm::mat4(1.0f), -angY, glm::vec3(1, 0, 0)));
	playerCamera.setPos(	x - camDist * lx, 
							y + camHeightY + camDist * (-ly), 
							z - camDist * lz);
	playerCamera.lookAtPos(x + 10 * lx, 
							y + 10 * ly, 
							z + 10 * lz);
}

void Player::tick()
{
	if (boolMove)
		moveMeFlat(deltaMove);

	if (boolKeyboardAngle)
	{
		angleX += deltaAngle;
		
		if(!mousePressed)
			orientMe(angleX, angleY);
		else
		{
			int mouseX, mouseY;
			SDL_GetMouseState(&mouseX, &mouseY);

			// recalcula ângulo temporário do movimento do mouse com o novo angleX
			angleTempX = angleX + (mouseX - mouseXPosOriginal) * MOUSE_SENSIBILITY;
			orientMe(angleTempX, angleTempY);

		}
	}

	y += ySpeed;
	ySpeed -= GRAVITY;

	// checa se chegou no chão
	if(y - 0.5f <= Game::Instance().getMap().Tile(x, z).getH()) // -0.5f pra descontar a metade inferior do cubo avatar
	{
		y = Game::Instance().getMap().Tile(x, z).getH() + 0.5f;
		ySpeed = 0;
		boolOnTheGround = true;
	}

	else
		boolOnTheGround = false;

	updateAvatarAndCamera();
}

bool Player::handleSdlEvent(SDL_Event& event)
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
					boolKeyboardAngle = true;
					deltaAngle = -TURN_SPEED;
				}
				break;
				case SDLK_d:
				case SDLK_RIGHT:
				{
					boolKeyboardAngle = true;
					deltaAngle = TURN_SPEED;
				}
				break;
				case SDLK_w:
				case SDLK_UP: 
				{
					boolMove = true;
					deltaMove = MOVEMENT_SPEED;
				}
				break;
				case SDLK_s:
				case SDLK_DOWN: 
				{
					boolMove = true;
					deltaMove = -MOVEMENT_SPEED;
				}		
				break;

				case SDLK_SPACE: 
				{
					//if(boolOnTheGround)
						ySpeed = JUMP_START_SPEED;
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
						boolKeyboardAngle = false;
						deltaAngle = 0.0f;
					}
				break;
				case SDLK_d:
				case SDLK_RIGHT:
					if (deltaAngle > 0.0f)
					{
						boolKeyboardAngle = false;
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

			mouseXPosOriginal = mouseX;
			mouseYPosOriginal = mouseY;

			mousePressed = true;
		break;
		case SDL_MOUSEBUTTONUP:
			angleY = angleTempY;
			angleX = angleTempX;

			mousePressed = false;
		break;

		case SDL_MOUSEMOTION:
			if(mousePressed)
			{
				SDL_GetMouseState(&mouseX, &mouseY);

				// ângulo atual após movimento é o ângulo original + diferença 
				// da pos original e a pos atual do mouse * sensibilidade
				angleTempX = angleX + (mouseX - mouseXPosOriginal) * MOUSE_SENSIBILITY; 
				angleTempY = angleY + (mouseY - mouseYPosOriginal) * MOUSE_SENSIBILITY;

				// evita que camera rode de cabeça pra baixo e flipe 
				// (trava quando olhando completamente pra baixo e tenta ir mais)
				if(angleTempY > 1.57)
					angleTempY = 1.57;

				else if(angleTempY < -1.57)
					angleTempY = -1.57;
				
				lx = cos(angleTempY)*sin(angleTempX);
				lz = -cos(angleTempY)*cos(angleTempX);
				ly = -sin(angleTempY);
			}
		break;
	}

	//if(evtHandler)
	//	evtHandler(event);

	return true;
}

void Player::orientMe(float angX, float angY)
{
	lx = cos(angY)*sin(angX);
	lz = -cos(angY)*cos(angX);
	ly = -sin(angY);
}

void Player::moveMeFlat(float i)
{
	/*x = x + i*lx;
	z = z + i*lz;
	y = y + i*ly;*/

	float proxX = x + i*lx, proxZ = z + i*lz;
	
	if(y - 0.5f >= Game::Instance().getMap().Tile(proxX, proxZ).getH()) // -0.5f pra descontar a metade inferior do cubo avatar
	{
		x = proxX;
		z = proxZ;
	}
}

GlCamera& Player::getCamera()
{
	return playerCamera;
}
