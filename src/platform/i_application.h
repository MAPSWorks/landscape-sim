#pragma once
#include <string>
#include <GLFW/glfw3.h>
#include <base/types.h>
#include <base/cmd_line_parser.h>
#include <base/json_loader.h>

// Base class for application
// Handles platform specifix tasks and provides template for app
namespace platform {
class IApplication {
public:
    // Parameters are the same as from main() arguments
    IApplication(uint32_t argc, char* argv[]);
    virtual ~IApplication();
    IApplication(IApplication const&) = delete;
    // Copy-assignment is not allowed doe to pure virtual functions
    void Run();
protected:
    // Data from command line that provides contents of the scene
    const base::CmdLineParser cmd_line_parser_;
    // Application and subsystem settings
    const base::JSONLoader settings_loader_;
    // Window handle, later used to retrieve window related data like size
    GLFWwindow* window_ = nullptr;
private:
    static void ResizeCallback(GLFWwindow* window, int width, int height);
    virtual void Update() = 0;
    virtual void RenderFrame() = 0;
    virtual void Resize(const t::Size& win_size) = 0;
    // To handle a moment just before exiting application
    virtual void OnExit() const = 0;
    void Init(const t::Size& win_size, std::string_view title);
    void Shutdown();
  
};

}; // platform