cmake_minimum_required(VERSION 3.25)
project(conway-c LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(EXECUTABLE_NAME "conway")

file(GLOB_RECURSE SOURCES "src/*.c")

add_executable(${EXECUTABLE_NAME} ${SOURCES})

if(CMAKE_C_COMPILER_ID MATCHES "Clang|GNU")
  target_compile_options(${EXECUTABLE_NAME} PRIVATE -Wall -Wextra)
elseif(MSVC)
  target_compile_options(${EXECUTABLE_NAME} PRIVATE /W4)
endif()

target_include_directories(${EXECUTABLE_NAME} PRIVATE 
    include
    vendor/raylib_6_0/include
)

# Cross-Platform Library Linking
if(APPLE)
    # macOS: Link local static lib and native frameworks
    target_link_libraries(${EXECUTABLE_NAME} PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor/raylib_6_0/lib/libraylib.a
        "-framework OpenGL"
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
    )
elseif(WIN32)
    # Windows: Link Windows build of raylib and system dependencies
    target_link_libraries(${EXECUTABLE_NAME} PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor/raylib_6_0/lib/raylib.lib # Adjust name if .a
        winmm
        gdi32
        opengl32
        shell32
    )
else()
    # Linux: Link Linux build of raylib and system dependencies
    target_link_libraries(${EXECUTABLE_NAME} PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor/raylib_6_0/lib/libraylib.a
        GL
        m
        pthread
        dl
        rt
        X11
    )
endif()
