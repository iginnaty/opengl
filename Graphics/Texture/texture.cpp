#include "texture.h"

namespace Graphics {
namespace Texture {
    Texture::Texture() {
        this->ID     = 0;
        this->width  = 0;
        this->height = 0;
    }

    Texture::~Texture() {
        this->free();
    }

    bool Texture::loadFromPixels32(GLuint *pixels, GLuint width, GLuint height) {
        this->free();

        this->width  = width;
        this->height = height;

        glGenTextures(1, &(this->ID));
        glBindTexture(GL_TEXTURE_2D, this->ID);

        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RGBA,
            width, height, 0,
            GL_RGBA, GL_UNSIGNED_BYTE,
            pixels
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, (GLuint)NULL);

        GLenum error = glGetError();

        if (GL_NO_ERROR != error) {
            std::cout <<
                "Error loading texture: " <<
                gluErrorString(error) <<
                "\n";
            return false;
        }

        return true;
    }

    bool Texture::loadFromFile(std::string path) {
        bool loaded = false;

        ILuint imageID = 0;

        ilGenImages(1, &imageID);
        ilBindImage(imageID);

        ILboolean success = ilLoadImage(path.c_str());

        if (IL_TRUE == success) {
            success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

            if (IL_TRUE == success) {
                loaded = this->loadFromPixels32((GLuint*)ilGetData(), (GLuint)ilGetInteger(IL_IMAGE_WIDTH), (GLuint)ilGetInteger(IL_IMAGE_HEIGHT));
            }

            ilDeleteImages(1, &imageID);
        }

        if (!loaded) {
            std::cout <<
                "Unable to load texture at " <<
                path <<
                "\n";
        }

        return loaded;
    }

    void Texture::free() {
        if (0 != this->ID) {
            glDeleteTextures(1, &(this->ID));
            this->ID = 0;
        }

        this->width  = 0;
        this->height = 0;
    }

    void Texture::render(GLfloat x = 0.0f, GLfloat y = 0.0f) {
        if (0 == this->ID) {
            return;
        }

        glTranslatef(x, y, 0.0f);

        glBindTexture(GL_TEXTURE_2D, this->ID);

        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f,        0.0f);
            glTexCoord2f(1.0f, 0.0f); glVertex2f(this->width, 0.0f);
            glTexCoord2f(1.0f, 1.0f); glVertex2f(this->width, this->height);
            glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f,        this->height);
        glEnd();

        glLoadIdentity();
    }

    inline GLuint Texture::getID() {
        return this->ID;
    }

    inline GLuint Texture::getWidth() {
        return this->width;
    }

    inline GLuint Texture::getHeight() {
        return this->height;
    }
}
}
