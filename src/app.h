#pragma once
#include <platform/platform.h>
#include <graphics/graphics_system.h>

class App {
public:
    App();
    ~App();
    void Run();
private:
    platform::Platform platform_;
    graphics::GraphicsSystem graphics_system_;
};