set(LIB_NAME lsim-platform)

# If headers are not listed to library they will not show in IDE
#set(HEADER_LIST include/lsim/platform/i_application.h )

add_library(${LIB_NAME} i_application.cc)

target_compile_features(${LIB_NAME} PUBLIC cxx_std_17)
target_compile_options(${LIB_NAME} PUBLIC -Wall -Wextra -pedantic -Werror -g  -D _DEBUG)

target_include_directories(${LIB_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(${LIB_NAME} PUBLIC ${SDL2_INCLUDE_DIRS})
#target_include_directories(${LIB_NAME} PUBLIC ${RENDERER_INCLUDE_DIR})

target_link_libraries(${LIB_NAME} PUBLIC ${SDL2_LIBRARIES})
target_link_libraries(${LIB_NAME} PUBLIC lsim-renderer)