#pragma once

#include <vector>
#include "texture.h"

namespace Graphics {
namespace Texture {
    class Library {
    private:
        Texture default_texture;
        std::vector<Texture> data;
    public:
        Library();
        void     setDefaultTexture(Texture default_texture);
        int      addTexture(Texture new_texture);
        Texture& getTexture(int position);
    };
}
}
