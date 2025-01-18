cmake_minimum_required(VERSION 3.16)
project(opengl_boilerplate VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# ----------------------------------------
# GLFW
# ----------------------------------------
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
option(GLFW_BUILD_DOCS OFF)
option(GLFW_INSTALL OFF)
add_subdirectory(third_party/glfw)


# ----------------------------------------
# GLAD
# ----------------------------------------
add_subdirectory(third_party/glad)


# ----------------------------------------
# GLM
# ----------------------------------------
option(GLM_BUILD_TEST OFF)
option(BUILD_SHARED_LIBS OFF)
add_subdirectory(third_party/glm)


# ----------------------------------------
# Executables
# ----------------------------------------
add_executable(${PROJECT_NAME}
  src/main.cpp
  src/lib/file_to_string.cpp
  src/rendering/camera.cpp
  src/rendering/mesh.cpp
  src/rendering/shader.cpp
  src/system/input.cpp
  src/system/window.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE src)

target_link_libraries(${PROJECT_NAME} PRIVATE
  glfw
  glad
  glm
)

if (APPLE)
  find_library(COCOA_LIBRARY Cocoa)
  find_library(IOKIT_LIBRARY IOKit)
  find_library(CORE_VIDEO_LIBRARY CoreVideo)

  target_link_libraries(${PROJECT_NAME} PRIVATE
    ${COCOA_LIBRARY}
    ${IOKIT_LIBRARY}
    ${CORE_VIDEO_LIBRARY}
  )
endif()


# ----------------------------------------
# Debug
# ----------------------------------------
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_definitions(DEBUG)
endif()

# ----------------------------------------
# Resources
# ----------------------------------------
add_custom_target(copy_resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/resources
        ${CMAKE_CURRENT_BINARY_DIR}/resources
    COMMENT "Copying resources directory..."
)

add_dependencies(${PROJECT_NAME} copy_resources)
