#include "spritesheet.h"

namespace Graphics {
namespace Texture {

SpriteSheet::SpriteSheet() : SpriteSheet(SPRITE_DEFAULT_SIZE, SPRITE_DEFAULT_SIZE) {
}

SpriteSheet::SpriteSheet(GLuint sprite_width, GLuint sprite_height) : Basic() {
    this->sprite_width = sprite_width;
    this->sprite_height = sprite_height;
}

}
}
