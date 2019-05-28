#pragma once
#include <platform/i_application.h>
#include <renderer/renderer.h>
#include <scene/scene_manager.h>

// Application for engine tests
namespace application {
class AlphaApp : public platform::IApplication {
public:
    AlphaApp();
    ~AlphaApp();
private:
    virtual void Update() override;
    virtual void RenderFrame() override;
    virtual void Resize(const t::Size& win_size) override;
    virtual void OnExit() const override;
    renderer::Renderer renderer_;
    scene::SceneManager scene_manager_;
};
}; //application