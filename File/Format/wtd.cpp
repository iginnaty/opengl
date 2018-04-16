#include "wtd.h"
#include <iostream>

namespace File {
namespace Format {

std::string getHeader(std::ifstream &input, bool check_for_name, std::string anon_name) {
    std::string output;
    std::string token;

    if (check_for_name) {
        input >> token;
        if ("{" != token) {
            output = token;
            input >> token;
            return output;
        }
        else {
            return anon_name;
        }
    }
    else {
        output = anon_name;
        input >> token;
        return output;
    }
}

bool parsePointBlock(WTD *wtd, std::ifstream &input, bool check_for_name) {
    std::string token;
    WTDPoint point;

    point.name = getHeader(input, check_for_name, "ANON_P_" + std::to_string(wtd->getPointLib()->size()));

    input >> point.data.x;
        input >> token;
    input >> point.data.y;
        input >> token;
    input >> point.data.z;

    input >> token;

    if ('}' != token[0]) {
        return false;
    }

    wtd->getPointLib()->push_back(point);

    return true;
}

bool parseTexCoordBlock(WTD *wtd, std::ifstream &input, bool check_for_name) {
    bool success = true;
    std::string token;
    Graphics::VertexI2D texture_coord;
    WTDTexture texture;

    texture.name = getHeader(input, check_for_name, "ANON_T_" + std::to_string(wtd->getTexCoordLib()->size()));

    while (success) {
        input >> token; // {
        if ('}' == token[0]) {
            break;
        }

        input >> texture_coord.x;
            input >> token; // ,
        input >> texture_coord.y;
            input >> token; // },

        wtd->getTexCoordLib()->push_back(texture_coord);
        texture.data.push_back(wtd->getTexCoordLib()->size() - 1);

        success &= !input.eof();
    }

    if (!success) {
        return false;
    }

    wtd->getTextureLib()->push_back(texture);

    return true;
}

bool parseFaceBlock(WTD *wtd, std::ifstream &input, bool check_for_name) {
    bool success = true;
    std::string token;
    WTDPoly face;
    unsigned index;

    face.name = getHeader(input, check_for_name, "ANON_F_" + std::to_string(wtd->getFacesLib()->size()));

    while (success) {
        input >> token;

        if ("{" == token) {
            success = parsePointBlock(wtd, input, false);

            if (success) {
                face.data.push_back(wtd->getPointLib()->size() - 1);
            }
        }
        else if ('~' == token[0]) {
            index = token.size() - 1;
            if (',' == token[index]) {
                token = token.substr(1, index - 1);
            }
            else {
                token = token.substr(1, index);
            }

            success = false;
            index = 0;
            for (auto &point : *wtd->getPointLib()) {
                if (point.name == token) {
                    face.data.push_back(index);
                    success = true;
                    break;
                }
                ++index;
            }
            if (!success) {
                wtd->errors.push_back("Failed to parse vertex statement in face data");
            }
        }
        else if ('}' == token[0]) {
            break;
        }
        else {
            wtd->errors.push_back("Unknown block opening " + token);
            success = false;
        }

        success &= !input.eof();
    }

    if (!success) {
        return false;
    }

    wtd->getFacesLib()->push_back(face);

    return true;
}

bool parseDefineBlock(WTD *wtd, std::ifstream &input) {
    bool success = true;
    std::string token;

    input >> token;

    if ('{' != token[0]) {
        wtd->errors.push_back("Expected start of block, encountered " + token);
        return false;
    }

    while (success) {
        input >> token;

        if ("point" == token) {
            success = parsePointBlock(wtd, input, true);
        }
        else if ("texture" == token) {
            success = parseTexCoordBlock(wtd, input, true);
        }
        else if ("face" == token) {
            success = parseFaceBlock(wtd, input, true);
        }
        else if ('}' == token[0]) {
            break;
        }
        else {
            wtd->errors.push_back("Unknown definable type " + token);
            success = false;
        }

        success &= !input.eof();
    }

    return success;
}

bool parseModelBlock(WTD *wtd, std::ifstream &input) {
    bool success = true;
    char peek;
    std::string token;
    unsigned index;
    WTDFace poly;

    input >> token;

    if ('[' != token[0]) {
        wtd->errors.push_back("Expected start of array, encountered " + token);
        return false;
    }

    while (success) {
        input >> token; // {

        if (']' == token[0]) {
            break;
        }

        input >> peek;

        if ('~' == peek) {
            input >> token;
            token.erase(token.size() - 1, 1);

            success = false;
            index = 0;
            for (auto &face : *wtd->getFacesLib()) {
                if (face.name == token) {
                    success = true;
                    poly.face_index = index;
                    break;
                }
                ++index;
            }
            if (!success) {
                wtd->errors.push_back("Undefined face " + token);
            }
        }
        else if ('{' == peek) {
            input.unget();
            success = parseFaceBlock(wtd, input, false);

            if (success) {
                poly.face_index = wtd->getFacesLib()->size() - 1;
            }
        }
        else {
            success = false;
            wtd->errors.push_back("Failed to parse face statement in model data");
        }

        if (!success) {
            break;
        }

        input >> peek;
        if ('~' == peek) {
            input >> token;

            success = false;
            index = 0;
            for (auto &texture : *wtd->getTextureLib()) {
                if (texture.name == token) {
                    success = true;
                    poly.texture_index = index;
                    break;
                }
                ++index;
            }
            if (!success) {
                wtd->errors.push_back("Undefined texture coordinate " + token);
            }
        }
        else if ('{' == peek) {
            input.unget();
            success = parseTexCoordBlock(wtd, input, false);

            if (success) {
                poly.texture_index = wtd->getTexCoordLib()->size() - 1;
            }
        }
        else {
            success = false;
            wtd->errors.push_back("Failed to parse texture coordinate statement in model data");
        }

        if (!success) {
            break;
        }

        if (
            (*wtd->getFacesLib())[poly.face_index].data.size() !=
            (*wtd->getTextureLib())[poly.texture_index].data.size()
        ) {
            wtd->errors.push_back("Data mismatch: face vertex and texture coordinate counts different");
            success = false;
            break;
        }

        wtd->getModelData()->push_back(poly);

        input >> token; // },
    }

    if (!success) {
        return false;
    }

    input >> token; // ];

    return success;
}

bool WTD::readFromFile(std::string path) {
    std::ifstream input;
    std::string token, trash;
    bool success = true;

    this->errors.clear();

    input.open(path);

    if (!input) {
        errors.push_back("Failed to open " + path);
        return false;
    }

    while (success) {
        input >> token;

        if ("define" == token) {
            success = parseDefineBlock(this, input);
        }
        else if ("set" == token) {
            input >> token;
            input >> trash;

            if ("spritesheet" == token) {
                input >> token;
                token = token.erase(token.size() - 1, 1);

                this->spritesheet_path = token;
            }
            else if ("spritesize" == token) {
                input >> this->sprite_size;
                input >> trash;
            }
            else if ("modelname" == token) {
                input >> token;
                token = token.erase(token.size() - 1, 1);

                this->name = token;
            }
            else if ("model" == token) {
                success = parseModelBlock(this, input);
            }
            else {
                this->errors.push_back("Unknown metadata " + token);
                success = false;
            }
        }
        else {
            this->errors.push_back("Unknown directive " + token);
            success = false;
        }

        success &= !input.eof();
    }

    input.close();

    this->print();

    return success;
}

void linearSearchAndAdd(const unsigned ID, std::vector<unsigned> &index_list) {
    unsigned index, length;

    for (index = 0, length = index_list.size(); index < length; ++index) {
        if (ID == index_list[index]) {
            break;
        }
    }
    if (index == length) {
        index_list.push_back(ID);
    }
}

bool WTD::writeToFile(std::string path) {
    std::ofstream output;
    std::vector<unsigned> point_indeces, face_indeces, tex_indeces;
    unsigned index, length, subindex, sublength;

    WTDPoint   *point;
    WTDTexture *tex;
    WTDPoly    *face;
    WTDFace    *data;
    Graphics::VertexI2D *tex_coord;

    output.open(path);

    if (!output) {
        return false;
    }

    for (auto &face : this->model_data) {
        linearSearchAndAdd(face.texture_index, tex_indeces);

        linearSearchAndAdd(face.face_index, face_indeces);
    }

    for (auto &face : face_indeces) {
        for (auto &point : this->lib_faces[face].data) {
            linearSearchAndAdd(point, point_indeces);
        }
    }

    output << "define {\n";

    for (index = 0, length = point_indeces.size(); index < length; ++index) {
        point = &this->lib_points[point_indeces[index]];

        output <<
            "    point " << point->name << " {\n"
            "        " <<
                point->data.x << ", " <<
                point->data.y << ", " <<
                point->data.z << "\n"
            "    };\n";
    }

    output << "\n";

    for (index = 0, length = tex_indeces.size(); index < length; ++index) {
        tex = &this->lib_textures[tex_indeces[index]];

        output <<
            "    texture " << tex->name << " {\n";
        for (subindex = 0, sublength = tex->data.size() - 1; subindex < sublength; ++subindex) {
            tex_coord = &this->lib_texture_coords[tex->data[subindex]];
            output <<
                "        { " <<
                    tex_coord->x << ", " <<
                    tex_coord->y <<
                " },\n";
        }

        tex_coord = &this->lib_texture_coords[tex->data[subindex]];
        output <<
            "        { " <<
                tex_coord->x << ", " <<
                tex_coord->y <<
            " }\n"
            "    };\n\n";
    }

    for (index = 0, length = face_indeces.size(); index < length; ++index) {
        face = &this->lib_faces[face_indeces[index]];

        output << "    face " << face->name << " {\n        ";

        for (subindex = 0, sublength = face->data.size() - 1; subindex < sublength; ++subindex) {
            output << "~" << this->lib_points[face->data[subindex]].name << ", ";
        }
        output << "~" << this->lib_points[face->data[subindex]].name << "\n    };\n";
    }

    output <<
        "};\n\n"
        "set spritesheet = " << this->spritesheet_path << ";\n"
        "set spritesize = " << this->sprite_size << ";\n"
        "set modelname = " << this->name << ";\n"
        "set model = [\n";

    for (index = 0, length = this->model_data.size() - 1; index < length; ++index) {
        data = &this->model_data[index];

        output << "    { ~" <<
            this->lib_faces[data->face_index].name << ", ~" <<
            this->lib_textures[data->texture_index].name << " },\n";
    }
    data = &this->model_data[index];

    output << "    { ~" <<
        this->lib_faces[data->face_index].name << ", ~" <<
        this->lib_textures[data->texture_index].name << " }\n];\n";

    return true;
}

WAD WTD::toWAD() {
    WAD output;

    unsigned index, length;

    WTDPoly    *face;
    WTDTexture *tex;
    std::vector<WADVertexData> wad_face;
    WADVertexData vertex_data;

    for (auto &point : this->lib_points) {
        output.getVertices()->push_back(point.data);
    }
    for (auto &tex : this->lib_textures) {
        for (auto &tex_coord : tex.data) {
            output.getTexCoords()->push_back(this->lib_texture_coords[tex_coord]);
        }
    }
    for (WTDFace &poly : this->model_data) {
        wad_face.clear();

        face = &this->lib_faces[poly.face_index];
        for (index = 0, length = face->data.size(); index < length; ++index) {
            vertex_data.position_index = face->data[index];
            wad_face.push_back(vertex_data);
        }

        tex = &this->lib_textures[poly.texture_index];
        for (index = 0, length = tex->data.size(); index < length; ++index) {
            wad_face[index].texture_index = tex->data[index];
        }

        output.getFaces()->push_back(wad_face);
    }

    output.setName(this->name);
    output.setSpritesheet(this->spritesheet_path);
    output.setSpriteSize(this->sprite_size);

    return output;
}

void WTD::print() {
    unsigned index;

    std::cout <<
        "\n"
        "### WTD ###\n\n"
        "=== METADATA ===\n\n"
        "    Name: " << this->name << "\n"
        "    Spritesheet: " << this->spritesheet_path << "\n"
        "    Sprite size: " << this->sprite_size << "px\n"
        "\n"
        "=== POINT LIBRARY ===\n\n";

    index = 0;
    for (auto &point : this->lib_points) {
        std::cout <<
            "    >>> POINT [" << index << "] <<<\n\n"
            "        Name: " << point.name << "\n"
            "        X: " << point.data.x << "\n"
            "        Y: " << point.data.y << "\n"
            "        Z: " << point.data.z << "\n"
            "\n";
        ++index;
    }

    std::cout << "=== FACE LIBRARY ===\n\n";

    index = 0;
    for (auto &face : this->lib_faces) {
        std::cout <<
            "    >>> FACE [" << index << "] <<<\n\n"
            "        Name: " << face.name << "\n"
            "        Size: " << face.data.size() << "\n"
            "        Points: [\n";

        for (auto &point : face.data) {
            std::cout << "            " << this->lib_points[point].name << ",\n";
        }

        std::cout << "        ]\n\n";
        ++index;
    }

    std::cout << "=== TEXTURE COORDINATE LIBRARY ===\n\n";

    index = 0;
    for (auto &tex : this->lib_textures) {
        std::cout <<
            "    >>> TEXTURE [" << index << "] <<<\n\n"
            "        Name: " << tex.name << "\n"
            "        Size: " << tex.data.size() << "\n"
            "        Points: [\n";

        for (auto &tex_coord : tex.data) {
            std::cout <<
                "            { " <<
                    this->lib_texture_coords[tex_coord].x << ", " <<
                    this->lib_texture_coords[tex_coord].y <<
                " },\n";
        }

        std::cout << "        ]\n\n";
        ++index;
    }

    std::cout << "=== MODEL DATA ===\n\n";

    index = 0;
    for (auto &poly : this->model_data) {
        std::cout <<
            "    >>> POLY [" << index << "] <<<\n\n"
            "        Face:    " << this->lib_faces[poly.face_index].name << "\n"
            "        Texture: " << this->lib_textures[poly.texture_index].name << "\n"
            "\n";
        ++index;
    }

    if (this->errors.size()) {
        std::cout << "### ERRORS ###\n\n";
        for (auto &error : this->errors) {
            std::cout << error << '\n';
        }
        std::cout << '\n';
    }
}

}
}
