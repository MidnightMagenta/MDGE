cmake_minimum_required(VERSION 3.10)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "cmake")

set(ProjectName "MDGE")
set(Subsystem "MDGE_SUBSYSTEM_CONSOLE")
set(GraphicsAPI "OpenGL")
set(VersionMajor 0)
set(VersionMinor 1)
set(VersionPatch 0)

project("${ProjectName}" VERSION 0.1)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")

find_package(OpenGL REQUIRED)

if (NOT DEFINED ENV{VULKAN_SDK})
    message(FATAL_ERROR "VULKAN_SDK is not set.")
endif()

add_subdirectory("${CMAKE_SOURCE_DIR}/dependencies/vma")

add_custom_target(compile_shaders ALL
    COMMENT "Compiling shaders"
    COMMAND "${PROJECT_SOURCE_DIR}/shaders/compile.bat"
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/shaders"
)

set(ShaderBinaryDirectory ${PROJECT_SOURCE_DIR}/shaders/shader_bin)
set(ShaderDirectory ${PROJECT_SOURCE_DIR}/shaders/gl_shaders)

add_custom_target(copy_shaders ALL
    COMMENT "Copying shaders"
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${ShaderDirectory} ${CMAKE_BINARY_DIR}/resources/shaders
)

add_custom_target(q_start_clock ALL
    COMMENT "Starting console clock"
    COMMAND "${PROJECT_SOURCE_DIR}/console_clock/bin/run.bat"
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/console_clock/bin"
)

add_library("${ProjectName}" STATIC
    #src files
    src/windows/win32_window.cpp
    src/windows/win32_cursor.cpp

    src/vulkan/vk_context.cpp
    src/vulkan/vk_window.cpp

    src/OpenGL/gl_window.cpp
    src/OpenGL/gl_program.cpp
    src/OpenGL/gl_buffer.cpp
    src/OpenGL/gl_vertex_buffer.cpp

    src/file.cpp
    #header files
    include/windows/win32_window.hpp
    include/windows/win32_cursor.hpp
    include/windows/win32_console.hpp

    include/vulkan/vk_context.hpp
    include/vulkan/vk_window.hpp

    include/OpenGL/gl_window.hpp
    include/OpenGL/gl_program.hpp
    include/OpenGL/gl_buffer.hpp
    include/OpenGL/gl_vertex_buffer.hpp

    include/file.hpp
)

add_dependencies("${ProjectName}" compile_shaders)
add_dependencies("${ProjectName}" copy_shaders q_start_clock)

target_compile_definitions("${ProjectName}" PUBLIC VERSION_MAJOR=${VersionMajor})
target_compile_definitions("${ProjectName}" PUBLIC VERSION_MINOR=${VersionMinor})
target_compile_definitions("${ProjectName}" PUBLIC VERSION_PATCH=${VersionPatch})

if(Subsystem STREQUAL "MDGE_SUBSYSTEM_CONSOLE")
    set_target_properties("${ProjectName}" PROPERTIES LINK_FLAGS /SUBSYSTEM:CONSOLE)
    target_compile_definitions("${ProjectName}" PRIVATE MDGE_SUBSYSTEM_CONSOLE)
elseif (Subsystem STREQUAL "MDGE_SUBSYSTEM_WINDOWS")
    set_target_properties("${ProjectName}" PROPERTIES LINK_FLAGS /SUBSYSTEM:WINDOWS)
    target_compile_definitions("${ProjectName}" PRIVATE MDGE_SUBSYSTEM_WINDOWS)
endif ()

add_subdirectory("${CMAKE_SOURCE_DIR}/dependencies/OpenGL")

target_include_directories("${ProjectName}" PUBLIC 
    "${CMAKE_SOURCE_DIR}/dependencies" 
    "$ENV{VULKAN_SDK}/Include"
)

target_link_libraries("${ProjectName}" PUBLIC 
    "$ENV{VULKAN_SDK}/Lib/vulkan-1.lib"
    "vma"
    OpenGL::GL
    GL_Dependencies
)

add_subdirectory(tests)