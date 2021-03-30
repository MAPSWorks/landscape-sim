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
    kFile // TODO(ivars): not implemented
  };
  // Severity of logged information
  enum class Severity {
    // Info level shows errors too
    kInfo = 0,
    kError = 1
  };

  // Logging
  template <typename... Args> inline static void Info(Args &&... args) {
    if constexpr (kEnabled && kOutput == Type::kClog && kDetail == Severity::kInfo) {
      // Linux terminal coloring
#if defined(__linux__)
      std::clog << "\033[33mINFO: \033[0m";
#else
      std::clog << "INFO: ";
#endif
      // A trailing space is added between each pair of arguments
      ((std::clog << std::forward<Args>(args) << ' '), ...) << "\n";
    }
  }
  template <typename... Args> inline static void Error(Args &&... args) {
    if constexpr (kEnabled && kOutput == Type::kClog && kDetail <= Severity::kError) {
      // Linux terminal coloring
#if defined(__linux__)
      std::cerr << "\033[31mERROR: \033[0m";
#else
      std::cerr << "ERROR: ";
#endif

      // A trailing space is added between each pair of arguments
      ((std::cerr << std::forward<Args>(args) << ' '), ...) << "\n";
    }
  }

  // When disabled nothing is written to log
#ifdef _DEBUG
  static constexpr bool kEnabled = true;
#else
  static constexpr bool kEnabled = false;
#endif
  // Setting below matter only when logging is enabled
  // Where to write log
  static constexpr Type kOutput = Type::kClog;
  // Show logs by severity
  static constexpr Severity kDetail = Severity::kInfo;
};
} // namespace lsim::base

#endif