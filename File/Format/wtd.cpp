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

std::vector<WTDModelData> * WTD::getData() {
    return &this->data;
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

    while (!this->input.eof() && '}' != token[0] && success) {
        point = nullptr;

        if ('{' == token[0]) {
            point = new WTDPoint();

            point->name = "ANON" + (unsigned)(this->points.size());
            success = this->getPoint(*point, false);

            if (success) {
                this->points.push_back(*point);
                delete point;

                this->input >> token;
                if ('}' != token[0]) {
                    success = false;
                    this->errors.push_back("Expected '}' when reading ANONYMOUS point");
                }
            }
            else {
                break;
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
        this->errors.push_back("File terminated early");
        return false;
    }
    else {
        this->input >> texture_coord.data.y;
        this->input >> token;
    }

    if (this->input.eof()) {
        this->errors.push_back("File terminated early");
        return false;
    }

    return true;
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
}

bool WTD::parseModel() {
    std::string token;
    bool success = true;
    WTDFace *face = nullptr;
    WTDTexCoord *tex_coord = nullptr;
    WTDModelData *model;

    this->input >> token;

    if ("[" != token) {
        this->errors.push_back("Expected array when parsing model");
        success = false;
    }

    this->input >> token;

    while (!this->input.eof() && success) {
        this->input >> token;

        if ('~' == token[0]) {
            token = token.substr(1, token.size() - 2);

            for (auto &face_p : this->faces) {
                if (face_p.name == token) {
                    face = &face_p;
                    break;
                }
            }

            if (nullptr == face) {
                success = false;
                this->errors.push_back("Undefined reference to defined face \"" + token + "\"");
            }
        }
        else if ("{" == token) {
            face = new WTDFace();
            success = this->getFace(*face, false);

            if (success) {
                face->name = "ANON" + (unsigned)(this->faces.size());
                this->faces.push_back(*face);
                delete face;
            }
            else {
                break;
            }
        }
        else {
            success = false;
            break;
        }

        this->input >> token;

        if ('~' == token[0]) {
            token = token.substr(1, token.size() - 1);

            for (auto &tex_p : this->texture_coords) {
                if (tex_p.name == token) {
                    tex_coord = &tex_p;
                    break;
                }
            }

            if (nullptr == tex_coord) {
                success = false;
                this->errors.push_back("Undefined reference to defined texture coord \"" + token + "\"");
            }
        }
        else if ("{" == token) {
            tex_coord = new WTDTexCoord();
            success = this->getTexCoord(*tex_coord, false);

            if (success) {
                tex_coord->name = "ANON" + (unsigned)(this->texture_coords.size());
                this->texture_coords.push_back(*tex_coord);
            }
            else {
                break;
            }
        }

        if (nullptr != face && nullptr != tex_coord) {
            model = new WTDModelData();
            model->face = face;
            model->texture = tex_coord;

            this->data.push_back(*model);
            delete model;
        }

        this->input >> token >> token;
    }

    if (this->input.eof()) {
        this->errors.push_back("File terminated early");
        return false;
    }

    return success;
}

void WTD::readFromFile(std::string path) {
    std::string token;
    bool success = true;

    this->input.open(path);

    if (!this->input) {
        return;
    }

    while (!this->input.eof() && success) {
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
            success = this->parseDefineBlock();
            if (!success) {
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
                this->input >> token;
                success = this->parseModel();
            }
            else if ("modelname" == token) {
                this->input >> token >> token;
                if (';' == token[token.size() - 1]) {
                    this->name = token.erase(token.size() - 1, 1);
                }
                std::cout << "Model name: " << this->name;
            }
        }
        else {
            std::cout << "UNT: " << token << '\n';
        }
    }

    if (0 == this->errors.size()) {
        this->loaded = true;
    }

    if (!this->input.eof()) {
        this->errors.push_back("Unexpected data at end of file ignored");
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

    std::cout << "\n=== MODEL ===\n\n";

    for (auto &model : this->data) {
        std::cout << "Model entry:\n\tFace:    " <<
            model.face->name << "\n\tTexture: " <<
            model.texture->name <<
        "\n";
    }
}

}
}
