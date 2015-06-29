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

#include "Map.h"

class Player
{
	private:
		const int camDist = 4, camHeightY = 1;

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

		bool boolIsJumping;
		float jumpSpeed;
		float fallSpeed;

		GlObject *playerAvatar;
		GlCamera *playerCamera;

		void updateAvatarAndCamera();

		Map *worldMap;

		void tick();

	public:
		Player(Map *m, GlCamera *cam);
		~Player();

		void render(glm::mat4 projMatrix, glm::mat4 viewMatrix);

		bool handleSdlEvent(SDL_Event& e);
};

#endif