cmake_minimum_required(VERSION 3.16)

project(OpenCVGUI VERSION 2.0.0 LANGUAGES C CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

option(OGUI_BUILD_SHARED   "Build OpenCVGUI as a shared library?" ON)
option(OGUI_GL3            "Build OpenCVGUI with OpenGL 3 support?" ON)
option(OGUI_BUILD_EXAMPLES "Build OpenCVGUI examples?" ON)
option(OGUI_SYSTEM_GLFW    "Use system GLFW if available instead of 3rdparty/glfw?" ON)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Dependencies
find_package(OpenCV REQUIRED COMPONENTS core imgproc imgcodecs highgui videoio)
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

# GLFW library: system package first, bundled submodule as fallback
if(OGUI_SYSTEM_GLFW)
    find_package(glfw3 3.2 QUIET)
endif()
if(glfw3_FOUND)
    message(STATUS "Using system GLFW ${glfw3_VERSION}")
else()
    message(STATUS "Using bundled GLFW from 3rdparty/glfw")
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    # Bundled GLFW declares an old cmake_minimum_required; CMake >= 4 needs this
    set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
    add_subdirectory(3rdparty/glfw)
    unset(CMAKE_POLICY_VERSION_MINIMUM)
endif()

# nanovg is compiled inside the library
set(
    NANOVG_SRC
    3rdparty/nanovg/src/nanovg.c
)

file(GLOB OGUI_SRC_LIB CONFIGURE_DEPENDS
    "src/OpenCVGUI/resources/*.cpp"
    "src/OpenCVGUI/*.cpp"
    "src/OpenCVGUI/*.c"
)
file(GLOB OGUI_HEADERS CONFIGURE_DEPENDS "include/OpenCVGUI/*.h")

# Set library type
if(OGUI_BUILD_SHARED)
    set(OGUI_LIBRARY_TYPE SHARED)
else()
    set(OGUI_LIBRARY_TYPE STATIC)
endif()

# Create library
add_library(opencvgui ${OGUI_LIBRARY_TYPE} ${OGUI_SRC_LIB} ${OGUI_HEADERS} ${NANOVG_SRC})
add_library(OpenCVGUI::OpenCVGUI ALIAS opencvgui)

set_target_properties(opencvgui PROPERTIES
                      OUTPUT_NAME "OpenCVGUI"
                      VERSION ${PROJECT_VERSION}
                      SOVERSION ${PROJECT_VERSION_MAJOR}
                      POSITION_INDEPENDENT_CODE ON
                      EXPORT_NAME OpenCVGUI
                      FOLDER "OpenCVGUI")

target_compile_features(opencvgui PUBLIC cxx_std_14)

if(OGUI_GL3)
    target_compile_definitions(opencvgui PRIVATE OGUI_GL3)
endif()

# Public headers include OpenCV, GLEW and glm, so they are part of the interface
target_include_directories(opencvgui
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/OpenCVGUI>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/glm>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/nanovg/src>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/OpenCVGUI>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/tinydir
)

target_link_libraries(opencvgui
    PUBLIC
        ${OpenCV_LIBS}
        GLEW::GLEW
        OpenGL::GL
    PRIVATE
        glfw
)

# Examples
if(OGUI_BUILD_EXAMPLES)
    foreach(example example example1 example_2dplot example3d basic_sample)
        add_executable(${example} src/${example}.cpp)
        target_link_libraries(${example} PRIVATE opencvgui)
    endforeach()

    find_package(OpenMP)
    if(OpenMP_CXX_FOUND)
        add_executable(example_videotimeline src/example_videotimeline.cpp)
        target_link_libraries(example_videotimeline PRIVATE opencvgui OpenMP::OpenMP_CXX)
    else()
        message(STATUS "OpenMP not found, skipping example_videotimeline")
    endif()
endif()

add_executable(font2c src/font2c.cpp)

# INSTALLATION
install(TARGETS opencvgui
        EXPORT OpenCVGUITargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install(FILES ${OGUI_HEADERS}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/OpenCVGUI)

install(DIRECTORY 3rdparty/glm/glm DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(DIRECTORY 3rdparty/nanovg/src/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING PATTERN "*.h")

# Package config
set(OGUI_CONFIG_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR}/cmake/OpenCVGUI)

install(EXPORT OpenCVGUITargets
        NAMESPACE OpenCVGUI::
        DESTINATION ${OGUI_CONFIG_INSTALL_DIR})

configure_package_config_file(OpenCVGUIConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/OpenCVGUIConfig.cmake
        INSTALL_DESTINATION ${OGUI_CONFIG_INSTALL_DIR})

write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/OpenCVGUIConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/OpenCVGUIConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/OpenCVGUIConfigVersion.cmake
        DESTINATION ${OGUI_CONFIG_INSTALL_DIR})
