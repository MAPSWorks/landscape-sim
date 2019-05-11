::for /r %%i in (*.frag, *.vert) do %VULKAN_SDK%/Bin/glslangValidator.exe -V %%i
C:\Libs\VulkanSDK\Bin32/glslangValidator.exe -o ..\product\shaders\vert.spv -V shader.vert
C:\Libs\VulkanSDK\Bin32/glslangValidator.exe -o ..\product\shaders\frag.spv -V shader.frag
pause