#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <fmt/format.h>

namespace gluten::debug {

enum class LogLevel {
    verbose,
    debug,
    standard,
    warning,
    error,
    fatal,
};

#ifndef  LOG_LEVEL
#define LOG_LEVEL LogLevel::error
#endif // ! LOG_LEVEL

const std::vector<std::string> PRESTRING = {
    "[ VERBOSE ] ",
    "[ DEBUG   ] ",
    "[ INFO    ] ",
    "[ WARNING ] ",
    "[ ERROR   ] ",
    "[ FATAL   ] "
};

template <LogLevel ll, class ...Args>
inline void Log(std::string format_str, Args&&... args) {
#if USE_FMT
    if constexpr(LOG_LEVEL >= ll) {
        std::cout << fmt::format(PRESTRING[(int) ll] + format_str + "\n");
    }
#else 
    std::cout << PRESTRING[(int) ll] + format_str + "\n";
#endif
}

template <class ...Args>
inline void LogError(std::string format_str, Args&&... args) {
    Log<LogLevel::error>(format_str, std::forward<Args>(args)...);
}

template <class ...Args>
inline void LogWarning(std::string format_str, Args&&... args) {
#if USE_FMT
    if constexpr(LOG_LEVEL >= LogLevel::warning) {
        std::cout << fmt::format(PRESTRING[(int) LogLevel::warning] + format_str + "\n");
    }
#else 
    std::cout << PRESTRING[(int) LogLevel::warning] + format_str + "\n";
#endif
}


}