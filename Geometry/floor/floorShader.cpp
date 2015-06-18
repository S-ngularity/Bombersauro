#include "floorShader.h"

FloorShader::FloorShader() {

    #ifndef Gl2COMPATIBLE
        static const char * const g_shaderFiles[2] = { "./floor/floor-gl3.vshader", "./floor/floor-gl3.fshader"};
    #else
        static const char * const g_shaderFiles[2] = { "./floor/floor-gl2.vshader", "./floor/floor-gl2.fshader"};
    #endif

    readAndCompileShader(program, g_shaderFiles[0], g_shaderFiles[1]);

	const GLuint h = program;

	h_uModelMatrix = glGetUniformLocation(h, "uModelMatrix");
	h_uProjMatrix = glGetUniformLocation(h, "uProjMatrix");

	h_aPosition = glGetAttribLocation(h, "aPosition");  
	h_aColor = glGetAttribLocation(h, "aColor");

	#ifndef Gl2COMPATIBLE
		glBindFragDataLocation(h, 0, "fragColor");
	#endif
		
	checkGlErrors();
}
