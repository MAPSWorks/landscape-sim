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
  IApplication(int argc, char *argv[], const Settings &settings);
  virtual ~IApplication();
  IApplication(IApplication const &) = delete;
  // Copy-assignment is not allowed doe to pure virtual functions
  // Main loop of an Application
  void Run();

protected:
  // Applications and subsystem settings
  const Settings settings_;
  // Platform specific window
  Window window_;
  // Framework with which rendering is performed
  renderer::Renderer renderer_;

private:
  virtual void RenderFrame() const = 0;
  virtual void OnExit() const =0;
};
} // namespace lsim::platform

#endif