#include "cubeShader.h"

#include "glsupport.h"

CubeShader::CubeShader() {

    static const char * const g_shaderFiles[2] = { "./Geometry/cube/cube-gl3.vshader", "./Geometry/cube/cube-gl3.fshader"};

    readAndCompileShader(program, g_shaderFiles[0], g_shaderFiles[1]);

    h_uModelMatrix = safe_glGetUniformLocation(program, "uModelMatrix");
    h_uViewMatrix = safe_glGetUniformLocation(program, "uViewMatrix");
    h_uProjMatrix = safe_glGetUniformLocation(program, "uProjMatrix");

    h_aPosition = safe_glGetAttribLocation(program, "aPosition");
    h_aColor = safe_glGetAttribLocation(program, "aColor");

    //#ifndef Gl2COMPATIBLE
    //    glBindFragDataLocation(program, 0, "fragColor");
    //#endif

    checkGlErrors();
}
