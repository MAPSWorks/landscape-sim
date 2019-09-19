#include "i_application.h"
#include <base/log.h>
#include <base/util.h>

namespace platform {
IApplication::IApplication(t::U32 argc, char* argv[]) :
    setting_file_("ini/settings.json"),
    scene_folder_("scenes/"),
    cmd_line_parser_(argc, argv),
    settings_loader_(setting_file_) {
    Init(t::Size32(settings_loader_.Get().at("windowSize").at(0).get<t::U32>(),
        settings_loader_.Get().at("windowSize").at(1).get<t::U32>()),
        settings_loader_.Get().at("renderer").at("appName").get<std::string>());
    int width, height;
    glfwGetFramebufferSize(window_, &width, &height);
    base::Log::Info("Platform initialized. Window size: (",width, " ", height,")");
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

// Scene file name is recieved through command line by app user
std::string IApplication::GetSceneFileName() const {
    return scene_folder_ + base::StripIlligallChars(cmd_line_parser_.GetOption("-s"));
}

// static
void IApplication::ResizeCallback(GLFWwindow* window, int width, int height) {
    // Retrieve current window pointer to acess member variables
    IApplication* this_obj = static_cast<IApplication*>(glfwGetWindowUserPointer(window));
    this_obj->Resize(t::Size32(width, height));
}

void IApplication::Init(const t::Size32& win_size, std::string_view title) {
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