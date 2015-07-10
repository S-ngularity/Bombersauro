#ifndef GLCAMERA
#define GLCAMERA

//#include <functional>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GlClasses/glsupport.h"

#define CAMERA_FOV 75.f
#define CAMERA_NEAR 0.1f
#define CAMERA_FAR 1000.f

class GlCamera
{
	private:
		glm::mat4 projection;

		//std::function<bool(SDL_Event& e)> evtHandler;

		double x, y, z;
		double lookAtX, lookAtY, lookAtZ;

	public:
		GlCamera();
		virtual ~GlCamera();

		glm::mat4 getView();
		glm::mat4 getProj();

		void setPos(double newX, double newY, double newZ);
		void lookAtPos(double newX, double newY, double newZ);

		void adjustProjection(int w, int h);

		//void tick();

		// Render
		//void render(glm::mat4 projMatrix, glm::mat4 viewMatrix);
		
		// Events
		//void setSdlEventHandler(std::function<bool(SDL_Event& e)> evth);
		//bool handleSdlEvent(SDL_Event& e);
};

# endif