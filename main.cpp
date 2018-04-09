#include "gstart.hpp"

#include "File/Format/wtd.h"

int main(int argc, char* argv[]) {
    // return runGraphics(argc, argv);

    File::Format::WTD wtd;

    wtd.readFromFile("Data/WTD/cube.wtd");
}
