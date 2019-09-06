#include <exception>
#include <base/log.h>
#include "application/world_to_gpu.h"

int main(int argc, char* argv[]) {
#ifndef NDEBUG
    base::Log::Info("Debug mode");
#endif
    try {
        application::WorldToGPU app(argc, argv);
        app.Run();
    }
    catch (const std::exception &e) {
        base::Log::Error("Exception: ", e.what());
        return 1;
    }
    catch (...) {
        base::Log::Error("Unknown error");
        return 1;
    }

    return 0;
}