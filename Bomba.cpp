#include "Bomba.h"

#include "Events/EventAggregator.h"
#include "Events/CustomEvents/Tick.h"

#include "Geometry/objloader.h"

#include <vector>

#include "Game.h"

#include <iostream>

Bomba::Bomba(float x, float y, float z, float ang, float force, float dirx, float diry, float dirz)
{
	xOrig = x;
	yOrig = y;
	zOrig = z;

	xAtual = x;
	yAtual = y;
	zAtual = z;

	anguloAltura = ang;
	shootForce = force;

	lx = dirx;
	ly = diry;
	lz = dirz;

	tAtual = 0.f;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals; // Won't be used at the moment.

	loadOBJ("Geometry/bomba.obj", vertices, uvs, normals);

	std::vector<GLfloat> vColor;
	for(int i=0; i < (int)vertices.size(); i++){
		vColor.push_back(0.1);
		vColor.push_back(0.1);
		vColor.push_back(0.1);
	}

	std::vector<GLfloat> vPos;
	for(int i=0; i < (int)vertices.size(); i++){
		vPos.push_back(vertices[i].x);
		vPos.push_back(vertices[i].y);
		vPos.push_back(vertices[i].z);
	}

	bombaObj = new GlObject(new CubeShader(), vertices.size(), &vPos[0], &vColor[0]);
	Game::Instance().addObject(bombaObj);

	EventAggregator::Instance().getEvent<Tick>().subscribe( [&](Tick &e){ tick(); });
}

Bomba::~Bomba()
{
	EventAggregator::Instance().getEvent<Tick>().unsubscribe( [&](Tick &e){ tick(); });
}

void Bomba::render(glm::mat4 projMatrix, glm::mat4 viewMatrix)
{
	bombaObj->render(projMatrix, viewMatrix);
}

void Bomba::tick()
{
	if(tAtual < 1)
	{
		tAtual += 0.05;
		funCurvAtualiza();
	}

	if(bombaObj != nullptr)
	{
		bombaObj->setModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(xOrig + xAtual * lx, yOrig + yAtual, zOrig + xAtual * lz))
									* glm::scale(glm::mat4(1.0f), glm::vec3(0.05, 0.05, 0.05))
								);
	}

	if(tAtual >= 1)
	{
		Game::Instance().removeObject(bombaObj);
		delete bombaObj;
		bombaObj = nullptr;
		//delete this;
	}
}

void Bomba::orientMe(float angX, float angY)
{
	lx = cos(angY)*sin(angX);
	lz = -cos(angY)*cos(angX);
	ly = -sin(angY);
}

void Bomba::moveMeFlat(float i)
{
	/*x = x + i*lx;
	z = z + i*lz;
	y = y + i*ly;*/

	/*float proxX = x + i*lx, proxZ = z + i*lz;
	
	if(y - 0.5f >= Game::Instance().getMap().Tile(proxX, proxZ).getH()) // -0.5f pra descontar a metade inferior do cubo avatar
	{
		x = proxX;
		z = proxZ;
	}*/
}
		
void Bomba::funCurvAtualiza()
{
	float angulo = glm::radians(anguloAltura);
	float forca = shootForce;
	float t = tAtual;

	float x = 0;
	float y = 0;

	float x1 = x + cos(angulo) * forca;
	float y1 = y + sin(angulo) * forca;

	float x2 = x + 2 * cos(angulo) * forca;
	float y2 = y;

	//result = pow((1-t), 2) * p0 + 2 * t * (1 - t) * p1 + pow(t,2) * p2;
	xAtual = pow((1-t), 2) * x + 2*t*(1-t) * x1 + pow(t,2) * x2;
	yAtual = pow((1-t), 2) * y + 2*t*(1-t) * y1 + pow(t,2) * y2;

	//std::cout << "x: " << x << " y: " << y<< std::endl << "x1: " << x1 << " y1: " << y1<< std::endl << "x2: " << x2 << " y2: " << y2 << std::endl;
	//std::cout << "xAtual: " << xAtual << " yAtual: " << yAtual << std::endl;
}