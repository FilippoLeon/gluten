#pragma once

#include <string>

namespace gluten::osutils {

inline std::string GetExtension(std::string filename) {
    return filename.substr(filename.find_last_of(".") + 1);
}
     
}