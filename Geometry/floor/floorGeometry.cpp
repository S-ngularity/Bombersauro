#include "floorGeometry.h"

FloorGeometry::FloorGeometry() {

    static GLfloat vPos[3*nvertices] = {
        0, 0, 0,
        1, 0, 0,
        0, 1, 0,
        1, 1, 0
    };

    static GLfloat vColor[3*nvertices] = {
        0.0, 0.0, 0.0,
        0.2, 0.2, 0.2,
        0.7, 0.7, 0.7,
        0.9, 0.9, 0.9
    };

    glBindBuffer(GL_ARRAY_BUFFER, vPosBo);
    glBufferData(GL_ARRAY_BUFFER,3*nvertices*sizeof(GLfloat),vPos,GL_STATIC_DRAW);
    checkGlErrors();

    glBindBuffer(GL_ARRAY_BUFFER, vColorBo);
    glBufferData(GL_ARRAY_BUFFER,3*nvertices*sizeof(GLfloat),vColor,GL_STATIC_DRAW);
    checkGlErrors();
}

void FloorGeometry::draw(FloorShader *curSS) {

    GLfloat modelglmatrix[16];
    modelMatrix.writeToColumnMajorMatrix(modelglmatrix);
    glUniformMatrix4fv(curSS->h_uModelMatrix, 1, 0, &modelglmatrix[0]);

    GLfloat projglmatrix[16];
    projMatrix.writeToColumnMajorMatrix(projglmatrix);
    glUniformMatrix4fv(curSS->h_uProjMatrix, 1, 0, &projglmatrix[0]);

    glEnableVertexAttribArray(curSS->h_aPosition);
    glEnableVertexAttribArray(curSS->h_aColor);

    glBindBuffer(GL_ARRAY_BUFFER, vPosBo);
    glVertexAttribPointer(curSS->h_aPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, vColorBo);
    glVertexAttribPointer(curSS->h_aColor, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, nvertices);

    glDisableVertexAttribArray(curSS->h_aPosition);
    glDisableVertexAttribArray(curSS->h_aColor);
}


