#include "platform.h"
#include <string>
#include <base/log.h>

namespace platform {
Platform::Platform(const t::Size& win_size) {
    Init(win_size);
    base::Log::Info("Platform initialized");
}

Platform::~Platform() {
    base::Log::Info("Platform shutting down...");
    Shutdown();
}

void Platform::Init(const t::Size& win_size) {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    std::string window_title = "<App name>";
    window_ = glfwCreateWindow(win_size.width, win_size.height, window_title.c_str(), nullptr, nullptr);
}

void Platform::Shutdown() {
    glfwDestroyWindow(window_);
    glfwTerminate();
}

bool Platform::IsRunning() const {
    return !glfwWindowShouldClose(window_);
}

void Platform::PollEvents() const {
    glfwPollEvents();
}
GLFWwindow* Platform::GetWindowHandle() const {
    return window_; 
}
}; // platform