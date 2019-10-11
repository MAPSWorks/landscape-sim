#pragma once
#include <string>
#include <GLFW/glfw3.h>
#include <base/types.h>
#include <base/cmd_line_parser.h>
#include <base/json_loader.h>
#include "input.h"

// Base class for application
// Handles platform specifix tasks and provides template for app
namespace platform {
class IApplication {
public:
    // Parameters are the same as from main() arguments
    IApplication(t::U32 argc, char* argv[]);
    virtual ~IApplication();
    IApplication(IApplication const&) = delete;
    // Copy-assignment is not allowed doe to pure virtual functions
    void Run();
protected:
    // File where application settings are stored
    const std::string setting_file_;
    // Folder where scene files are stored
    const std::string scene_folder_;
    // Get the currently supplied scene file name
    std::string GetSceneFileName() const;
    // Data from command line that provides contents of the scene
    const base::CmdLineParser cmd_line_parser_;
    // Application and subsystem settings
    const base::JSONLoader settings_loader_;
    // Window handle, later used to retrieve window related data like size
    GLFWwindow* window_ = nullptr;
private:
    static void ResizeCallback(GLFWwindow* window, int width, int height);
    static void KeyCallback(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods);
    virtual void Update() = 0;
    virtual void RenderFrame() = 0;
    virtual void Resize(const t::Size32& win_size) = 0;
    virtual void KeyPress(const Input& input) = 0;
    // To handle a moment just before exiting application
    virtual void OnExit() const = 0;
    void Init(const t::Size32& win_size, std::string_view title);
    void Shutdown();
    // Key and mouse input state tracker
    Input input_;
};

}; // platform