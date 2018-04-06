#include <IL/il.h>
#include <IL/ilu.h>
#include "texture.h"

namespace Graphics {
namespace Texture {

Basic::Basic() {
    this->ID     = 0;
    this->width  = 0;
    this->height = 0;

    this->image_width  = 0;
    this->image_height = 0;
}

Basic::~Basic() {
    this->free();
}

bool Basic::loadFromFile(std::string path) {
    bool loaded = false;

    ILuint ID = 0;
    ilGenImages(1, &ID);
    ilBindImage(ID);

    ILboolean success = ilLoadImage(path.c_str());
    if (IL_TRUE == success) {
        success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
        if (IL_TRUE == success) {
            GLuint
                image_width  = ilGetInteger(IL_IMAGE_WIDTH),
                image_height = ilGetInteger(IL_IMAGE_HEIGHT);

            GLuint
                width  = Util::Math::nextPowerOfTwo(image_width),
                height = Util::Math::nextPowerOfTwo(image_height);

            if (image_width != width || image_height != height) {
                iluImageParameter(ILU_PLACEMENT, ILU_UPPER_LEFT);
                iluEnlargeCanvas(width, height, 1);
            }

            loaded = this->loadFromPixels((GLuint*)ilGetData(), image_width, image_height, width, height);
        }

        ilDeleteImages(1, &ID);
    }

    if (!loaded) {
        std::cerr <<
            "unable to load texture: \"" <<
            path <<
            "\"\n";
    }

    return loaded;
}

bool Basic::loadFromPixels(GLuint *data, GLuint image_width, GLuint image_height, GLuint width, GLuint height) {
    this->free();

    this->width  = width;
    this->height = height;
    this->image_width  = image_width;
    this->image_height = image_height;

    glGenTextures(1, &(this->ID));
    glBindTexture(GL_TEXTURE_2D, this->ID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D, 0);

    GLenum error = glGetError();
    if (GL_NO_ERROR != error) {
        std::cerr <<
            "Error load texture from pixels: " <<
            gluErrorString(error) <<
            "\n";
        return false;
    }

    return true;
}

void Basic::free() {
    if (0 != this->ID) {
        glDeleteTextures(1, &(this->ID));
    }

    this->width  = 0;
    this->height = 0;
    this->image_width  = 0;
    this->image_height = 0;
}

GLuint Basic::getWidth() {
    return this->image_width;
}

GLuint Basic::getHeight() {
    return this->image_height;
}

GLuint Basic::getID() {
    return this->ID;
}

float Basic::getWidthRatio() {
    return (float)this->image_width / (float)this->width;
}

float Basic::getHeightRatio() {
    return (float)this->image_height / (float)this->height;
}

}
}
