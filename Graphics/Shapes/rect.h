#pragma once
#include "../INCLUDES.h"
#include "../Texture/texture.h"
#include "../vertexdata.hpp"

namespace Graphics {
namespace Shapes {

class Rect {
private:
    unsigned width, height;
    GLuint vbo_id, ibo_id;
    Graphics::Texture::Basic *texture;
    Graphics::VertexData data[4];
public:
    Rect(unsigned width, unsigned height);
    ~Rect();
    void setTexture(Graphics::Texture::Basic *texture);
    void initVBO();
    void freeVBO();
    void render(float x, float y);
};

}
}
