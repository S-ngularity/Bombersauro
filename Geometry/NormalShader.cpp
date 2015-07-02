#include "NormalShader.h"

#include "glsupport.h"

NormalShader::NormalShader() {

    static const char * const g_shaderFiles[2] = { "./Geometry/vphong.glsl", "./Geometry/fphong.glsl"};

    readAndCompileShader(program, g_shaderFiles[0], g_shaderFiles[1]);

    h_uModelMatrix = safe_glGetUniformLocation(program, "uModelMatrix");
    h_uViewMatrix = safe_glGetUniformLocation(program, "uViewMatrix");
    h_uProjMatrix = safe_glGetUniformLocation(program, "uProjMatrix");

    h_uLightPosition = safe_glGetUniformLocation(program, "uLightPosition");
    h_uLightIntensity = safe_glGetUniformLocation(program, "uLightIntensity");

    h_aPosition = safe_glGetAttribLocation(program, "aPosition");
    h_aColor = safe_glGetAttribLocation(program, "aColor");
    h_aNormal = safe_glGetAttribLocation(program, "aNormal");

    //#ifndef Gl2COMPATIBLE
    //    glBindFragDataLocation(program, 0, "fragColor");
    //#endif

    checkGlErrors();
}
