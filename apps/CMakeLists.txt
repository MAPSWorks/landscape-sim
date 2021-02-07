set(TARGET_NAME alpha-app)

add_executable(${TARGET_NAME} main.cc
                              alpha.cc)

#target_include_directories(${TARGET_NAME} PUBLIC ${SDL2_INCLUDE_DIRS})
#target_include_directories(${TARGET_NAME} PUBLIC ${GLM_INCLUDE_DIRS})
#target_link_libraries(${TARGET_NAME} PUBLIC ${SDL2_LIBRARIES})
#target_link_libraries(${TARGET_NAME} PUBLIC vulkan)
target_compile_features(${TARGET_NAME} PUBLIC cxx_std_17)
if (MSVC)
    target_compile_options(${TARGET_NAME} PUBLIC /W4 /WX -Zi -D _DEBUG)
else()
    target_compile_options(${TARGET_NAME} PUBLIC -Wall -Wextra -pedantic -Werror -g -D _DEBUG)
endif()

# temporary, in reality make library and link it here
# target_include_directories(${TARGET_NAME} PUBLIC ../include)
target_link_libraries(${TARGET_NAME} PUBLIC lsim-platform)