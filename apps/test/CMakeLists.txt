set(TARGET_NAME test-app)
# Headers are used in certain IDE's
set(HEADERS test.h
            )
set(SOURCES main.cc
            test.cc)

add_executable(${TARGET_NAME} ${SOURCES} ${HEADERS})

target_compile_features(${TARGET_NAME} PUBLIC cxx_std_17)
if (MSVC)
    target_compile_options(${TARGET_NAME} PUBLIC /W4 /WX -Zi -D _DEBUG)
else()
    target_compile_options(${TARGET_NAME} PUBLIC -Wall -Wextra -pedantic -Werror -g -D _DEBUG)
endif()

target_link_libraries(${TARGET_NAME} PUBLIC lsim-platform)

add_subdirectory(shaders)