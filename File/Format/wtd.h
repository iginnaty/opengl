#pragma once

#include <string>
#include <vector>
#include <fstream>
#include "../../Graphics/vertexdata.hpp"

namespace File {
namespace Format {

template<typename ValueT> struct WTDTuple {
    std::string name;
    ValueT data;
};

typedef WTDTuple<Graphics::Vertex>        WTDPoint;
typedef WTDTuple<std::vector<WTDPoint*> > WTDFace;
typedef WTDTuple<Graphics::Vertex2D>      WTDTexCoord;

struct WTDModelData {
    WTDFace     *face;
    WTDTexCoord *texture;
};

class WhatTextData {
private:
    bool loaded;
    std::string name;
    std::vector<WTDPoint>     points;
    std::vector<WTDFace>      faces;
    std::vector<WTDTexCoord>  texture_coords;
    std::vector<WTDModelData> data;
    std::string spritesheet_path;
    unsigned sprite_size;

    std::vector<std::string> errors;
    std::ifstream input;

    bool getPoint    (WTDPoint &point, bool parse_name);
    bool getFace     (WTDFace &face, bool parse_name);
    bool getTexCoord (WTDTexCoord &texture_coord, bool parse_name);

    bool parsePointBlock    ();
    bool parseFaceBlock     ();
    bool parseTexCoordBlock ();
    bool parseDefineBlock   ();
    bool parseModel         ();
public:
    WhatTextData();
    bool isLoaded();
    std::vector<WTDPoint>     * getPoints();
    std::vector<WTDFace>      * getFaces();
    std::vector<WTDTexCoord>  * getTexCoords();
    std::vector<WTDModelData> * getData();

    void readFromFile(std::string path);
    void print();
    void printPoint    (const WTDPoint &point);
    void printFace     (const WTDFace &face);
    void printTexCoord (const WTDTexCoord &texture_coord);
};

typedef WhatTextData WTD;

}
}
