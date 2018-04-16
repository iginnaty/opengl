#include "wad.h"
#include <fstream>
#include <iostream>

namespace File {
namespace Format {

struct IndexPair {
    unsigned
        from,
        to;
};

std::string readHeadedLengthString(std::ifstream &input) {
    size_t length;
    char *buffer;
    std::string output;

    input.read((char *)&length, sizeof(length));
    buffer = new char[length + 1];
    input.read(buffer, length);
    buffer[length] = 0;

    output = std::string(buffer);
    delete [] buffer;

    return output;
}

template<typename bintype>
std::vector<bintype> readHeadedLengthVector(std::ifstream &input, size_t data_length = sizeof(bintype)) {
    size_t length;
    unsigned index;
    bintype *buffer;
    std::vector<bintype> data_vector;

    input.read((char *)&length, sizeof(length));
    buffer = new bintype{0};

    for (index = 0; index < length; ++index) {
        input.read((char *)buffer, data_length);
        data_vector.push_back(*buffer);
    }

    delete buffer;

    return data_vector;
}

bool WAD::readFromFile(std::string path) {
    std::ifstream input;
    size_t length;
    unsigned index;
    std::vector<WADVertexData> face;

    input.open(path, std::ios::in | std::ios::binary);

    if (!input) {
        return false;
    }

    this->name = readHeadedLengthString(input);

    this->vertices = readHeadedLengthVector<Graphics::Vertex>(input);
    this->texture_coords = readHeadedLengthVector<Graphics::VertexI2D>(input);

    input.read((char *)&length, sizeof(length));

    for (index = 0; index < length; ++index) {
        face = readHeadedLengthVector<WADVertexData>(input, sizeof(WADVertexData().position_index) + sizeof(WADVertexData().texture_index));

        this->faces.push_back(face);
    }

    this->spritesheet_path = readHeadedLengthString(input);
    input.read((char *)&this->sprite_size, sizeof(this->sprite_size));

    input.close();

    return true;
}

void writeHeadedLengthString(std::ofstream &output, std::string data) {
    size_t length = data.size();
    output.write((char *)&length, sizeof(length));
    output.write(data.c_str(), length);
}

template<typename bintype>
void writeHeadedLengthVector(std::ofstream &output, std::vector<bintype> &data_vector, size_t data_length = sizeof(bintype)) {
    size_t length = data_vector.size();
    unsigned index;

    output.write((char *)&length, sizeof(length));
    for (index = 0; index < length; ++index) {
        output.write((char *)&data_vector[index], data_length);
    }
}

bool WAD::writeToFile(std::string path) {
    std::ofstream output;
    unsigned index;
    size_t length;

    output.open(path, std::ios::out | std::ios::binary);

    if (!output) {
        return false;
    }

    this->compress();

    // name (size prepend not null terminated)
    writeHeadedLengthString(output, this->name);

    // vertex dictionary
    writeHeadedLengthVector(output, this->vertices);

    // texture coordinate dictionary
    writeHeadedLengthVector(output, this->texture_coords);

    // model data
    length = this->faces.size();
    output.write((char *)&length, sizeof(length));
    for (index = 0; index < length; ++index) {

        // face
        writeHeadedLengthVector(output, this->faces[index], sizeof(WADVertexData().position_index) + sizeof(WADVertexData().texture_index));
    }

    // spritesheet path
    writeHeadedLengthString(output, this->spritesheet_path);

    // sprite size
    output.write((char *)&this->sprite_size, sizeof(this->sprite_size));

    output.close();

    return true;
}

void WAD::clear() {
    this->name = "";
    this->spritesheet_path = "";
    this->sprite_size = 0;

    this->vertices.clear();
    this->texture_coords.clear();
    this->faces.clear();
}

int compress_searchMapAndAdd(const unsigned from, std::vector<IndexPair> &index_map) {
    IndexPair relation;
    unsigned index, length;

    for (index = 0, length = index_map.size(); index < length; ++index) {
        if (from == index_map[index].from) {
            return index_map[index].to;
        }
    }

    relation.from = from;
    index_map.push_back(relation);

    return -1;
}

template<typename VecType>
unsigned compress_createMap(
    VecType &new_vector, const VecType &old_vector,
    unsigned search_index, std::vector<IndexPair> &index_map
) {
    unsigned index, length;
    int existing_map;

    IndexPair *relation = nullptr;

    existing_map = compress_searchMapAndAdd(search_index, index_map);
    if (-1 == existing_map) {
        relation = &index_map.back();

        for (index = 0, length = new_vector.size(); index < length; ++index) {
            if (new_vector[index] == old_vector[search_index]) {
                existing_map = index;
                break;
            }
        }

        if (-1 == existing_map) {
            relation->to = new_vector.size();
            new_vector.push_back(old_vector[search_index]);
        }
        else {
            relation->to = existing_map;
        }

        return relation->to;
    }
    else {
        return existing_map;
    }

}

void WAD::compress() {
    std::vector<Graphics::Vertex>    cmp_vertices;
    std::vector<Graphics::VertexI2D> cmp_texture_coords;
    std::vector<std::vector<WADVertexData> > cmp_faces;

    std::vector<IndexPair>
        vertex_map,
        texture_map;

    for (auto &face : this->faces) {
        for (WADVertexData &indeces : face) {
            indeces.position_index = compress_createMap(cmp_vertices, this->vertices, indeces.position_index, vertex_map);
            indeces.texture_index  = compress_createMap(cmp_texture_coords, this->texture_coords, indeces.texture_index, texture_map);
        }
        cmp_faces.push_back(face);
    }

    this->vertices = cmp_vertices;
    this->texture_coords = cmp_texture_coords;
    this->faces = cmp_faces;
}

#if DEBUG_WAD
    void WAD::loadDebugData() {
        Graphics::Vertex    *temp_vertex;
        Graphics::VertexI2D *temp_vertex2d;
        WADVertexData *temp_face;
        std::vector<WADVertexData> temp_faces;

        this->name = "DEBUG WAD";
        this->spritesheet_path = "debug_path.pth";
        this->sprite_size = 16;

        temp_vertex = new Graphics::Vertex{0};
        this->vertices.push_back(*temp_vertex);

        temp_vertex->x = 1;
        this->vertices.push_back(*temp_vertex);

        temp_vertex->y = 1;
        this->vertices.push_back(*temp_vertex);

        delete temp_vertex;

        temp_vertex2d = new Graphics::VertexI2D();
        temp_vertex2d->x = 1;
        temp_vertex2d->y = 1;
        this->texture_coords.push_back(*temp_vertex2d);

        temp_vertex2d->x = 2;
        this->texture_coords.push_back(*temp_vertex2d);

        temp_vertex2d->y = 2;
        this->texture_coords.push_back(*temp_vertex2d);

        delete temp_vertex2d;

        temp_face = new WADVertexData{0};
        temp_faces.push_back(*temp_face);

        temp_face->position_index = 1;
        temp_face->texture_index = 1;
        temp_faces.push_back(*temp_face);

        temp_face->position_index = 2;
        temp_face->texture_index = 2;
        temp_faces.push_back(*temp_face);

        delete temp_face;
        this->faces.push_back(temp_faces);
    }

    void WAD::print() {
        unsigned index, length, sub_index, sub_length;
        Graphics::Vertex *vertex;
        Graphics::VertexI2D *tex_coord;
        std::vector<WADVertexData> *face;
        WADVertexData *vertex_data;

        std::cout <<
            "\n=== WAD ===\n\n"
            "Name: " <<
                this->name <<
            "\nSpritesheet: " <<
                this->spritesheet_path <<
            "\nSprite size: " <<
                this->sprite_size <<
            "\n";

        std::cout << "\n>>> Vertices <<<\n\n";
        for (index = 0, length = this->vertices.size(); index < length; ++index) {
            vertex = &this->vertices[index];

            std::cout <<
                "[" <<
                    index <<
                "] "
                "( " <<
                    vertex->x << ", " <<
                    vertex->y << ", " <<
                    vertex->z <<
                " )\n";
        }

        std::cout << "\n>>> Texture Coordinates <<<\n\n";
        for (index = 0, length = this->texture_coords.size(); index < length; ++index) {
            tex_coord = &this->texture_coords[index];

            std::cout <<
                "[" <<
                    index <<
                "] "
                "( " <<
                    tex_coord->x << ", " <<
                    tex_coord->y <<
                " )\n";
        }

        std::cout << "\n>>> Faces <<<\n\n";
        for (index = 0, length = this->faces.size(); index < length; ++index) {
            std::cout <<
                "~~~ Face [" <<
                    index <<
                "] ~~~\n";

            face = &this->faces[index];
            for (sub_index = 0, sub_length = face->size(); sub_index < sub_length; ++sub_index) {
                vertex_data = &(*face)[sub_index];

                std::cout <<
                    "\n--- Vertex [" <<
                        index << ":" <<
                        sub_index <<
                    "] ---\n\n"
                    "Position Index: " <<
                        vertex_data->position_index <<
                    "\n"
                    "Texture Index:  " <<
                        vertex_data->texture_index <<
                    "\n";
            }
        }

        std::cout << "\n\n";
    }
#endif

}
}
