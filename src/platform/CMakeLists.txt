set(LIB_NAME lsim-platform)

# Module specific include directory
set(MODULE_DIR  ${PROJ_INCLUDE_DIR}/lsim/platform)

# Headers are used in certain IDE's
set(HEADERS ${MODULE_DIR}/i_application.h
            ${MODULE_DIR}/types.h
            ${MODULE_DIR}/window.h
)
set(SOURCES i_application.cc
            window.cc)

add_library(${LIB_NAME} ${SOURCES} ${HEADERS})

target_compile_features(${LIB_NAME} PUBLIC cxx_std_17)
if (MSVC)
    target_compile_options(${LIB_NAME} PUBLIC /W4 /WX -Zi -D _DEBUG)
else()
    target_compile_options(${LIB_NAME} PUBLIC -Wall -Wextra -pedantic -Werror -g -D _DEBUG)
endif()

target_include_directories(${LIB_NAME} PUBLIC ${PROJ_INCLUDE_DIR})
target_include_directories(${LIB_NAME} PUBLIC ${SDL2_INCLUDE_DIRS})
#target_include_directories(${LIB_NAME} PUBLIC ${RENDERER_INCLUDE_DIR})
# TODO possible to remove this if base module is dependant on glm
# or rethink if glm types should be together with common types, otherwise we need to include
# glm everywhere where basic types are used
target_include_directories(${LIB_NAME} PUBLIC ${GLM_INCLUDE_DIRS})

target_link_libraries(${LIB_NAME} PUBLIC ${SDL2_LIBRARIES})
target_link_libraries(${LIB_NAME} PUBLIC lsim-renderer)