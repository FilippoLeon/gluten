#pragma once

#include <stdexcept>

#include "gluten/debug/log.hpp"

namespace gluten::debug {

class RuntimeException : std::runtime_error {
public:
    RuntimeException(std::string msg) 
        : std::runtime_error(msg), msg(msg) {
        LogError(msg);
    }

    const char* what() const noexcept {
        return msg.c_str();
    }
private:
    std::string msg;
};

}