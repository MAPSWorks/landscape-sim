#include "platform.h"
#include <string>
#include <util/log.h>

namespace platform {
Platform::Platform(const t::Size& win_size) : window_size_(win_size) {
    Init();
    util::Log::Info("Platform initialized");
}

Platform::~Platform() {
    util::Log::Info("Platform shutting down...");
    Shutdown();
}

void Platform::Init() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    std::string window_title = "<App name>";
    window_ = glfwCreateWindow(window_size_.width, window_size_.height, window_title.c_str(), nullptr, nullptr);
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