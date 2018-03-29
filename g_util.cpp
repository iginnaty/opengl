#include <IL/il.h>
#include <IL/ilu.h>

#include "graphics.h"
#include "g_util.h"
#include "texture.h"

#define COLOR_MODE_CYAN 0
#define COLOR_MODE_MULTI 1

Graphics::Texture gCheckers;

int gColorMode = COLOR_MODE_CYAN;
GLfloat gProjectionScale = 1.0f;

float gCameraX = 0.0f, gCameraY = 0.0f;

bool initGL() {
    glViewport(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, SCREEN_WIDTH * gProjectionScale, SCREEN_HEIGHT * gProjectionScale, 0.0f, 1.0f, -1.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPushMatrix();

    glClearColor(0.3125f, 0.6836f, 0.8906f, 1.0f);
    glEnable(GL_TEXTURE_2D);

    GLenum error = glGetError();

    if (GL_NO_ERROR != error) {
        std::cout <<
            "Error initializing OpenGL:\n" <<
            gluErrorString(error);
        return false;
    }

    ilInit();
    ilClearColor(255, 255, 255, 0);

    ILenum ilError = ilGetError();
    if (IL_NO_ERROR != ilError) {
        std::cout <<
            "Unable to initialize DevIL: " <<
            iluErrorString(ilError) <<
            "\n";

        return false;
    }

    return true;
}

void update() {
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, SCREEN_WIDTH * gProjectionScale, SCREEN_HEIGHT * gProjectionScale, 0.0f, 1.0f, -1.0f);

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glTranslatef(-gCameraX, -gCameraY, 0.0f);
    gCheckers.render(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

    glPushMatrix();

    glutSwapBuffers();
}

void handleKeys(unsigned char key, int x, int y) {
    if ('e' == key) {
        gProjectionScale *= 2.0f;

        if (2.0f < gProjectionScale) {
            gProjectionScale = 0.5f;
        }
    }
    else if ('w' == key) {
        gCameraY -= 4.0f;
    }
    else if ('s' == key) {
        gCameraY += 4.0f;
    }
    else if ('a' == key) {
        gCameraX -= 4.0f;
    }
    else if ('d' == key) {
        gCameraX += 4.0f;
    }

    std::cout << key << "\n";
}

bool loadMedia() {
    if (!gCheckers.loadFromFile("checkers.png")) {
        std::cout << "Unable to load checkerboard from file\n";
        return false;
    }

    return true;
}
