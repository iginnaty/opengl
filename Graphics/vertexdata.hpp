#pragma once

#include "INCLUDES.h"

namespace Graphics {
    struct Vertex {
        GLfloat x, y, z;

    public:
        bool operator == (const Vertex &rhs) {
            return
                this->x == rhs.x &&
                this->y == rhs.y &&
                this->z == rhs.z;
        }
    };

    struct Vertex2D {
        GLfloat x, y;
    };

    struct VertexI2D {
        int x, y;
    public:
        bool operator == (const VertexI2D &rhs) {
            return
                this->x == rhs.x &&
                this->y == rhs.y;
        }
    };

    struct VertexData {
        Vertex position;
        Vertex2D texture;
    };
}
