#include "texture_library.h"

namespace Graphics {
namespace Texture {
    Library::Library() {
    }

    void Library::setDefaultTexture(Texture default_texture) {
        this->default_texture = default_texture;
    }

    int Library::addTexture(Texture new_texture) {
        this->data.push_back(new_texture);

        return this->data.size() - 1;
    }

    Texture& Library::getTexture(int position) {
        if (0 > position || (int)this->data.size() > position) {
            return this->default_texture;
        }

        return this->data[position];
    }
}
}
