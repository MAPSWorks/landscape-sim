#pragma once
#include <iostream>
//#define WIN32_LEAN_AND_MEAN
//#include <windows.h>
// PIENTRY warning is probably when windows is included before glfw (or other way around)

namespace base {
// Logging class
class Log {
public:
    // Stream where the log is written
    enum Type {
        kClog,
        kFile // TODO: not implemented
    };
    // Severity of logged information
    enum Severity {
        // Info level shows errors too
        kInfo = 0,
        kError = 1
    };

    // Logging
    template<typename ...Args>
    inline static void Info(Args&& ... args) {
        if (enabled && output == kClog && detail == kInfo) {
            (std::clog << ... << std::forward<Args>(args)) << '\n';
        }
    }
    template<typename ...Args>
    inline static void Error(Args&& ... args) {
        if (enabled && output == kClog && detail <= kError) {
            (std::cerr << ... << std::forward<Args>(args)) << '\n';
        }
    }

    // When disabled nothing is written to log
#ifdef NDEBUG
    static constexpr bool enabled = false;
#else
    static constexpr bool enabled = true;
#endif
    // Setting below matter only when logging is enabled
    // Where to write log
    static constexpr Type output = kClog;
    // Show logs by severity
    static constexpr Severity detail = kInfo;
};
}; // util
