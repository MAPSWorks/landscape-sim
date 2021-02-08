set(TARGET_NAME test-app)

add_executable(${TARGET_NAME} main.cc
                              test.cc)

target_compile_features(${TARGET_NAME} PUBLIC cxx_std_17)
if (MSVC)
    target_compile_options(${TARGET_NAME} PUBLIC /W4 /WX -Zi -D _DEBUG)
else()
    target_compile_options(${TARGET_NAME} PUBLIC -Wall -Wextra -pedantic -Werror -g -D _DEBUG)
endif()

target_link_libraries(${TARGET_NAME} PUBLIC lsim-platform)