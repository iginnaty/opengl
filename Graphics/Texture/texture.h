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

        GLuint getWidth() {
            return this->image_width;
        }

        GLuint getHeight() {
            return this->image_height;
        }

        GLuint getID() {
            return this->ID;
        }

        float getWidthRatio() {
            return (float)this->image_width / (float)this->width;
        }

        float getHeightRatio() {
            return (float)this->image_height / (float)this->height;
        }

        bool isLoaded() {
            return (bool)this->ID;
        }
    };

}
}
