#include <exception>
#include <vulkan/vulkan.hpp>
#include <util/log.h>
#include "app.h"

int main(int argc, char* argv[]) {
    try {
        App app;
        app.Run();
    }
    // TODO: remove after no vulkan.hpp is used
    catch (const vk::SystemError &e) {
        util::Log::Error("vk::SystemError: ", e.what());
        return 1;
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