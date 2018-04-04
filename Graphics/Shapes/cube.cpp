#include "cube.h"

namespace Graphics {
namespace Shapes {

static float zr;

Cube::Cube(unsigned width) {
    this->width = width;

    this->vbo_id = 0;
    this->ibo_id = 0;

    memcpy(&(this->data), &CUBE_MAP, 24 * sizeof(Graphics::VertexData));

    for (int index = 0, count = 24 * 5; index < count; ++index) {
        ((GLfloat*)this->data)[index] *= width;
    }

    zr = 0.0f;
}

Cube::~Cube() {
    this->freeVBO();
}

void Cube::setTextures(Graphics::Texture::SpriteSheet *sprite_sheet) {
    this->sprite_sheet = sprite_sheet;

    float width_ratio  = sprite_sheet->getWidthRatio(),
          height_ratio = sprite_sheet->getHeightRatio();

        std::cout << width_ratio << ':' << height_ratio << '\n';

    for (int face = 0; face < 24; face += 4) {
        this->data[face].texture.x = (face / 4) * width_ratio;

        this->data[face + 1].texture.x = (face / 4 + 1) * width_ratio;

        this->data[face + 2].texture.x = (face / 4 + 1) * height_ratio;
        this->data[face + 2].texture.y = 1.0f;

        this->data[face + 3].texture.x = (face / 4) * height_ratio;
        this->data[face + 3].texture.y = 1.0f;
    }

    this->initVBO();
}

void Cube::initVBO() {
    if (0 != this->sprite_sheet->getID() && 0 == this->vbo_id) {
        GLuint indeces[24] = {
            0, 1, 2, 3,
            4, 5, 6, 7,
            8, 9, 10, 11,
            12, 13, 14, 15,
            16, 17, 18, 19,
            20, 21, 22, 23
        };

        glGenBuffers(1, &(this->vbo_id));
        glBindBuffer(GL_ARRAY_BUFFER, this->vbo_id);
        glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(Graphics::VertexData), this->data, GL_DYNAMIC_DRAW);

        glGenBuffers(1, &(this->ibo_id));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 24 * sizeof(GLuint), indeces, GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}

void Cube::freeVBO() {
    if (0 != this->vbo_id) {
        glDeleteBuffers(1, &(this->vbo_id));
        glDeleteBuffers(1, &(this->ibo_id));
    }
}

void Cube::render() {
    glTranslatef(320.0f, 240.0f, -96.0f);
    glRotatef(zr, 0.0f, 1.0f, 0.0f);
    glRotatef(zr, 1.0f, 0.0f, 0.0f);
    glBindTexture(GL_TEXTURE_2D, this->sprite_sheet->getID());

    zr += 1.0f;
    if (360.0f <= zr) {
        zr = 0.0f;
    }

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        glBindBuffer(GL_ARRAY_BUFFER, this->vbo_id);
        glBufferSubData(GL_ARRAY_BUFFER, 0, 24 * sizeof(Graphics::VertexData), this->data);

        glTexCoordPointer(2, GL_FLOAT, sizeof(Graphics::VertexData), (GLvoid*)offsetof(Graphics::VertexData, texture));
        glVertexPointer(3, GL_FLOAT, sizeof(Graphics::VertexData), (GLvoid*)offsetof(Graphics::VertexData, position));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo_id);
        glDrawElements(GL_QUADS, 24, GL_UNSIGNED_INT, 0);

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}

}
}
