#pragma once

#include <fstream>
#include <string>

#include "../Format/wtd.h"
#include "../Format/wad.h"

namespace File {
namespace Parser {

class WhatTextData {
private:
    std::ifstream input;
    void error(std::string message);
    void getPoint(File::Format::WTDPoint &point);
    bool parsePointBlock(Format::WTD &output);
    bool parseFaceBlock(Format::WTD &output);
    bool parseTexCoordBlock(Format::WTD &output);
    bool parseDefineBlock(Format::WTD &output);
public:
    Format::WTD readFromFile(std::string path);
    Format::WAD parseFile(std::string path);
};

typedef WhatTextData WTD;

}
}
