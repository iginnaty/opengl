#pragma once

#include <string>
#include <vector>
#include <fstream>
#include "../../Graphics/vertexdata.hpp"

#define DEBUG_WTD 1

namespace File {
namespace Format {

template<typename ValueT> struct WTDTuple {
    std::string name;
    ValueT data;
};

typedef WTDTuple<Graphics::Vertex>       WTDPoint;
typedef WTDTuple<std::vector<unsigned> > WTDPoly;
typedef WTDTuple<std::vector<Graphics::Vertex2D> > WTDTexCoord;

struct WTDFace {
    unsigned vertices;
    unsigned texture_coords;
};

class WhatTextData {
private:
    std::string name;
    std::string spritesheet_path;
    unsigned sprite_size;

    std::vector<WTDPoint>    lib_points;
    std::vector<WTDTexCoord> lib_texture_coords;
    std::vector<WTDPoly>     lib_faces;
    std::vector<WTDFace>     model_data;
public:
    std::vector<std::string> errors;

    bool readFromFile(std::string path);
    bool writeToFile(std::string path);

    std::vector<WTDPoint>* getPointLib() {
        return &this->lib_points;
    }

    std::vector<WTDTexCoord>* getTexCoordLib() {
        return &this->lib_texture_coords;
    }

    std::vector<WTDPoly>* getFacesLib() {
        return &this->lib_faces;
    }

    std::vector<WTDFace>* getModelData() {
        return &this->model_data;
    }

    void setName(std::string name) {
        this->name = name;
    }

    void setSpritesheetPath(std::string path) {
        this->spritesheet_path = path;
    }

    void setSpriteSize(unsigned size) {
        this->sprite_size = size;
    }

    #if DEBUG_WTD
    void print();
    #endif
};

typedef WhatTextData WTD;

}
}
