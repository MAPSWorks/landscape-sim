#include "i_application.h"
#include <base/log.h>

namespace platform {
IApplication::IApplication(uint32_t argc, char* argv[]) :
    cmd_line_parser_(argc, argv),
    settings_loader_("ini/settings.json") {
    Init(t::Size(settings_loader_.Get().at("windowSize").at(0).get<uint32_t>(),
        settings_loader_.Get().at("windowSize").at(1).get<uint32_t>()),
        settings_loader_.Get().at("renderer").at("appName").get<std::string>());
    int width, height;
    glfwGetFramebufferSize(window_, &width, &height);
    base::Log::Info("Platform initialized. Window size: ",width, " ", height);
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

// static
void IApplication::ResizeCallback(GLFWwindow* window, int width, int height) {
    // Retrieve current window pointer to acess member variables
    IApplication* this_obj = static_cast<IApplication*>(glfwGetWindowUserPointer(window));
    this_obj->Resize(t::Size(width, height));
}

void IApplication::Init(const t::Size& win_size, std::string_view title) {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    window_ = glfwCreateWindow(win_size.width, win_size.height, title.data(), nullptr, nullptr);
    // Set window pointer to use current renderable in callback function
    glfwSetWindowUserPointer(window_, this);
    // Callbacks
    glfwSetFramebufferSizeCallback(window_, ResizeCallback);
}

void IApplication::Shutdown() {
    glfwDestroyWindow(window_);
    glfwTerminate();
}
}; // platform