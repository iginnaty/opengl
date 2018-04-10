#pragma once

#include "../../Graphics/vertexdata.hpp"
#include "wtd.h"
#include <vector>
#include <string>

#define DEBUG_WAD 1

namespace File {
namespace Format {

struct WADVertexData {
    unsigned position_index, texture_index;

    Graphics::Vertex *position;
    Graphics::VertexI2D *texture;
};

struct WAD_VAO {
    unsigned length;
    Graphics::VertexData *vao;
};

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

    #if DEBUG_WAD
    void loadDebugData();
    void print();
    #endif
};

typedef WheresAlltheData WAD;

}
}

