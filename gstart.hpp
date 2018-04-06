#pragma once

#include <IL/il.h>
#include <IL/ilu.h>

#include "Graphics/INCLUDES.h"
#include "Graphics/WINDOW.h"

#include "Graphics/Shapes/cube.h"
#include "Graphics/Texture/spritesheet.h"

Graphics::Shapes::Cube *cube;
Graphics::Texture::SpriteSheet *checkers;

void update() {
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    cube->render();

    glutSwapBuffers();
}

bool initGL() {
    GLenum error = glewInit();

    if (GLEW_OK != error) {
        std::cerr <<
            "Failed to initialize GLEW: " <<
            glewGetErrorString(error) <<
            "\n";
        return false;
    }

    if (!GLEW_VERSION_2_1) {
        std::cerr << "OpenGL 2.1 not supported\n";
        return false;
    }

    glViewport(0.0f, 0.0f, Window::SCREEN_WIDTH, Window::SCREEN_HEIGHT);

    // Projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, Window::SCREEN_WIDTH, Window::SCREEN_HEIGHT, 0.0f, 256.0f, 1.0f);
    // gluPerspective(30.0f, 1.5f, 1.0f, 256.0f);

    // Modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Clear
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Render options
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    error = glGetError();
    if (GL_NO_ERROR != error) {
        std::cerr <<
            "Failed to initialize OpenGL: " <<
            gluErrorString(error) <<
            "\n";
        return false;
    }

    ilInit();
    iluInit();
    ilClearColor(255, 255, 255, 0);

    ILenum il_error = ilGetError();
    if (IL_NO_ERROR != il_error) {
        std::cerr <<
            "Failed to initialize DevIL: " <<
            iluErrorString(il_error) <<
            "\n";
        return false;
    }

    return true;
}

bool loadMedia() {
    checkers = new Graphics::Texture::SpriteSheet();
    if (!checkers->loadFromFile("textures/cube.png")) {
        std::cerr << "Unable to load OpenGL textures\n";
        return false;
    }

    cube = new Graphics::Shapes::Cube(64);
    cube->setTextures(checkers);
    return true;
}

void mainLoop(int value) {
    update();
    render();
    glutTimerFunc(1000 / Window::SCREEN_FPS, mainLoop, 0);
}

int runGraphics(int &argc, char *argv[]) {

    // Initialize and create context
    glutInit(&argc, argv);
    glutInitContextVersion(2, 1);

    // Load window
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(Window::SCREEN_WIDTH, Window::SCREEN_HEIGHT);
    glutCreateWindow(Window::NAME);

    if (!initGL()) {
        std::cerr << "Unable to load OpenGL\n";
        return 1;
    }

    if (!loadMedia()) {
        std::cerr << "Unable to load media\n";
        return 2;
    }

    // Enter main loop
    glutDisplayFunc(render);
    glutTimerFunc(1000 / Window::SCREEN_FPS, mainLoop, 0);
    glutMainLoop();

    return 0;
}
