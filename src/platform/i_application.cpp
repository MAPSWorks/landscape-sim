#include "i_application.h"
#include <base/log.h>
#include <base/util.h>
#include <imgui/imgui.h>
#include <gui/imgui_impl_glfw.h>
#include <gui/imgui_impl_vulkan.h>

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
    base::Log::Info("Platform: initialized. Window size: (",width, " ", height,")");
}

IApplication::~IApplication() {
    base::Log::Info("Platform: shutting down...");
    Shutdown();
}

void IApplication::Run() {
    while (!glfwWindowShouldClose(window_)) {
        glfwPollEvents();
        Update(input_);
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

// static
void IApplication::KeyCallback(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods) {
    // Retrieve current window pointer to acess member variables
    IApplication* this_obj = static_cast<IApplication*>(glfwGetWindowUserPointer(window));
    // "Built in" key cheking
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    this_obj->input_.UpdateKeyData(static_cast<Input::Key>(key), static_cast<Input::Action>(action));
    this_obj->KeyPress(this_obj->input_); //virtual
}

// static
void IApplication::MouseCallback(GLFWwindow* window, double xpos, double ypos) {
    IApplication* this_obj = static_cast<IApplication*>(glfwGetWindowUserPointer(window));
    this_obj->input_.UpdateMouse((float)xpos, (float)ypos);
    this_obj->MouseMove(this_obj->input_); //virtual
}

// static
void IApplication::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    IApplication* this_obj = static_cast<IApplication*>(glfwGetWindowUserPointer(window));
    // Enable/disable mouse cursor
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        this_obj->input_.SetMouseCursorStatus(!this_obj->input_.GetMouseData().cursor_disabled);
        glfwSetInputMode(this_obj->window_, GLFW_CURSOR, (this_obj->input_.GetMouseData().cursor_disabled ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL));
        // Make sure camera dont jump when returning to camera controle mode
        if (this_obj->input_.GetMouseData().cursor_disabled) {
            this_obj->input_.SetMouseFirstMove(true);
        }
    }

    this_obj->input_.UpdateMouseButton(static_cast<Input::MouseButton>(button), static_cast<Input::Action>(action), mods);
    this_obj->MouseButtonPress(this_obj->input_); //virtual
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
    glfwSetKeyCallback(window_, KeyCallback);
    glfwSetCursorPosCallback(window_, MouseCallback);
    glfwSetMouseButtonCallback(window_, MouseButtonCallback);
    // Parameters
    glfwSetInputMode(window_, GLFW_CURSOR, (input_.GetMouseData().cursor_disabled ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL));
    // GUI
    bool use_gui = cmd_line_parser_.OptionExists("-g");
    if (use_gui) {
        base::Log::Info("Platform: GUI on");
    }
    else {
        base::Log::Info("Platform: GUI off");
    }
}

void IApplication::Shutdown() {
    glfwDestroyWindow(window_);
    glfwTerminate();
}
}; // platform