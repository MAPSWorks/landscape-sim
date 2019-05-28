#include "i_application.h"
#include <string>
#include <base/log.h>

namespace platform {
IApplication::IApplication(t::Size win_size) {
    Init(win_size);
    base::Log::Info("Platform initialized");
}

IApplication::~IApplication() {
    base::Log::Info("Platform shutting down...");
    Shutdown();
}

void IApplication::Run() {
    while (!glfwWindowShouldClose(window_)) {
        glfwPollEvents();
        Update();
        RenderFrame();
    }
    OnExit();
}

GLFWwindow* IApplication::GetWindowHandle() const {
    return window_;
}

// static
void IApplication::ResizeCallback(GLFWwindow* window, int width, int height) {
    // Retrieve current window pointer to acess member variables
    IApplication* this_obj = static_cast<IApplication*>(glfwGetWindowUserPointer(window));
    this_obj->Resize(t::Size(width, height));
}

void IApplication::Init(const t::Size& win_size) {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    std::string window_title = "<App name>";
    window_ = glfwCreateWindow(win_size.width, win_size.height, window_title.c_str(), nullptr, nullptr);
    // Set window pointer to use current object in callback function
    glfwSetWindowUserPointer(window_, this);
    // Callbacks
    glfwSetFramebufferSizeCallback(window_, ResizeCallback);
}

void IApplication::Shutdown() {
    glfwDestroyWindow(window_);
    glfwTerminate();
}
}; // platform