//
// Created by Ivars Rusbergs in 2021
//
// Abstraction of Vulkan shader module object.
// Includes reading in binary spir-v file.
#ifndef LSIM_RENDERER_VLK_SHADER_NODULE_H_
#define LSIM_RENDERER_VLK_SHADER_NODULE_H_
#include <string>

namespace lsim::renderer::vlk {
// Reads in shader file and creates shader module.
// Shader module can be deleted after pipeline is created.
// Shader module is not tied to anything specific - like shader type or shader entry point.
class ShaderModule {


};

}

#endif