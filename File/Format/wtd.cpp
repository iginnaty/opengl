#include "wtd.h"

namespace File {
namespace Format {

WTD::WhatTextData() {
    this->loaded = false;
};

bool WTD::isLoaded() {
    return this->loaded;
}

std::vector<WTDPoint> * WTD::getPoints() {
    return &this->points;
}

std::vector<WTDFace> * WTD::getFaces() {
    return &this->faces;
}

std::vector<WTDTexCoord> * WTD::getTexCoords() {
    return &this->texture_coords;
}

}
}
