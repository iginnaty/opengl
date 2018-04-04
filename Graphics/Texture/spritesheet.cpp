#include "spritesheet.h"

namespace Graphics {
namespace Texture {

SpriteSheet::SpriteSheet() : SpriteSheet(SPRITE_DEFAULT_SIZE, SPRITE_DEFAULT_SIZE) {
}

SpriteSheet::SpriteSheet(GLuint sprite_width, GLuint sprite_height) : Basic() {
    this->sprite_width = sprite_width;
    this->sprite_height = sprite_height;
}

GLuint SpriteSheet::getWidth() {
    return this->sprite_width;
}

GLuint SpriteSheet::getHeight() {
    return this->sprite_height;
}

float SpriteSheet::getWidthRatio() {
    return (float)this->sprite_width / (float)this->width;
}

float SpriteSheet::getHeightRatio() {
    return (float)this->sprite_height / (float)this->width;
}

}
}
