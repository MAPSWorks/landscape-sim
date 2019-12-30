#include "world_to_gpu.h"
#include <base/log.h>

namespace application {
WorldToGPU::WorldToGPU(t::U32 argc, char* argv[]) :
    platform::IApplication(argc, argv),
    renderer_(settings_loader_, window_),
    scene_manager_(renderer_, GetSceneFileName()),
    gui_(cmd_line_parser_.OptionExists("-g"), window_, true, renderer_) {
    base::Log::Info("'World to GPU' initialized");
}

void WorldToGPU::Update(const platform::Input& input) {
    scene_manager_.Update(input);
}

void WorldToGPU::RenderFrame() {
    scene_manager_.RenderFrame(gui_);
}

void WorldToGPU::Resize(const t::Size32& win_size) {
    renderer_.Resize();
}

// Gets called right after exiting main loop and before exiting app
void WorldToGPU::OnExit() const {
    renderer_.WaitForGPUIdle();
}

void WorldToGPU::KeyPress(const platform::Input& input) {
    // To be used ...
}
void WorldToGPU::MouseMove(const platform::Input& input) {
    scene_manager_.MouseMove(input);
}

void WorldToGPU::MouseButtonPress(const platform::Input& input) {
    // To be used ...
}
}; // application