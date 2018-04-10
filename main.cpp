#include "gstart.hpp"

#include "File/Format/wtd.h"
#include "File/Format/wad.h"

int main(int argc, char* argv[]) {
    // return runGraphics(argc, argv);

    File::Format::WTD wtd;
    File::Format::WAD wad;

    wtd.readFromFile("Data/WTD/cube.wtd");

    wad.fromWTD(wtd);

    wad.writeToFile("Data/WAD/test.wad");
}
