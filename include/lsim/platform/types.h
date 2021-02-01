//
// Created by Ivars Rusbergs in 2021
//
// Collection of commonly used platform types
#ifndef LSIM_PLATFORM_TYPES_H_
#define LSIM_PLATFORM_TYPES_H_
#include <string>

#include "lsim/base/types.h"
namespace lsim::platform {
// Application and engine user settings
struct Settings {
  // Application name
  const std::string name;
  // Application version
  const uint16_t version;
  // Initial window size, may change during resize
  // but this value will not be affected.const.
  // Dont rely on this variable in the middle of an app.
  const Size<uint32_t> initial_size;
};
} // namespace lsim::platform
#endif