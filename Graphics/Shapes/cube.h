#pragma once
#include "../INCLUDES.h"
#include "../Texture/spritesheet.h"
#include "../vertexdata.hpp"

namespace Graphics {
namespace Shapes {

const static Graphics::VertexData CUBE_MAP[24] = {
    // FRONT FACE
    { { 0, 0, 0 }, { 0 } },
    { { 1, 0, 0 }, { 0 } },
    { { 1, 1, 0 }, { 0 } },
    { { 0, 1, 0 }, { 0 } },
    // BACK FACE
    { { 0, 0, 1 }, { 0 } },
    { { 1, 0, 1 }, { 0 } },
    { { 1, 1, 1 }, { 0 } },
    { { 0, 1, 1 }, { 0 } },
    // TOP FACE
    { { 0, 0, 1 }, { 0 } },
    { { 1, 0, 1 }, { 0 } },
    { { 1, 0, 0 }, { 0 } },
    { { 0, 0, 0 }, { 0 } },
    // BOTTOM FACE
    { { 1, 1, 1 }, { 0 } },
    { { 0, 1, 1 }, { 0 } },
    { { 0, 1, 0 }, { 0 } },
    { { 1, 1, 0 }, { 0 } },
    // LEFT FACE
    { { 0, 0, 1 }, { 0 } },
    { { 0, 0, 0 }, { 0 } },
    { { 0, 1, 0 }, { 0 } },
    { { 0, 1, 1 }, { 0 } },
    // RIGHT FACE
    { { 1, 0, 0 }, { 0 } },
    { { 1, 0, 1 }, { 0 } },
    { { 1, 1, 1 }, { 0 } },
    { { 1, 1, 0 }, { 0 } }
};

class Cube {
protected:
    unsigned width;
    GLuint vbo_id, ibo_id;
    Graphics::Texture::SpriteSheet *sprite_sheet;
    Graphics::VertexData data[24];
public:
    Cube(unsigned width);
    ~Cube();
    void setTextures(Graphics::Texture::SpriteSheet *sprite_sheet);
    void initVBO();
    void freeVBO();
    void render();
};

}
}
