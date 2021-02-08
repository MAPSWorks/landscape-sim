# Used to compile shader sources to spir-v binaries
# glslc shader compiler is used in should be in PATH
set(TARGET_NAME test-app-shaders)
add_custom_target( ${TARGET_NAME} )

# list of shader sources to compile
set(SOURCES test.vert test.frag)

foreach(file ${SOURCES})
  add_custom_command(
    TARGET ${TARGET_NAME}
    COMMAND glslc  ${CMAKE_CURRENT_SOURCE_DIR}/${file} -o ${file}.spv
  )
endforeach()
