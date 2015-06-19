#ifndef GLCAMERA
#define GLCAMERA

#include <functional>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "glsupport.h"

#define CAMERA_FOV 75.f
#define CAMERA_NEAR 0.1f
#define CAMERA_FAR 500.f

class GlCamera
{
	private:
		glm::mat4 projection;
		void adjustProjection(int w, int h);

		std::function<bool(SDL_Event& e)> evtHandler;

		float angleX, angleY, deltaAngle;
		float angle2X,angle2Y;
		int deltaX, deltaY;
		float x, y, z;
		float lx, ly, lz, deltaMove;

		bool boolAngle, boolMove;
		bool mousePressed;

		void orientMe(float angX, float angY);
		void moveMeFlat(float i);

	public:
		GlCamera(int w, int h, int yHeight);
		virtual ~GlCamera();

		glm::mat4 getView();
		glm::mat4 getProj();

		void tick();

		// Render
		//void render(glm::mat4 projMatrix, glm::mat4 viewMatrix);
		
		// Events
		void setSdlEventHandler(std::function<bool(SDL_Event& e)> evth);
		bool handleSdlEvent(SDL_Event& e);
};

# endif