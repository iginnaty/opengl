#include "wad.h"
#include <fstream>
#include <iostream>

namespace File {
namespace Format {

bool WAD::writeToFile(std::string path) {
    std::ofstream output;

    output.open(path, std::ios::out | std::ios::binary);

    std::size_t name_size = this->name.size();

    output.write((char *)&name_size, sizeof(name_size));
    output.write(this->name.c_str(), name_size);

    output.close();

    return true;
}

bool WAD::fromWTD(WTD &input) {
    std::vector<WTDModelData> *wtd_data = nullptr;
    std::vector<WADVertexData> active_face;
    WADVertexData *active_ao_entry;

    Graphics::Vertex    *active_vertex   = nullptr;
    Graphics::VertexI2D *active_vertex2d = nullptr;
    unsigned index;

    if (!input.isLoaded()) {
        return false;
    }

    wtd_data = input.getData();

    std::cout << "Start\n";

    // for each piece of the model
    for (auto &model_piece : *wtd_data) {

        // for every vertex in the piece's face
        for (auto &vertex : model_piece.face->data) {
            active_vertex = nullptr;
            // check if we have the vertex saved
            index = 0;
            for (auto &saved_vertex : this->vertices) {
                if (vertex->data == saved_vertex) {
                    active_vertex = &vertex->data;
                    break;
                }
                ++index;
            }

            if (nullptr == active_vertex) {
                this->vertices.push_back(vertex->data);
            }

            active_face.push_back(WADVertexData());
            active_ao_entry = &active_face.back();

            active_ao_entry->position = active_vertex;
            active_ao_entry->position_index = index;
        }

        for (auto &tex_coord : model_piece.texture->data) {
            active_vertex2d = nullptr;
            index = 0;
            for (auto &saved_vertex2d : this->texture_coords) {
                if (tex_coord->data == saved_vertex2d) {
                    active_vertex2d = &tex_coord->data;
                    break;
                }
                ++index;
            }

            if (nullptr == active_vertex2d) {
                this->texture_coords.push_back(tex_coord->data);
            }

            /*
                save texture coordinate data to appropriate entry
            */
        }

        std::cout << "[ ";
        for (auto &vertex : active_face) {
            std::cout << vertex.position_index << ", ";
        }
        std::cout << "]\n";

        active_face.clear();
    }
}

}
}
