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
  IApplication(IApplication const &) = delete;
  IApplication &operator=(IApplication const &) = delete;
  IApplication(IApplication &&) = delete;
  IApplication &operator=(IApplication &&) = delete;
  // Main loop of an Application
  void Run();

protected:
  [[nodiscard]] const struct Settings &Settings() const;
  [[nodiscard]] const class Window &Window() const;
  [[nodiscard]] const renderer::Renderer &Renderer() const;

private:
  virtual void RenderFrame() const = 0;
  virtual void OnExit() const = 0;
  // Applications and subsystem settings
  const struct Settings settings_;
  // Platform specific window
  const class Window window_;
  // Framework with which rendering is performed
  const renderer::Renderer renderer_;
};
} // namespace lsim::platform

#endif