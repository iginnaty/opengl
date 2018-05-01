#pragma once

#include "../../Graphics/vertexdata.hpp"
#include "wtd.h"
#include <vector>
#include <string>

#define DEBUG_WAD 1

namespace File {
namespace Format {

struct WADVertexData {
    unsigned
        position_index,
        texture_index;
};

struct WAD_VAOPrecursor {
    std::vector<Graphics::VertexData> array_object;
    std::vector<GLuint> index_object;
    unsigned size;
    GLenum mode;
};

class WhatTextData;

class WheresAlltheData {
private:
    std::string name;

    std::vector<Graphics::Vertex>    vertices;
    std::vector<Graphics::VertexI2D> texture_coords;
    std::vector<std::vector<WADVertexData> > faces;

    std::string spritesheet_path;
    unsigned sprite_size;

public:
    bool writeToFile(std::string path);
    bool readFromFile(std::string path);
    void clear();
    void compress();
    WAD_VAOPrecursor getVAO(float spritesheet_width, float spritesheet_height);

    WhatTextData toWTD();

    #if DEBUG_WAD
    void loadDebugData();
    void print();
    #endif

    std::vector<Graphics::Vertex> * getVertices() {
        return &this->vertices;
    }

    std::vector<Graphics::VertexI2D> * getTexCoords() {
        return &this->texture_coords;
    }

    std::vector<std::vector<WADVertexData> > * getFaces() {
        return &this->faces;
    }

    void setSpritesheet(std::string path) {
        this->spritesheet_path = path;
    }

    void setSpriteSize(unsigned size) {
        this->sprite_size = size;
    }

    void setName(std::string new_name) {
        this->name = new_name;
    }

    std::string getSpriteSheet() {
        return this->spritesheet_path;
    }
};

typedef WheresAlltheData WAD;

}
}

