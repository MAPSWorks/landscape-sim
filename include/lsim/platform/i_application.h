//
// Created by Ivars Rusbergs in 2021
//
// Applications using the engine are inherited from this class
#ifndef LSIM_PLATFORM_IAPPLICATION_H_
#define LSIM_PLATFORM_IAPPLICATION_H_

#include <lsim/renderer/renderer.h>

#include "types.h"
#include "window.h"

namespace lsim::platform {
// Abstract base class to be used as a parent class for apps.
// Handles application specific tasks and stores it's state.
class IApplication {
public:
  IApplication(int argc, char **argv, const Settings &settings);
  virtual ~IApplication();
  IApplication(const IApplication &) = delete;
  IApplication &operator=(const IApplication &) = delete;
  IApplication(IApplication &&) = delete;
  IApplication &operator=(IApplication &&) = delete;
  // Main loop of an Application
  void Run();

protected:
  [[nodiscard]] class Window &Window();
  [[nodiscard]] renderer::Renderer &Renderer();
  // Applications and subsystem settings
  // Member variable is const and seen as const global in
  // subclasses (NOLINTNEXTLINE)
  const Settings settings;

private:
  virtual void RenderFrame() = 0;
  virtual void OnExit() = 0;
  // Platform specific window
  class Window window_;
  // Framework with which rendering is performed
  renderer::Renderer renderer_;
};
} // namespace lsim::platform

#endif