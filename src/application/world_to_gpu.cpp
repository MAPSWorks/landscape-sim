#include "world_to_gpu.h"
#include <base/log.h>

namespace application {
WorldToGPU::WorldToGPU(t::U32 argc, char* argv[]) :
    platform::IApplication(argc, argv),
    renderer_(settings_loader_, window_),
    scene_(GetSceneFileName(), renderer_),
    scene_manager_(renderer_, scene_) {
    base::Log::Info("'World to GPU' initialized");
}

void WorldToGPU::Update(const platform::Input& input) {
    scene_manager_.Update(input);
}

void WorldToGPU::RenderFrame() {
    scene_manager_.RenderFrame();
}

void WorldToGPU::Resize(const t::Size32& win_size) {
    renderer_.Resize();
}

// Gets called right after exiting main loop and before exiting app
void WorldToGPU::OnExit() const {
    renderer_.WaitForGPUIdle();
}

void WorldToGPU::KeyPress(const platform::Input& input) {
    
}
}; // application