::for /r %%i in (*.frag, *.vert) do %VULKAN_SDK%/Bin/glslangValidator.exe -V %%i
C:\Libs\VulkanSDK\Bin32/glslangValidator.exe -o ..\product\shaders\vert.spv -V shader.vert
C:\Libs\VulkanSDK\Bin32/glslangValidator.exe -o ..\product\shaders\frag.spv -V shader.frag

C:\Libs\VulkanSDK\Bin32/glslangValidator.exe -o ..\product\shaders\vert_vb.spv -V shader_vb.vert
C:\Libs\VulkanSDK\Bin32/glslangValidator.exe -o ..\product\shaders\frag_vb.spv -V shader_vb.frag

C:\Libs\VulkanSDK\Bin32/glslangValidator.exe -o ..\product\shaders\terrain_vert.spv -V terrain.vert
C:\Libs\VulkanSDK\Bin32/glslangValidator.exe -o ..\product\shaders\terrain_frag.spv -V terrain.frag

pause