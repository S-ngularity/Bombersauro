#ifndef CUBESHADER_H
#define CUBESHADER_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "glsupport.h"

class CubeShader {

    public:

    GlProgram program;

    GLint h_uModelMatrix;
	GLint h_uViewMatrix;
	GLint h_uProjMatrix;

    GLint h_aPosition;
    GLint h_aColor;

    CubeShader();
};

#endif
