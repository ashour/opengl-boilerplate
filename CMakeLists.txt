cmake_minimum_required(VERSION 3.16)
project(opengl_boilerplate VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(THIRD_PARTY_DIR third_party)

# ----------------------------------------
# Assimp
# ----------------------------------------
option(BUILD_SHARED_LIBS OFF)
option(ASSIMP_INSTALL OFF)
option(ASSIMP_BUILD_TESTS OFF)
add_subdirectory(${THIRD_PARTY_DIR}/assimp)

# ----------------------------------------
# fmt
# ----------------------------------------
add_subdirectory(${THIRD_PARTY_DIR}/fmt)

# ----------------------------------------
# GLAD
# ----------------------------------------
add_subdirectory(${THIRD_PARTY_DIR}/glad)


# ----------------------------------------
# GLM
# ----------------------------------------
option(GLM_BUILD_TEST OFF)
option(BUILD_SHARED_LIBS OFF)
add_subdirectory(${THIRD_PARTY_DIR}/glm)


# ----------------------------------------
# stb_image
# ----------------------------------------
include_directories(${THIRD_PARTY_DIR}/stb)

# ----------------------------------------
# GLFW
# ----------------------------------------
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
option(GLFW_BUILD_DOCS OFF)
option(GLFW_INSTALL OFF)
add_subdirectory(${THIRD_PARTY_DIR}/glfw)

# ----------------------------------------
# Executables
# ----------------------------------------
file (GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
  src/*.h
  src/*.cpp
)
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE src)

target_link_libraries(${PROJECT_NAME} PRIVATE
  assimp
  fmt
  glad
  glm
  glfw
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
