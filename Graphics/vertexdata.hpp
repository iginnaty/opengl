#pragma once

#include "INCLUDES.h"

namespace Graphics {
    struct Vertex {
        GLfloat x, y, z;
    };

    struct Vertex2D {
        GLfloat x, y;
    };

    struct VertexData {
        Vertex position;
        Vertex2D texture;
    };
}
