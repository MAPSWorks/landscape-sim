#pragma once
#include <platform/platform.h>
#include <renderer/renderer.h>
#include <scene/scene_manager.h>

namespace application {
class App {
public:
    App();
    ~App();
    void Run();
private:
    platform::Platform platform_;
    renderer::Renderer renderer_;
    scene::SceneManager scene_manager_;
};
}; //application