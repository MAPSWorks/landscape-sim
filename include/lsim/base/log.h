//
// Created by Ivars Rusbergs in 2021
//
// Application logging
#ifndef LSIM_BASE_LOG_H_
#define LSIM_BASE_LOG_H_

#include <iostream>

namespace lsim::base {
// Usage: 
//   Log::Info("caller module", "description", "additional")
class Log {
public:
  // Stream where the log is written
  enum class Type {
    kClog,
    kFile // TODO: not implemented
  };
  // Severity of logged information
  enum class Severity {
    // Info level shows errors too
    kInfo = 0,
    kError = 1
  };

  // Logging
  template <typename... Args> inline static void Info(Args &&... args) {
    if (enabled && output == Type::kClog && detail == Severity::kInfo) {
      // Linux terminal coloring
#if defined(__linux__)
      std::clog << "\033[33mINFO: \033[0m";
#endif
      // A trailing space is added between each pair of arguments
      ((std::clog << std::forward<Args>(args) << ' ') , ...) << "\n";
    }
  }
  template <typename... Args> inline static void Error(Args &&... args) {
    if (enabled && output == Type::kClog && detail <= Severity::kError) {
      // Linux terminal coloring
#if defined(__linux__)
      std::clog << "\033[31mERROR: \033[0m";
#endif
      // A trailing space is added between each pair of arguments
      ((std::cerr << std::forward<Args>(args) << ' ') , ...) << "\n";
    }
  }

  // When disabled nothing is written to log
#ifdef _DEBUG
  static constexpr bool enabled = true;
#else
  static constexpr bool enabled = false;
#endif
  // Setting below matter only when logging is enabled
  // Where to write log
  static constexpr Type output = Type::kClog;
  // Show logs by severity
  static constexpr Severity detail = Severity::kInfo;
};
} // namespace lsim::base

#endif