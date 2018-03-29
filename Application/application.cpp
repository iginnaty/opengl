#include <IL/il.h>
#include <IL/ilu.h>

#include "../Graphics/INCLUDES.h"
#include "../Graphics/Texture/texture.h"
#include "application.h"

namespace {
    unsigned
        SCREEN_WIDTH  = 640,
        SCREEN_HEIGHT = 480,
        SCREEN_FPS    = 60;

    Graphics::Texture::Texture gCheckers;

    GLfloat gProjectionScale = 1.0f;
    float gCameraX = 0.0f, gCameraY = 0.0f;

    GLuint gVertexBuffer = 0;

    GLuint gIndexBuffer = 0;

    bool initGL() {
        GLenum glewError = glewInit();
        if( glewError != GLEW_OK ) {
            std::cout <<
                "Error initializing GLEW: " <<
                glewGetErrorString( glewError ) <<
                "\n";

            return false;
        }

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
                "Error initializing OpenGL:" <<
                gluErrorString(error) <<
                "\n";
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
        //gCameraX = 0.0f;
        //gCameraY = 0.0f;
    }

    void render() {
        glClear(GL_COLOR_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0.0f, SCREEN_WIDTH * gProjectionScale, SCREEN_HEIGHT * gProjectionScale, 0.0f, 1.0f, -1.0f);

        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();

        //gCheckers.render(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
        //glRotatef(-gCameraX, 0.0f, 0.0f, 0.0f);

        glEnableClientState(GL_VERTEX_ARRAY);

            glBindBuffer( GL_ARRAY_BUFFER, gVertexBuffer );
            glVertexPointer( 2, GL_FLOAT, 0, NULL );

            glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, gIndexBuffer );

            glDrawElements( GL_TRIANGLES, 16, GL_UNSIGNED_INT, NULL );

        glDisableClientState(GL_VERTEX_ARRAY);

        glLoadIdentity();
        glTranslatef(-gCameraX, -gCameraY, 0.0f);

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
        else if (27 == key) { // ESC
            Application::get().stop();
        }

        std::cout << key << "\n";
    }

    bool loadMedia() {
        /*
        if (!gCheckers.loadFromFile("checkers.png")) {
            std::cout << "Unable to load checkerboard from file\n";
            return false;
        }

        return true;
        */

        GLfloat vertices[] = {
            50.0f,  50.0f,
            100.0f, 50.0f,
            100.0f, 100.0f,
            50.0f,  100.0f,
            150.0f, 150.0f,
            200.0f, 150.0f,
            200.0f, 200.0f,
            150.0f, 200.0f
        };
        GLuint indices[] = {
            0, 1, 2,
            2, 3, 0,
            4, 5, 6,
            6, 7, 4
        };

        glGenBuffers( 1, &gVertexBuffer );
        glBindBuffer( GL_ARRAY_BUFFER, gVertexBuffer );
        glBufferData( GL_ARRAY_BUFFER, 16 * sizeof(GLfloat), vertices, GL_STATIC_DRAW );

        glGenBuffers( 1, &gIndexBuffer );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, gIndexBuffer );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, 12 * sizeof(GLuint), indices, GL_STATIC_DRAW );

        return true;
    }

    void runMainLoop(int val) {
        update();
        render();

        glutTimerFunc(1000 / SCREEN_FPS, runMainLoop, val);
    }
}

Application::Application() {
    this->status = Application::Status::NOT_RUNNING;
}

Application& Application::get() {
    static Application instance;
    return instance;
}

int Application::init(int argc, char *argv[]) {
    if (Status::ACTIVE == this->status) {
        return 1;
    }

    glutInit(&argc, argv);

    glutInitContextVersion(2, 1);

    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutCreateWindow("OpenGL");

    if (!initGL()) {
        std::cout << "Unable to initialize graphics library\n";
        return 2;
    }

    if (!loadMedia()) {
        std::cout << "Unable to load media\n";
        return 3;
    }

    //this->textures.setDefaultTexture(gCheckers);

    return 0;
}

void Application::run() {
    glutKeyboardFunc(handleKeys);
    glutDisplayFunc(render);
    glutTimerFunc(1000 / SCREEN_FPS, runMainLoop, 0);

    glutMainLoop();
}

void Application::stop() {
    glutLeaveMainLoop();
    std::cout << "Terminated by user\n";
}
