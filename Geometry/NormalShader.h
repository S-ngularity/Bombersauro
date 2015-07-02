#ifndef NORMALSHADER_H
#define NORMALSHADER_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "glsupport.h"

class NormalShader {

    public:

    GlProgram program;

    GLint h_uModelMatrix;
	GLint h_uViewMatrix;
	GLint h_uProjMatrix;
	GLint h_uLightPosition;
	GLint h_uLightIntensity;

    GLint h_aPosition;
    GLint h_aColor;
	GLint h_aNormal;


    NormalShader();
};

#endif
