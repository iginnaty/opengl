#include "gstart.hpp"

#include "File/Parser/wtd.h"

int main(int argc, char* argv[]) {
    // return runGraphics(argc, argv);

    File::Parser::WTD wtd_reader;

    wtd_reader.readFromFile("Data/WTD/cube.wtd");
}
