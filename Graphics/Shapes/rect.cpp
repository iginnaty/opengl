#include "rect.h"

namespace Graphics {
namespace Shapes {

GLfloat x, xr;

Rect::Rect(unsigned width, unsigned height) : data{0.0f} {
    this->width  = width;
    this->height = height;

    this->vbo_id = 0;
    this->ibo_id = 0;

    this->data[1].position.x = width;

    this->data[2].position.x = width;
    this->data[2].position.y = height;

    this->data[3].position.y = height;

    x = 0.0f;
    xr = 0.0f;
}

Rect::~Rect() {
    this->freeVBO();
}

void Rect::setTexture(Graphics::Texture::Basic *texture) {
    this->texture = texture;

    this->data[1].texture.x = texture->getWidthRatio();

    this->data[2].texture.x = texture->getWidthRatio();
    this->data[2].texture.y = texture->getHeightRatio();

    this->data[3].texture.y = texture->getHeightRatio();

    this->initVBO();
}

void Rect::initVBO() {
    if (0 != this->texture->getID() && 0 == this->vbo_id) {
        Graphics::VertexData vertices[4];
        GLuint indeces[4] = { 0, 1, 2, 3 };

        glGenBuffers(1, &(this->vbo_id));
        glBindBuffer(GL_ARRAY_BUFFER, this->vbo_id);
        glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Graphics::VertexData), vertices, GL_DYNAMIC_DRAW);

        glGenBuffers(1, &(this->ibo_id));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indeces, GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}

void Rect::freeVBO() {
    if (0 != this->vbo_id) {
        glDeleteBuffers(1, &(this->vbo_id));
        glDeleteBuffers(1, &(this->ibo_id));
    }
}

void Rect::render(float xt, float y) {
    glTranslatef(320.0f, 240.0f, -32.0f);
    glRotatef(x, 0.0f, 1.0f, 0.0f);
    glRotatef(x, 0.0f, 0.0f, 1.0f);

    x += 1.0f;

    if (x >= 360.0f) {
        x = 0.0f;
    }

    glBindTexture(GL_TEXTURE_2D, this->texture->getID());

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        glBindBuffer(GL_ARRAY_BUFFER, this->vbo_id);
        glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(Graphics::VertexData), this->data);

        glTexCoordPointer(2, GL_FLOAT, sizeof(Graphics::VertexData), (GLvoid*)offsetof(Graphics::VertexData, texture));
        glVertexPointer(3, GL_FLOAT, sizeof(Graphics::VertexData), (GLvoid*)offsetof(Graphics::VertexData, position));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo_id);
        glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, 0);

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}

}
}

