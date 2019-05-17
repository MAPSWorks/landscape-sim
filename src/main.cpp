#include <exception>
#include <vulkan/vulkan.hpp>
#include <util/log.h>
#include "app.h"

int main(int argc, char* argv[]) {
    try {
        App app;
        app.Run();
    }
    catch (const std::exception &e) {
        util::Log::Error("Exception: ", e.what());
        return 1;
    }
    catch (...) {
        util::Log::Error("Unknown error");
        return 1;
    }

    return 0;
}