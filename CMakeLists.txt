cmake_minimum_required (VERSION 2.8.3)

project("OpenCVGUI")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

set(OGUI_VERSION_MAJOR "1")
set(OGUI_VERSION_MINOR "0")
set(OGUI_VERSION_PATCH "1")
set(OGUI_VERSION_EXTRA "")
set(OGUI_VERSION "${OGUI_VERSION_MAJOR}.${OGUI_VERSION_MINOR}")
set(OGUI_VERSION_FULL "${OGUI_VERSION}.${OGUI_VERSION_PATCH}${OGUI_VERSION_EXTRA}")


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

option(OGUI_BUILD_SHARED  "Build OpenCVGUI as a shared library?" ON)
option(OGUI_GL3  "Build OpenCVGUI with OpenGL 3 support?" ON)

if(OGUI_GL3)
  add_definitions(-DOGUI_GL3)
endif()

#find OpenCV packages
find_package(OpenCV REQUIRED)

set(EXTRA_LIBS "")
if (WIN32)
  list(APPEND EXTRA_LIBS gdi32 winmm user32 glu32 opengl32 kernel32)
elseif (APPLE)
  find_library(opengl_library OpenGL)
  # Set up library and include paths
  find_library(COCOA_FRAMEWORK Cocoa)
  find_library(IOKIT_FRAMEWORK IOKit)
  find_library(CORE_FOUNDATION_FRAMEWORK CoreFoundation)
  find_library(CORE_VIDEO_FRAMEWORK CoreVideo)
  mark_as_advanced(COCOA_FRAMEWORK
                   IOKIT_FRAMEWORK
                   CORE_FOUNDATION_FRAMEWORK
                   CORE_VIDEO_FRAMEWORK)
  list(APPEND EXTRA_LIBS "${COCOA_FRAMEWORK}"
                             "${IOKIT_FRAMEWORK}"
                             "${CORE_FOUNDATION_FRAMEWORK}"
                             "${CORE_VIDEO_FRAMEWORK}"
                              ${opengl_library})


elseif(CMAKE_SYSTEM MATCHES "Linux")
  list(APPEND EXTRA_LIBS GL)
endif()


find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    link_libraries(${GLEW_LIBRARIES})
    message(STATUS "${GLEW_LIBRARIES} ${GLEW_INCLUDE_DIRS}")
endif()

# GLFW library
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(3rdparty/glfw)

#create nanovg library
set(
    NANOVG_SRC
    3rdparty/nanovg/src/fontstash.h 
    3rdparty/nanovg/src/nanovg.c
    3rdparty/nanovg/src/nanovg.h 
    3rdparty/nanovg/src/nanovg_gl.h 
    3rdparty/nanovg/src/nanovg_gl_utils.h 
    3rdparty/nanovg/src/stb_image.h 
    3rdparty/nanovg/src/stb_truetype.h 
)
# ADD_LIBRARY( nanovg STATIC ${NANOVG_SRC} )

set(
	OGUI_INCLUDE_DIRS
	include/OpenCVGUI
)

set(
        EXTRA_INCLUDES
        3rdparty/nanovg/src/
        3rdparty/glfw/include/
        3rdparty/glm/
)
# include required files
include_directories(
	${OGUI_INCLUDE_DIRS}
    ${EXTRA_INCLUDES}
	${OpenCV_INCLUDE_DIRS}
)
	
file(GLOB OGUI_SRC_LIB
	"src/OpenCVGUI/*.cpp"
    "src/OpenCVGUI/*.c"
	"include/OpenCVGUI/*.h"
)

# Set library type
if (OGUI_BUILD_SHARED)
  set(OGUI_LIBRARY_TYPE "SHARED")
else()
  set(OGUI_LIBRARY_TYPE "STATIC")
endif()

# Create library
ADD_LIBRARY( opencvgui ${OGUI_LIBRARY_TYPE} ${OGUI_SRC_LIB} ${NANOVG_SRC})
	
set_target_properties(opencvgui PROPERTIES
                      OUTPUT_NAME "OpenCVGUI"
                      VERSION ${OGUI_VERSION}
                      SOVERSION ${OGUI_VERSION_MAJOR}
                      POSITION_INDEPENDENT_CODE ON
                      FOLDER "OpenCVGUI")

target_include_directories(opencvgui PUBLIC
			${OGUI_INCLUDE_DIRS}
                           )

#target_include_directories(opencvgui PRIVATE
#target_include_directories(opencvgui PRIVATE
#                           "${OGUI_SRC_LIB}")

# TARGET_LINK_LIBRARIES(opencvgui glfw nanovg)

# Compile a sample project
set(EXTRA_LIBS_EXE "")
if(UNIX)
    find_package(X11)
    if(X11_FOUND)
        add_definitions(-DX11)
        include_directories(${X11_INCLUDE_DIR})
        list(APPEND EXTRA_LIBS_EXE "${X11_LIBRARIES}")
    endif()
endif()

ADD_EXECUTABLE(example src/example.cpp )
TARGET_LINK_LIBRARIES(example ${OpenCV_LIBS} opencvgui glfw ${EXTRA_LIBS} ${GLEW_LIBRARIES} ${EXTRA_LIBS_EXE})

ADD_EXECUTABLE(example1 src/example1.cpp )
TARGET_LINK_LIBRARIES(example1 ${OpenCV_LIBS} opencvgui glfw ${EXTRA_LIBS} ${GLEW_LIBRARIES} ${EXTRA_LIBS_EXE})

ADD_EXECUTABLE(basic_sample src/basic_sample.cpp )
TARGET_LINK_LIBRARIES(basic_sample ${OpenCV_LIBS} opencvgui glfw ${EXTRA_LIBS} ${GLEW_LIBRARIES} ${EXTRA_LIBS_EXE})

# INSTALLATION
install(TARGETS opencvgui
        DESTINATION lib)

install(DIRECTORY ${OGUI_INCLUDE_DIRS}
        DESTINATION include
        FILES_MATCHING PATTERN "*.h")

install(DIRECTORY 3rdparty/glm/glm DESTINATION include)

install(DIRECTORY 3rdparty/nanovg/src/
        DESTINATION include
        FILES_MATCHING PATTERN "*.h")

# Package config
set(INCLUDE_INSTALL_DIR include/ )
set(LIB_INSTALL_DIR lib/ )
include(CMakePackageConfigHelpers)
configure_package_config_file(OpenCVGUIConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/OpenCVGUIConfig.cmake
        INSTALL_DESTINATION ${LIB_INSTALL_DIR}/OpenCVGUI/cmake
        PATH_VARS INCLUDE_INSTALL_DIR)

write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/OpenCVGUIConfigVersion.cmake
        VERSION 1.2.3
        COMPATIBILITY SameMajorVersion )

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/OpenCVGUIConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/OpenCVGUIConfigVersion.cmake
        DESTINATION ${LIB_INSTALL_DIR}/OpenCVGUI/cmake )