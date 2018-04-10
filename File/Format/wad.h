#pragma once

#include "../../Graphics/vertexdata.hpp"
#include "wtd.h"
#include <vector>
#include <string>

namespace File {
namespace Format {

struct WADVertexData {
    Graphics::Vertex *position;
    Graphics::VertexI2D *texture;

    unsigned position_index, texture_index;
};

struct WAD_VAO {
    unsigned length;
    Graphics::VertexData *vao;
};

class WheresAlltheData {
private:
    std::vector<Graphics::Vertex>   vertices;
    std::vector<Graphics::Vertex2D> texture_coords;
    std::vector<std::vector<WADVertexData> > data;

    std::string spritesheet_path;
    unsigned sprite_size;

public:
    bool writeToFile(std::string path);
    bool readFromFile(std::string path);
    bool fromWTD(WTD &input);
    WAD_VAO getVAO();
    std::string name;
};

typedef WheresAlltheData WAD;

}
}

