#pragma once

#include <string>
#include "../INCLUDES.h"
#include "../../Util/Math/powertwo.hpp"

namespace Graphics {
namespace Texture {

    class Basic {
    protected:
        GLuint ID;
        GLuint width, height;
        GLuint image_width, image_height;

        void free();
    public:
        Basic();
        ~Basic();
        bool loadFromFile(std::string path);
        bool loadFromPixels(GLuint* pixels, GLuint image_width, GLuint image_height, GLuint width, GLuint height);
        GLuint getHeight();
        GLuint getWidth();
        GLuint getID();
        float getWidthRatio();
        float getHeightRatio();
    };

}
}
