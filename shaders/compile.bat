::for /r %%i in (*.frag, *.vert) do %VULKAN_SDK%/Bin/glslangValidator.exe -V %%i

C:\Libs\VulkanSDK\Bin32/glslangValidator.exe -o ..\product\shaders\terrain_vert.spv -V terrain.vert
C:\Libs\VulkanSDK\Bin32/glslangValidator.exe -o ..\product\shaders\terrain_frag.spv -V terrain.frag

pause