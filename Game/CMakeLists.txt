cmake_minimum_required(VERSION 3.16)

project(mars-rover-client)

# Add source files
set(
    SOURCES
    main.cpp
    src/ClientGame.cpp
    src/ClientNetwork.cpp
    src/NetworkServices.cpp
    src/ServerGame.cpp
    src/ServerNetwork.cpp
    src/LoadingHelper.cpp
    src/EntityComponentSystem.cpp
    src/graphics/GameWorld.cpp
    src/graphics/Mob.cpp
    src/graphics/Camera.cpp
    src/graphics/GameWindow.cpp
    src/graphics/Player.cpp
    src/graphics/Shader.cpp
)

# Add header files
set(
    HEADERS
    src/ClientGame.h
    src/ClientNetwork.h
    src/NetworkData.h
    src/NetworkServices.h
    src/ServerGame.h
    src/ServerNetwork.h
    src/EntityComponentSystem.h
    src/GameConstants.h
    src/LoadingHelper.h
    src/graphics/GameWorld.h
    src/graphics/Mob.h
    src/graphics/Camera.h
    src/graphics/core.h
    src/graphics/GameWindow.h
    src/graphics/Player.h
    src/graphics/Shader.h
)

# Require GL
find_package(OpenGL REQUIRED)

# Add include directories
include_directories(
    include
)

# Add library directories
link_directories(
    lib
)

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} glew32s.lib glfw3)

# Move assets to .exe
add_custom_target(CopyShaders ALL
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	"${PROJECT_SOURCE_DIR}/shaders"
	"${CMAKE_BINARY_DIR}/shaders"
)
add_dependencies(mars-rover-client CopyShaders)