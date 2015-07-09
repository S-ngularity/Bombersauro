#include "Player.h"

#include "Events/EventAggregator.h"
#include "Events/CustomEvents/Tick.h"

#include "Geometry/objloader.h"

#include <vector>

#include "Game.h"

#include "Bomba.h"

#define MOUSE_SENSIBILITY 0.004

#define GRAVITY 0.004
#define JUMP_START_SPEED 0.1

#define MOVEMENT_SPEED 0.15
#define TURN_SPEED 0.03f
#define LEG_TURNING_SPEED (40*MOVEMENT_SPEED)

#define ANG_SHOOT_START 20
#define ANG_MIN 10
#define ANG_MAX 75
#define ANG_INCREMENT 5

#define SHOOT_FORCE_START 10
#define SHOOT_FORCE_MIN 10
#define SHOOT_FORCE_MAX 60
#define SHOOT_FORCE_INCREMENT 2

Player::Player()
{
	bomb = nullptr;
	
	SDL_GetMouseState(&mouseLastX, &mouseLastY);
	mouseLeftPressed = false;
	mouseRightPressed = false;
	boolKeyboardAngle = false;
	boolMove = false;

	allowedToFly = false;
	boolOnTheGround = false;
	ySpeed = 0;

	cameraAngleX = glm::radians(135.f);
	cameraAngleY = glm::radians(25.f);
	playerAngleX = cameraAngleX;
	playerAngleY = cameraAngleY;
	orientMe();
	camlx = playerlx;
	camlz = playerlz;
	camly = playerly;

	x = 0.5f;
	y = Game::Instance().getMap().getH(0, 0);
	z = 0.5f;

	deltaAngle = 0.0;
	deltaMove=0.0;

	shootAng = ANG_SHOOT_START;
	shootForce = SHOOT_FORCE_START;

	perna1Ang = 245.f;
	perna2Ang = 40.f;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	float corDino[3] = {25/255.f, 80/255.f, 25/255.f};

	loadOBJ("Geometry/Objects/dinobody.obj", vertices, uvs, normals);

	std::vector<GLfloat> vColor;
	for(int i=0; i < (int)vertices.size(); i++){
		vColor.push_back(corDino[0]);
		vColor.push_back(corDino[1]);
		vColor.push_back(corDino[2]);
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

	playerAvatar = new GlObject(Game::Instance().getNormalShader(), vertices.size(), &vPos[0], &vColor[0], normals.size(), &vNormals[0]);

	std::vector<glm::vec3> verticesLeg;
	std::vector<glm::vec2> uvsLeg;
	std::vector<glm::vec3> normalsLeg;

	loadOBJ("Geometry/Objects/dinoleg.obj", verticesLeg, uvsLeg, normalsLeg);

	std::vector<GLfloat> vColorLeg1;
	for(int i=0; i < (int)verticesLeg.size(); i++){
		vColorLeg1.push_back(corDino[0]);
		vColorLeg1.push_back(corDino[1]);
		vColorLeg1.push_back(corDino[2]);
	}

	std::vector<GLfloat> vPosLeg1;
	for(int i=0; i < (int)verticesLeg.size(); i++){
		vPosLeg1.push_back(verticesLeg[i].x);
		vPosLeg1.push_back(verticesLeg[i].y);
		vPosLeg1.push_back(verticesLeg[i].z);
	}

	std::vector<GLfloat> vNormalsLeg1;
	for(int i=0; i < (int)normalsLeg.size(); i++){
		vNormalsLeg1.push_back(normalsLeg[i].x);
		vNormalsLeg1.push_back(normalsLeg[i].y);
		vNormalsLeg1.push_back(normalsLeg[i].z);
	}

	playerLeg1 = new GlObject(Game::Instance().getNormalShader(), verticesLeg.size(), &vPosLeg1[0], &vColorLeg1[0], normalsLeg.size(), &vNormalsLeg1[0]);

	std::vector<GLfloat> vPosLeg2(vPosLeg1);
	std::vector<GLfloat> vColorLeg2(vColorLeg1);
	std::vector<GLfloat> vNormalsLeg2(vNormalsLeg1);
	playerLeg2 = new GlObject(Game::Instance().getNormalShader(), verticesLeg.size(), &vPosLeg2[0], &vColorLeg2[0], normalsLeg.size(), &vNormalsLeg2[0]);
	
	updateAvatarAndCamera();

	EventAggregator::Instance().getEvent<Tick>().subscribe( [&](Tick &e){ tick(); });
}

Player::~Player()
{
	EventAggregator::Instance().getEvent<Tick>().unsubscribe( [&](Tick &e){ tick(); });

	delete playerAvatar;
	delete playerLeg1;
	delete playerLeg2;
}

void Player::render(glm::mat4 projMatrix, glm::mat4 viewMatrix)
{
	playerAvatar->render(projMatrix, viewMatrix);
	playerLeg1->render(projMatrix, viewMatrix);
	playerLeg2->render(projMatrix, viewMatrix);
}

void Player::resetPos()
{
	x = 0.5f;
	y = Game::Instance().getMap().getH(0, 0);
	z = 0.5f;
}

void Player::updateAvatarAndCamera()
{
	playerAvatar->setModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z)) // move até posição do player
									* glm::rotate(glm::mat4(1.0f), -playerAngleX + glm::radians(180.f), glm::vec3(0, 1, 0)) // rotaciona pra direção do player
									* glm::translate(glm::mat4(1.0f), glm::vec3(0.25, -0.05, 0)) // ajusta modelo original
									* glm::scale(glm::mat4(1.0f), glm::vec3(0.05, 0.05, 0.05))
									//* glm::rotate(glm::mat4(1.0f), angY, glm::vec3(1, 0, 0)) // rotaciona Y pra direção da câmera
								);

	playerLeg1->setModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z)) // move até posição do player
									* glm::rotate(glm::mat4(1.0f), -playerAngleX + glm::radians(180.f), glm::vec3(0, 1, 0)) // rotaciona pra direção do player
									//* glm::rotate(glm::mat4(1.0f), angY, glm::vec3(1, 0, 0)) // rotaciona Y pra direção da câmera
									* glm::translate(glm::mat4(1.0f), glm::vec3(-0.23, 0.38,  -0.3)) // ajusta modelo
									* glm::scale(glm::mat4(1.0f), glm::vec3(0.03, 0.03, 0.03))
									* glm::rotate(glm::mat4(1.0f), glm::radians(perna1Ang), glm::vec3(1, 0, 0)) // roda perna
								);

	playerLeg2->setModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z)) 
									* glm::rotate(glm::mat4(1.0f), -playerAngleX + glm::radians(180.f), glm::vec3(0, 1, 0))
									//* glm::rotate(glm::mat4(1.0f), angY, glm::vec3(1, 0, 0))
									* glm::translate(glm::mat4(1.0f), glm::vec3(0.29, 0.38, -0.3))
									* glm::scale(glm::mat4(1.0f), glm::vec3(0.03, 0.03, 0.03)) 
									* glm::rotate(glm::mat4(1.0f), glm::radians(perna2Ang), glm::vec3(1, 0, 0))
								);

	playerCamera.setPos(	x - camDist * camlx - cos(cameraAngleX) * camOffset, 
							y + camHeightY + camDist * (-camly), 
							z - camDist * camlz - sin(cameraAngleX) * camOffset);
	playerCamera.lookAtPos(x + 10000 * camlx, 
							y + 10000 * camly, 
							z + 10000 * camlz);
}

void Player::tick()
{
	// botão direito orienta player pra nova direção da camera
	if(mouseRightPressed)
	{
		playerlx = camlx;
		playerlz = camlz;
		playerly = camly;

		playerAngleX = cameraAngleX;
		playerAngleY = cameraAngleY;
	}

	else // só orienta o player pelo teclado quando não controlando direção do player com o mouse
	{
		if (boolKeyboardAngle)
		{
			playerAngleX += deltaAngle;
			
			orientMe();
			
			// só orienta a câmera pra direção do player quando não está sendo controlada pelo mouse
			if(!mouseLeftPressed)
			{
				cameraAngleX = playerAngleX;
				cameraAngleY = playerAngleY;

				camlx = playerlx;
				camly = playerly;
				camlz = playerlz;

			}
		}
	}

	// move player com teclas ou com os dois botões do mouse apertados
	if (boolMove || (mouseLeftPressed && mouseRightPressed))
	{
		// se está movendo com botões do mouse, seta velocidade 
		// de movimento e matém player virado pra direção da câmera
		if(mouseLeftPressed && mouseRightPressed)
		{
			deltaMove = MOVEMENT_SPEED;

			playerlx = camlx;
			playerlz = camlz;
			playerly = camly;

			playerAngleX = cameraAngleX;
			playerAngleY = cameraAngleY;
		}

		moveMeFlat(deltaMove);
	}

	// gravidade
	y += ySpeed;
	ySpeed -= GRAVITY;

	// checa se chegou no chão
	if(y <= Game::Instance().getMap().getH(x, z))
	{
		y = Game::Instance().getMap().getH(x, z);
		ySpeed = 0;
		boolOnTheGround = true;
	}

	else
		boolOnTheGround = false;

	updateAvatarAndCamera();
}

bool Player::handleSdlEvent(SDL_Event& event)
{
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
					if(boolOnTheGround || allowedToFly)
						ySpeed = JUMP_START_SPEED;
				}		
				break;

				case SDLK_e: 
				{
					bomb = new Bomba(x, y, z, shootAng, shootForce, playerlx, playerly, playerlz);
				}		
				break;

				case SDLK_f: 
				{
					allowedToFly = !allowedToFly;
				}		
				break;

				case SDLK_1: 
				{
					shootForce -= SHOOT_FORCE_INCREMENT;
					if(shootForce < SHOOT_FORCE_MIN)
						shootForce = SHOOT_FORCE_MIN;
				}		
				break;

				case SDLK_2: 
				{
					shootForce += SHOOT_FORCE_INCREMENT;
					if(shootForce > SHOOT_FORCE_MAX)
						shootForce = SHOOT_FORCE_MAX;
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

		case SDL_MOUSEWHEEL:
			if(event.wheel.y > 0)
			{
				shootAng += ANG_INCREMENT;
				if(shootAng > ANG_MAX)
					shootAng = ANG_MAX;
			}

			else if(event.wheel.y < 0)
			{
				shootAng -= ANG_INCREMENT;
				if(shootAng < ANG_MIN)
					shootAng = ANG_MIN;
			}
		break;

		case SDL_MOUSEBUTTONDOWN:
			SDL_GetMouseState(&mouseLastX, &mouseLastY);
			
			if(event.button.button == SDL_BUTTON_LEFT)
				mouseLeftPressed = true;
			
			else if(event.button.button == SDL_BUTTON_RIGHT)
				mouseRightPressed = true;
		break;
		
		case SDL_MOUSEBUTTONUP:
			
			if(event.button.button == SDL_BUTTON_LEFT)
				mouseLeftPressed = false;
			
			else if(event.button.button == SDL_BUTTON_RIGHT)
				mouseRightPressed = false;
		break;

		case SDL_MOUSEMOTION:
			if(mouseLeftPressed || mouseRightPressed)
			{
				int mouseX, mouseY;
				SDL_GetMouseState(&mouseX, &mouseY);

				// ângulo atual após movimento é o ângulo original + diferença 
				// da pos original e a pos atual do mouse * sensibilidade
				cameraAngleX = cameraAngleX + (mouseX - mouseLastX) * MOUSE_SENSIBILITY; 
				cameraAngleY = cameraAngleY + (mouseY - mouseLastY) * MOUSE_SENSIBILITY;

				mouseLastX = mouseX;
				mouseLastY = mouseY;

				// evita que camera rode de cabeça pra baixo e flipe 
				// (trava quando olhando completamente pra baixo e tenta ir mais)
				if(cameraAngleY > 1.50)
					cameraAngleY = 1.50;

				else if(cameraAngleY < -1.50)
					cameraAngleY = -1.50;
				
				camlx = cos(cameraAngleY)*sin(cameraAngleX);
				camlz = -cos(cameraAngleY)*cos(cameraAngleX);
				camly = -sin(cameraAngleY);
			}
		break;
	}

	//if(evtHandler)
	//	evtHandler(event);

	return true;
}

void Player::orientMe()
{
	playerlx = cos(playerAngleY)*sin(playerAngleX);
	playerlz = -cos(playerAngleY)*cos(playerAngleX);
	playerly = -sin(playerAngleY);
}

void Player::moveMeFlat(float i)
{
	float proxX = x + i*sin(playerAngleX), proxZ = z + i*(-cos(playerAngleX));
	
	if(y >= Game::Instance().getMap().getH(proxX, proxZ))
	{
		if(proxX*playerlx+proxZ*playerlz > x*playerlx+z*playerlz)
		{
			perna1Ang += LEG_TURNING_SPEED;
			perna2Ang += LEG_TURNING_SPEED;;
		}

		else if(proxX*playerlx+proxZ*playerlz < x*playerlx+z*playerlz)
		{
			perna1Ang -= LEG_TURNING_SPEED;
			perna2Ang -= LEG_TURNING_SPEED;;
		}

		if(perna1Ang > 360)
			perna1Ang -= 360;
		
		if(perna2Ang > 360)
			perna2Ang -= 360;

		x = proxX;
		z = proxZ;
	}
}

GlCamera& Player::getCamera()
{
	return playerCamera;
}
