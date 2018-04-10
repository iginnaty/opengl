#include "gstart.hpp"

#include "File/Format/wtd.h"
#include "File/Format/wad.h"

#include <iostream>

int main(int argc, char* argv[]) {
    // return runGraphics(argc, argv);

    File::Format::WAD wad;

    wad.loadDebugData();
    wad.print();

    std::cout << "Writing WAD to file...\n";

    wad.writeToFile("Data/WAD/test.wad");

    std::cout << "Reading WAD from file...\n";

    wad.clear();
    wad.readFromFile("Data/WAD/test.wad");

    wad.print();
}
