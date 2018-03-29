#pragma once

#include <string>
#include <IL/il.h>
#include "../INCLUDES.h"

namespace Graphics {
namespace Texture {
    class Texture {
    public:
        Texture();
        ~Texture();
        bool   loadFromPixels32(GLuint *pixels, GLuint width, GLuint height);
        bool   loadFromFile(std::string path);
        void   render(GLfloat x, GLfloat y);
        GLuint getID();
        GLuint getWidth();
        GLuint getHeight();
    protected:
        void free();

        GLuint ID,
               width,
               height;
    };
}
}
