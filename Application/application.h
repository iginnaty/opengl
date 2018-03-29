#pragma once

#include "../Graphics/Texture/texture_library.h"

class Application {
private:
    enum class Status {
        NOT_RUNNING,
        ACTIVE
    };

    Status status;
    Graphics::Texture::Library textures;

    Application();
    Application(Application const &rhs) = delete; // DNI
    void operator = (Application const &rhs) = delete; // DNI
public:
    static Application& get();
    int init(int argc, char *argv[]);
    void run();
    void stop();
};
