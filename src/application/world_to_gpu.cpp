#include "world_to_gpu.h"
#include <base/log.h>

namespace application {
WorldToGPU::WorldToGPU(uint32_t argc, char* argv[]) :
    platform::IApplication(argc, argv),
    renderer_(settings_loader_, window_),
    scene_("scenes/" + cmd_line_parser_.GetOption("-s")),
    scene_manager_(renderer_, scene_) {
    base::Log::Info("'World to GPU' initialized");
}

WorldToGPU::~WorldToGPU() {
    base::Log::Info("'World to GPU' shutting down...");
}

void WorldToGPU::Update() {
    scene_manager_.Update();
}

void WorldToGPU::RenderFrame() {
    scene_manager_.RenderFrame();
}

void WorldToGPU::Resize(const t::Size& win_size) {
    renderer_.Resize();
}

// Gets called right after exiting main loop and before exiting app
void WorldToGPU::OnExit() const {
    renderer_.WaitForGPUIdle();
}
}; // application