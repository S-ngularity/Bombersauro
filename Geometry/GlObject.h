#ifndef GLOBJECT
#define GLOBJECT

#include <list>
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

#include "Geometry/cube/CubeShader.h"

class GlObject
{
	private:
		CubeShader *shader;

		int numVerts;
		GlBufferObject vboPos, vboColor;

		glm::mat4 modelMatrix;

		//std::function<bool(SDL_Event& e)> evtHandler;
		//std::function<void()> preRenderProcedure;
		//std::function<void()> postRenderProcedure;

		//std::list<GlObject*> childList;

	public:
		GlObject(CubeShader *s, int nVerts, GLfloat *vPos, GLfloat *vColor);
		virtual ~GlObject();

		void setModelMatrix(glm::mat4 m);

		// Render
		void render(glm::mat4 projMatrix, glm::mat4 viewMatrix);
		
		//void addChild(GlObject *c);

		// Events
		//void setSdlEventHandler(std::function<bool(SDL_Event& e)> evth);
		//bool handleSdlEvent(SDL_Event& e);

		// Mouse
		//bool isMouseInside();
		//static bool getRelativeMousePos(GlObject *obj, int *x, int *y);
};

# endif