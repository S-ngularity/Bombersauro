#ifndef FLOORSHADER_H
#define FLOORSHADER_H

#include "glSupport.h"

class FloorShader {

	public:

    GlProgram program;

    GLint h_uModelMatrix;
	GLint h_uProjMatrix;

	GLint h_aPosition;
	GLint h_aColor;

    FloorShader();
};

#endif
