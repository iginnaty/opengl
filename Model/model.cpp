#include "model.h"

namespace Graphics {

static float zr = 0.0f;

Model::Model() {
    this->vbo_id = 0;
    this->ibo_id = 0;
    this->loaded = false;
}

Model::Model(Graphics::Texture::SpriteSheet *sprite_sheet, WAD &model) : Model() {
    this->sprite_sheet = sprite_sheet;

    this->data = model;
    if (sprite_sheet->isLoaded()) {
        this->loaded = true;
        this->vao_data = this->data.getVAO((float)sprite_sheet->getWidth(), (float)sprite_sheet->getHeight());
    }
}

Model::~Model() {
    this->freeVBO();
}

void Model::initVBO() {
    if (0 == this->sprite_sheet->getID() || 0 != this->vbo_id) {
        return;
    }

    glGenBuffers(1, &this->vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo_id);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(Graphics::VertexData) * this->vao_data.size,
        &this->vao_data.array_object[0],
        GL_DYNAMIC_DRAW
    );
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &this->ibo_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo_id);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(GLuint) * this->vao_data.size,
        &this->vao_data.index_object[0],
        GL_DYNAMIC_DRAW
    );
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Model::freeVBO() {
    if (0 == this->vbo_id) {
        return;
    }

    glDeleteBuffers(1, &this->vbo_id);
    glDeleteBuffers(1, &this->ibo_id);
}

void Model::render() {
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
        glBufferSubData(
            GL_ARRAY_BUFFER,
            0,
            sizeof(Graphics::VertexData) * this->vao_data.size,
            &this->vao_data.array_object[0]
        );

        glTexCoordPointer(2, GL_FLOAT, sizeof(Graphics::VertexData), (GLvoid*)offsetof(Graphics::VertexData, texture));
        glVertexPointer(3, GL_FLOAT, sizeof(Graphics::VertexData), (GLvoid*)offsetof(Graphics::VertexData, position));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo_id);
        glDrawElements(this->vao_data.mode, this->vao_data.size, GL_UNSIGNED_INT, 0);

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}

}

