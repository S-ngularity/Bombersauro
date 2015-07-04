#ifndef PLAYER
#define PLAYER

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "glsupport.h"

#include "GlCamera.h"
#include "Geometry/GlObject.h"

class Bomba;

class Player
{
	private:
		const int camDist = 4, camHeightY = 1;
		const float camOffset = 0.8;

		float x, y, z;
		float lx, ly, lz;

		float deltaMove, deltaAngle;
		float angleX, angleY;
		float angleTempX,angleTempY;
		int mouseXPosOriginal, mouseYPosOriginal;

		bool boolKeyboardAngle, boolMove, boolDragged;
		bool mousePressed;

		void orientMe(float angX, float angY);
		void moveMeFlat(float i);

		bool allowedToFly;
		bool boolOnTheGround;
		float ySpeed;

		float shootAng;
		float shootForce;

		float perna1Ang, perna2Ang;

		GlObject *playerAvatar;
		GlObject *playerLeg1;
		GlObject *playerLeg2;
		
		GlCamera playerCamera;

		Bomba *bomb;

		void updateAvatarAndCamera();

		void tick();

	public:
		Player();
		~Player();

		void render(glm::mat4 projMatrix, glm::mat4 viewMatrix);

		void resetPos();

		bool handleSdlEvent(SDL_Event& e);

		GlCamera& getCamera();
};

#endif