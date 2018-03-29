#include "Graphics/INCLUDES.h"
#include "Application/application.h"

int main(int argc, char* argv[]) {
    Application &app = Application::get();

    if (0 == app.init(argc, argv)) {
        app.run();
    }

    return 0;
}
