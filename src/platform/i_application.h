#pragma once
#include <GLFW/glfw3.h>
#include <base/types.h>

// Base class for application
// Handles platform specifix tasks and provides template for app
namespace platform {
class IApplication {
public:
    // TODO  make settings structure
    IApplication(t::Size win_size);
    virtual ~IApplication();
    IApplication(IApplication const&) = delete;
    void Run();
    GLFWwindow* GetWindowHandle() const;
private:
    static void ResizeCallback(GLFWwindow* window, int width, int height);
    virtual void Update() = 0;
    virtual void RenderFrame() = 0;
    virtual void Resize(const t::Size& win_size) = 0;
    // To handle a moment just before exiting application
    virtual void OnExit() const = 0;
    void Init(const t::Size& win_size);
    void Shutdown();
    GLFWwindow* window_ = nullptr;
};

}; // platform