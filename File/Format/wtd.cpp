#include "wtd.h"
#include <iostream>

#define FILE_PARSER_WTD_DEBUG 1

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

bool WTD::getPoint(WTDPoint &point, bool parse_name) {
    std::string token;
    bool success = true;
    unsigned index = 0;
    float *data = (float*)(&point.data);

    if (parse_name) {
        this->input >> token;

        if ("{" != token) {
            point.name = token;
            this->input >> token;
        }
        else {
            point.name = "ANON" + (unsigned)(this->points.size());
        }
    }

    while (!this->input.eof() && 3 > index) {
        this->input >> data[index];
        this->input >> token;
        ++index;
    }

    if (3 > index) {
        success = false;
        this->errors.push_back("File terminated early");
    }

    return success;
}

bool WTD::parsePointBlock() {
    bool success = true;
    WTDPoint point;

    success = this->getPoint(point, true);

    if (success) {
        this->points.push_back(point);
    }

    return success;
}

bool WTD::getFace(WTDFace &face, bool parse_name) {
    std::string token;
    bool success = true;
    WTDPoint *point;

    if (parse_name) {
        this->input >> token;

        if ("{" != token) {
            face.name = token;
            this->input >> token;
        }
        else {
            face.name = "ANON" + (unsigned)(this->faces.size());
        }
    }

    this->input >> token;

    while (!this->input.eof() && "};" != token && success) {
        point = nullptr;

        if ('{' == token[0]) {
            point = new WTDPoint();

            point->name = "ANON" + (unsigned)(this->points.size());
            this->getPoint(*point, false);

            this->points.push_back(*point);

            this->input >> token;
            if ('}' != token[0]) {
                success = false;
                this->errors.push_back("Expected '}' when reading ANONYMOUS point");
            }
        }
        else if ('~' == token[0]) {
            if (',' == token[token.size() - 1]) {
                token = token.substr(1, token.size() - 2);
            }
            else {
                token = token.erase(0, 1);
            }

            for (auto &point_p : this->points) {
                if (point_p.name == token) {
                    point = &point_p;
                    break;
                }
            }

            if (nullptr == point) {
                success = false;
                this->errors.push_back("Undefined reference to defined point \"" + token + "\"");
            }

            this->input >> token;
        }

        if (nullptr == point) {
            success = false;
        }
        else {
            face.data.push_back(point);
        }
    }

    if (this->input.eof()) {
        success = false;
        this->errors.push_back("File terminated early");
    }

    return success;
}

bool WTD::parseFaceBlock() {
    std::string token;
    bool success = true;
    WTDFace face;

    success = this->getFace(face, true);

    if (success) {
        this->faces.push_back(face);
    }

    return success;
}

bool WTD::getTexCoord(WTDTexCoord &texture_coord, bool parse_name) {
    std::string token;
    bool success = true;

    if (parse_name) {
        this->input >> token;

        if ("{" != token) {
            texture_coord.name = token;
            this->input >> token;
        }
        else {
            texture_coord.name = "ANON" + (unsigned)(this->texture_coords.size());
        }
    }

    this->input >> texture_coord.data.x;
    this->input >> token;

    if (this->input.eof()) {
        success = false;
        this->errors.push_back("File terminated early");
    }
    else {
        this->input >> texture_coord.data.y;
        this->input >> token;
    }

    if (this->input.eof()) {
        success = false;
        this->errors.push_back("File terminated early");
    }

    return success;
}

bool WTD::parseTexCoordBlock() {
    bool success = true;
    WTDTexCoord texture_coord;

    success = this->getTexCoord(texture_coord, true);

    if (success) {
        this->texture_coords.push_back(texture_coord);
    }

    return success;
}

bool WTD::parseDefineBlock() {
    std::string token;
    bool success = true;

    #if FILE_PARSER_WTD_DEBUG
        std::cout << "\n===Definitions===\n\n";
    #endif

    while (!this->input.eof() && success) {
        this->input >> token;
        if ("point" == token) {
            success = this->parsePointBlock();
        }
        else if ("face" == token) {
            success = this->parseFaceBlock();
        }
        else if ("texcoord" == token) {
            success = this->parseTexCoordBlock();
        }
        else if ("};" == token) {
            break;
        }
    }

    if (this->input.eof()) {
        success = false;
    }

    return success;
};

void WTD::readFromFile(std::string path) {
    std::string token;
    bool subcase_success = true;

    this->input.open(path);

    if (!this->input) {
        return;
    }

    while (!this->input.eof()) {
        this->input >> token;

        // comments
        if ('#' == token[0]) {
            #if FILE_PARSER_WTD_DEBUG
                std::cout <<
                    "Comment: " <<
                    token <<
                    " ";
                std::getline(this->input, token);
                std::cout << token << "\n";
            #elif
                this->input.ignore(INT_MAX, '\n');
            #endif
            continue;
        }

        if ("define" == token) {
            this->input >> token; // remove following brace
            subcase_success = this->parseDefineBlock();
            if (!subcase_success) {
                break;
            }
        }
        else if ("set" == token) {
            this->input >> token;
            if ("spritesheet" == token) {
                this->input >> token >> token;

                if (';' == token[token.size() - 1]) {
                    this->spritesheet_path = token.erase(token.size() - 1, 1);
                }
                std::cout << "Spritesheet: " << this->spritesheet_path << '\n';
            }
            else if ("spritesize" == token) {
                this->input >> token >> this->sprite_size;
                std::cout << "Sprite size: " << this->sprite_size << "px\n";
                this->input >> token;
            }
            else if ("model" == token) {
                this->input >> token >> token;
                std::cout << "Model...\n";
            }
        }
        else {
            std::cout << "UNT: " << token << '\n';
        }
    }

    if (!this->input.eof()) {
        this->errors.push_back("Parser terminated early");
    }

    for (auto &msg : this->errors) {
        std::cerr << msg << '\n';
    }

    input.close();

    this->print();
}

void WTD::printPoint(const WTDPoint &point) {
    std::cout <<
        "[ " <<
            point.name <<
        " ] = ( " <<
            point.data.x << ", " <<
            point.data.y << ", " <<
            point.data.z <<
        " )\n";
}

void WTD::printFace(const WTDFace &face) {
    std::cout <<
        "[ " <<
            face.name <<
        " ] = {\n";

    for (const auto &point : face.data) {
        std::cout << "\t[ ~" << point->name << " ],\n";
    }

    std::cout << "}\n";
}

void WTD::printTexCoord(const WTDTexCoord &texture_coord) {
    std::cout <<
        "[ " <<
            texture_coord.name <<
        " ] = ( " <<
            texture_coord.data.x << ", " <<
            texture_coord.data.y <<
        " )\n";
}

void WTD::print() {
    std::cout << "\n=== POINTS ===\n\n";

    for (auto &point : this->points) {
        this->printPoint(point);
    }

    std::cout << "\n=== FACES ===\n\n";

    for (auto &face : this->faces) {
        this->printFace(face);
    }

    std::cout << "\n=== TEXTURE COORDS ===\n\n";

    for (auto &texture_coord : this->texture_coords) {
        this->printTexCoord(texture_coord);
    }
}

}
}
