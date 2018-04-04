#pragma once

#include "texture.h"

namespace Graphics {
namespace Texture {

const static GLuint SPRITE_DEFAULT_SIZE = 16;

class SpriteSheet : public Basic {
protected:
    GLuint sprite_width, sprite_height;
public:
    SpriteSheet();
    SpriteSheet(GLuint sprite_width, GLuint sprite_height);
    GLuint getHeight();
    GLuint getWidth();
    float getWidthRatio();
    float getHeightRatio();
};

}
}
