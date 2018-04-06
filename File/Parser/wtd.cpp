#include <iostream>
#include "wtd.h"

#define FILE_PARSER_WTD_DEBUG 1

namespace File {
namespace Parser {

#if FILE_PARSER_WTD_DEBUG
void printPoint(File::Format::WTDPoint &point) {
    std::cout <<
        "Point \"" <<
            point.name <<
        "\": " <<
        point.data.x << ", " <<
        point.data.y << ", " <<
        point.data.z << '\n';
}
#endif

void WTD::getPoint(File::Format::WTDPoint &point) {
    int index = 0;
    float *data = (float*)(&point.data);
    std::string token;

    while (!this->input.eof() && 3 > index) {
        this->input >> data[index];
        this->input >> token;
        ++index;
    }
}

void WTD::error(std::string message) {
    std::cerr <<
        "!Error: " <<
        message <<
        "\n";
}

bool WTD::parseFaceBlock(Format::WTD &output) {
    std::string token;
    bool success = true;
    File::Format::WTDFace face;
    std::vector<File::Format::WTDFace> *faces = output.getFaces();
    std::vector<File::Format::WTDPoint> *points = output.getPoints();
    File::Format::WTDPoint *point;

    this->input >> token;

    if ("{" != token) {
        face.name = token;
        this->input >> token;
    }

    #if FILE_PARSER_WTD_DEBUG
        std::cout <<
            "Face \"" <<
                face.name <<
            "\": \n";
    #endif

    this->input >> token;

    while (!this->input.eof() && "};" != token) {
        point = nullptr;

        if ('{' == token[0]) {
            point = new File::Format::WTDPoint();
            this->getPoint(*point);
            points->push_back(*point);

            point->name = "ANON";

            this->input >> token;
            if ('}' != token[0]) {
                success = false;
            }
        }
        else if ('~' == token[0]) {
            if (',' == token[token.size() - 1]) {
                token = token.substr(1, token.size() - 2);
            }
            else {
                token = token.erase(0, 1);
            }

            for (auto &point_p : *points) {
                if (point_p.name == token) {
                    point = &point_p;
                    break;
                }
            }

            this->input >> token;
        }

        if (nullptr == point) {
            success = false;
        }
        else {
            face.data.push_back(&point->data);

            #if FILE_PARSER_WTD_DEBUG
                std::cout << '\t';
                printPoint(*point);
            #endif
        }
    }

    #if FILE_PARSER_WTD_DEBUG
        std::cout <<
            "# of points: " <<
            face.data.size() <<
            '\n';
    #endif

    faces->push_back(face);

    return success;
}

bool WTD::parseTexCoordBlock(Format::WTD &output) {
    std::string token;
    bool success = true;
    File::Format::WTDTexCoord texture_coord;
    std::vector<File::Format::WTDTexCoord> *texture_coords = output.getTexCoords();

    this->input >> token;

    if ("{" != token) {
        texture_coord.name = token;
        this->input >> token;
    }

    this->input >> texture_coord.data.x;
    this->input >> token;

    if (this->input.eof()) {
        success = false;
    }
    else {
        this->input >> texture_coord.data.y;
        this->input >> token;
    }

    if (this->input.eof()) {
        success = false;
    }

    texture_coords->push_back(texture_coord);

    #if FILE_PARSER_WTD_DEBUG
        std::cout <<
            "Texture \"" <<
                texture_coord.name <<
            "\": " <<
                texture_coord.data.x << ", " <<
                texture_coord.data.y <<
            '\n';
    #endif

    return success;
}

bool WTD::parsePointBlock(Format::WTD &output) {
    std::string token;
    bool success = true;
    File::Format::WTDPoint point;
    std::vector<File::Format::WTDPoint> *points = output.getPoints();

    this->input >> token;

    if ("{" != token) {
        point.name = token;
        this->input >> token;
    }

    this->getPoint(point);

    if (this->input.eof()) {
        success = false;
    }

    #if FILE_PARSER_WTD_DEBUG
        printPoint(point);
    #endif

    points->push_back(point);

    return success;
}

bool WTD::parseDefineBlock(Format::WTD &output) {
    std::string token;
    bool success = true;

    #if FILE_PARSER_WTD_DEBUG
        std::cout << "\n===Definitions===\n\n";
    #endif

    while (!this->input.eof() && success) {
        this->input >> token;
        if ("point" == token) {
            success = this->parsePointBlock(output);
        }
        else if ("face" == token) {
            success = this->parseFaceBlock(output);
        }
        else if ("texcoord" == token) {
            success = this->parseTexCoordBlock(output);
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

Format::WTD WTD::readFromFile(std::string path) {
    Format::WTD output;
    std::string token;
    bool subcase_success = true;

    this->input.open(path);

    if (!this->input) {
        return output;
    }

    while (!this->input.eof()) {
        this->input >> token;

        std::cout << '[' << token << "]\n";

        // comments
        if ('#' == token[0]) {
            #if FILE_PARSER_WTD_DEBUG
                std::cout <<
                    "Comment: " <<
                    token <<
                    " ";
                std::getline(this->input, token);
                std::cout << token << "\n";
                continue;
            #elif
                this->input.ignore(INT_MAX, '\n');
            #endif
        }

        if ("define" == token) {
            this->input >> token; // remove following brace
            subcase_success = this->parseDefineBlock(output);
            if (!subcase_success) {
                break;
            }
        }
        else if ("set" == token) {
            this->input >> token;
            if ("spritesheet" == token) {
                this->input >> token >> token;
                std::cout << "Spritesheet: " << token << '\n';
            }
            else if ("spritesize" == token) {
                this->input >> token >> token;
                std::cout << "Sprite size: " << token << "px\n";
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
        this->error("Parser terminated early");
    }

    input.close();

    return output;
}

Format::WAD WTD::parseFile(std::string path) {
    Format::WAD output;
    return output;
}

}
}
