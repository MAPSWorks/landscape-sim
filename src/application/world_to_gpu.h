#pragma once
#include <platform/i_application.h>
#include <renderer/renderer.h>
#include <scene/scene_manager.h>
#include <gui/gui.h>

// Collects and executes subsystems that make up a complete application
namespace application {
class WorldToGPU : public platform::IApplication {
public:
    // Parameters from main() function arguments
    WorldToGPU(t::U32 argc, char* argv[]);
private:
    virtual void Update(const platform::Input& input) override;
    virtual void RenderFrame() override;
    virtual void Resize(const t::Size32& win_size) override;
    virtual void OnExit() const override;
    virtual void KeyPress(const platform::Input& input) override;
    virtual void MouseMove(const platform::Input& input) override;
    virtual void MouseButtonPress(const platform::Input& input) override;
    renderer::Renderer renderer_;
    scene::SceneManager scene_manager_;
    gui::GUI gui_;
};
}; // application