# CMakeList.txt : CMake project for Roxy, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.10)

project("Roxy" VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Project structure
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(ASSETS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/assets)

# Source files
set(SOURCES
    ${SRC_DIR}/main.cpp
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})

# Windows-specific settings
if(WIN32)
    # DirectX 11 (comes with Windows SDK)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        d3d11.lib
        dxgi.lib
        d3dcompiler.lib
        dxguid.lib
    )
    
    # DirectXMath is header-only, included in Windows SDK
    # No additional linking needed
endif()

# SDL2 for window management
# Download SDL2 development libraries from: https://www.libsdl.org/download-2.0.php
# For now, we'll use find_package. You can also use FetchContent or vcpkg
find_package(SDL2 CONFIG)
if(SDL2_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2 SDL2::SDL2main)
    message(STATUS "SDL2 found")
else()
    message(WARNING "SDL2 not found. Please install via vcpkg: vcpkg install sdl2:x64-windows")
endif()

# Bullet Physics (optional for now, we'll add later)
# find_package(Bullet CONFIG)
# if(Bullet_FOUND)
#     target_link_libraries(${PROJECT_NAME} PRIVATE ${BULLET_LIBRARIES})
#     target_include_directories(${PROJECT_NAME} PRIVATE ${BULLET_INCLUDE_DIRS})
# endif()

# Compiler warnings
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Copy assets to build directory (for later use)
# add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#     ${ASSETS_DIR} $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
# )
