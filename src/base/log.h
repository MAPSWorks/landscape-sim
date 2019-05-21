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

    // Write data to output 
    template<typename T>
    inline static void Info(T &&t) {
        if (enabled && output == kClog && detail == kInfo) {
            std::clog << t << "\n";
        }
    }
    template<typename T>
    inline static void Error(T &&t) {
        if (enabled && output == kClog && detail <= kError) {
            //HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            //SetConsoleTextAttribute(hConsole, 12);
            std::cerr << t << "\n";
        }
    }

    template<typename Head, typename... Tail>
    inline static void Info(Head &&head, Tail&&... tail) {
        if (enabled && output == kClog && detail == kInfo) {
            std::clog << head;
            Info(std::forward<Tail>(tail)...);
        }
    }
    template<typename Head, typename... Tail>
    inline static void Error(Head &&head, Tail&&... tail) {
        if (enabled && output == kClog && detail <= kError) {
            std::cerr << head;
            Error(std::forward<Tail>(tail)...);
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
