#include "../Graphics/INCLUDES.h"
#include "../File/Format/wad.h"
#include "../Graphics/Texture/spritesheet.h"

namespace Graphics {

typedef File::Format::WAD WAD;
typedef File::Format::WAD_VAOPrecursor WAD_VAOPrecursor;

class Model {
private:
    bool loaded;
    GLuint vbo_id, ibo_id;
    Graphics::Texture::SpriteSheet *sprite_sheet;
    WAD data;
    WAD_VAOPrecursor vao_data;
public:
    Model();
    Model(Graphics::Texture::SpriteSheet *sprite_sheet, WAD &model);
    ~Model();
    void setTextures(Graphics::Texture::SpriteSheet *sprite_sheet);
    void initVBO();
    void freeVBO();
    void render();

    bool isLoaded() {
        return this->loaded;
    }

    void setModel(WAD &model) {
        this->data = model;
    }

    void setSpriteSheet(Graphics::Texture::SpriteSheet *sprite_sheet) {
        this->sprite_sheet = sprite_sheet;
    }
};

}
