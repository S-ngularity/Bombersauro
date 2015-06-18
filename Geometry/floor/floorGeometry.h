#ifndef FLOORGEOMETRY_H
#define FLOORGEOMETRY_H

#include "glSupport.h"
#include "floor/floorShader.h"
#include "matrix4.h"

class FloorGeometry {

public:

    static const int nvertices = 4;
    //static const int nfaces = 2;

    GlBufferObject vPosBo, vColorBo;

    Matrix4 modelMatrix;
    Matrix4 projMatrix;

    FloorGeometry();

    void draw(FloorShader* curSS);
};

#endif
