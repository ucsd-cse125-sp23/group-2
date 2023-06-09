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
    src/EntityComponentSystem.cpp
    src/graphics/GameWorld.cpp
    src/graphics/RenderEntity.cpp
    src/graphics/Camera.cpp
    src/graphics/GameWindow.cpp
    src/graphics/Shader.cpp
    src/graphics/stb_image.cpp
    src/graphics/Skybox.cpp
    src/graphics/Cube.cpp
    src/graphics/Health.cpp
    src/Prefabs.cpp
    src/PoissonDisk.cpp
    src/audio/AudioManager.cpp
    src/graphics/gui/GUIElement.cpp
    src/graphics/Text.cpp
    src/graphics/Effects.cpp
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
    src/graphics/GameWorld.h
    src/graphics/RenderEntity.h
    src/graphics/Camera.h
    src/graphics/core.h
    src/graphics/GameWindow.h
    src/graphics/Shader.h
    src/graphics/stb_image.h
    src/graphics/Skybox.h
    src/graphics/ObjectModel.h
    src/graphics/Mesh.h
    src/graphics/Cube.h
    src/Prefabs.h
    src/PoissonDisk.h
    src/audio/AudioManager.h
    src/graphics/Health.h
    src/graphics/gui/GUIElement.h
    src/graphics/Text.h
    src/graphics/Effects.h
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
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} glew32s.lib glfw3.lib assimp-vc143-mt.lib fmod_vc.lib freetype.lib)

# Move assets to .exe
add_custom_command(TARGET mars-rover-client POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	"${CMAKE_SOURCE_DIR}/shaders"
	"${PROJECT_BINARY_DIR}/shaders"
)

# Move dll to .exe
add_custom_command(TARGET mars-rover-client POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	"${CMAKE_SOURCE_DIR}/dll"
	"${PROJECT_BINARY_DIR}"
)