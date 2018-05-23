#pragma once

#include <string>
#include "../Graphics/vertexdata.hpp"

namespace Manager {
    template <typename Managed>
    struct ListEntry {
        bool        is_valid;
        std::string raw_name;
        unsigned    hashed_name;
        Managed     data;
    };
}
