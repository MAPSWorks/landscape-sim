//
// Created by Ivars Rusbergs in 2021
//
#include "alpha.h"

#include <lsim/platform/i_application.h>

Alpha::Alpha(int argc, char *argv[]) : lsim::platform::IApplication(argc, argv) {



}


/*
#include <iostream>
#include <stdexcept>
#include <vector>

// Enable the WSI extensions
#if defined(__ANDROID__)
#define VK_USE_PLATFORM_ANDROID_KHR
#elif defined(__linux__)
#define VK_USE_PLATFORM_XLIB_KHR
#elif defined(_WIN32)
#define VK_USE_PLATFORM_WIN32_KHR
#endif
#include <SDL2/SDL_syswm.h>
#include <SDL2/SDL_vulkan.h>

Application::Application(int argc, char *argv[]) : name_("Alpha") {
  (void)argc;
  (void)argv;

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw std::runtime_error("Could not initialize SDL.");
  }
  window_ =
      SDL_CreateWindow(name_.c_str(), SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_VULKAN);
  if (window_ == NULL) {
    throw std::runtime_error("Could not create SDL window.");
  }

  // Get WSI extensions from SDL
  unsigned extension_count;
  if (!SDL_Vulkan_GetInstanceExtensions(window_, &extension_count, NULL)) {
    throw std::runtime_error(
        "Could not get the number of required instance extensions from SDL.");
  }
  std::vector<const char *> extensions(extension_count);
  if (!SDL_Vulkan_GetInstanceExtensions(window_, &extension_count,
                                        extensions.data())) {
    throw std::runtime_error(
        "Could not get the names of required instance extensions from SDL.");
  }

  // Use validation layers if this is a debug build
  std::vector<const char *> layers;
#if defined(_DEBUG)
  layers.push_back("VK_LAYER_KHRONOS_validation");
#endif

  // VkApplicationInfo allows the programmer to specifiy some basic information
  // about the program, which can be useful for layers and tools to provide more
  // debug information.
  VkApplicationInfo appInfo = {};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pNext = NULL;
  appInfo.pApplicationName = name_.c_str();
  appInfo.applicationVersion = 1;
  appInfo.pEngineName = "LSIM";
  appInfo.engineVersion = 1;
  appInfo.apiVersion = VK_API_VERSION_1_0;

  // VkInstanceCreateInfo is where the programmer specifies the layers and/or
  // extensions that are needed.
  VkInstanceCreateInfo instInfo = {};
  instInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  instInfo.pNext = NULL;
  instInfo.flags = 0;
  instInfo.pApplicationInfo = &appInfo;
  instInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
  instInfo.ppEnabledExtensionNames = extensions.data();
  instInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
  instInfo.ppEnabledLayerNames = layers.data();

  // Create the Vulkan instance.
  VkResult result = vkCreateInstance(&instInfo, NULL, &instance_);
  if (result == VK_ERROR_INCOMPATIBLE_DRIVER) {
    throw std::runtime_error("Unable to find a compatible Vulkan Driver.");
  } else if (result) {
    throw std::runtime_error(
        "Could not create a Vulkan instance (for unknown reasons).");
  }

  // Create a Vulkan surface for rendering
  if (!SDL_Vulkan_CreateSurface(window_, instance_, &surface_)) {
    throw std::runtime_error("Could not create a Vulkan surface.");
  }
}

Application::~Application() {
  vkDestroySurfaceKHR(instance_, surface_, NULL);
  SDL_DestroyWindow(window_);
  SDL_Quit();
  vkDestroyInstance(instance_, NULL);
}

void Application::Run() {

  std::cout << "Entering loop" << std::endl;
  // Poll for user input.
  bool stillRunning = true;
  while (stillRunning) {

    SDL_Event event;
    while (SDL_PollEvent(&event)) {

      switch (event.type) {

      case SDL_QUIT:
        stillRunning = false;
        break;

      default:
        // Do nothing.
        break;
      }
    }

    // ... render

    SDL_Delay(10);
  }
}
*/