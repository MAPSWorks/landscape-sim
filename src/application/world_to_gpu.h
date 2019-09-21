#pragma once
#include <platform/i_application.h>
#include <renderer/renderer.h>
#include <scene/scene.h>
#include <scene/scene_manager.h>

// Collects and executes subsystems that make up a complete application
namespace application {
class WorldToGPU : public platform::IApplication {
public:
    // Parameters from main() function arguments
    WorldToGPU(t::U32 argc, char* argv[]);
private:
    virtual void Update() override;
    virtual void RenderFrame() override;
    virtual void Resize(const t::Size32& win_size) override;
    virtual void OnExit() const override;
    renderer::Renderer renderer_;
    scene::Scene scene_;
    scene::SceneManager scene_manager_;
};
}; // application