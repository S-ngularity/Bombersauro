#ifndef BOMBAAAA
#define BOMBAAAA

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "glsupport.h"

#include "Geometry/GlObject.h"

class Bomba
{
	private:
		float xOrig, yOrig, zOrig;
		float xAtual, yAtual, zAtual;
		float anguloAltura;
		float shootForce;

		float lx, ly, lz;

		float tAtual;

		void orientMe(float angX, float angY);
		void moveMeFlat(float i);

		GlObject *bombaObj;

		void tick();

		void funCurvAtualiza();

	public:
		Bomba(float x, float y, float z, float ang, float force, float dirx, float diry, float dirz);
		~Bomba();

		void render(glm::mat4 projMatrix, glm::mat4 viewMatrix);
};

#endif