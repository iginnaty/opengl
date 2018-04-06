#pragma once

#include <string>
#include <vector>
#include "../../Graphics/vertexdata.hpp"

namespace File {
namespace Format {

template<typename ValueT> struct WTDTuple {
    std::string name;
    ValueT data;
};

typedef WTDTuple<Graphics::Vertex>                WTDPoint;
typedef WTDTuple<std::vector<Graphics::Vertex*> > WTDFace;
typedef WTDTuple<Graphics::Vertex2D>              WTDTexCoord;

class WhatTextData {
private:
    bool loaded;
    std::string name;
    std::vector<WTDPoint>    points;
    std::vector<WTDFace>     faces;
    std::vector<WTDTexCoord> texture_coords;
public:
    WhatTextData();
    bool isLoaded();
    std::vector<WTDPoint> *    getPoints();
    std::vector<WTDFace> *     getFaces();
    std::vector<WTDTexCoord> * getTexCoords();
};

typedef WhatTextData WTD;

}
}
