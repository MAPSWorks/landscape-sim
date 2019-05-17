#include "app.h"
#include <util/log.h>
#include <util/types.h>

App::App() : platform_(t::Size(800, 600)),
             graphics_system_({ "Test app", 1, "W2GPU", 1 }, 
                                platform_.GetWindowHandle()) {
    util::Log::Info("App initialized");
}

App::~App() {
    util::Log::Info("App shutting down...");
}

void App::Run() {
    while (platform_.IsRunning()) {
        platform_.PollEvents();
        graphics_system_.RenderFrame();
    }
    graphics_system_.WaitForIdle();
}

