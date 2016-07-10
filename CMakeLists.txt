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

#find OpenCV packages
find_package(OpenCV REQUIRED)

set(EXTRA_LIBS "")
if (WIN32)
  list(APPEND EXTRA_LIBS opengl32)
elseif (APPLE)
  find_library(cocoa_library Cocoa)
  find_library(opengl_library OpenGL)
  list(APPEND EXTRA_LIBS ${cocoa_library} ${opengl_library})
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
ADD_LIBRARY( nanovg STATIC ${NANOVG_SRC} )

set(
	OGUI_INCLUDE_DIRS
	include/OpenCVGUI/
    3rdparty/nanovg/src/
	3rdparty/glfw/include/
)
# include required files
include_directories(
	${OGUI_INCLUDE_DIRS}
	${OpenCV_INCLUDE_DIRS}
)
	
file(GLOB OGUI_SRC_LIB
	"src/OpenCVGUI/*.cpp"
)

# Set library type
if (OGUI_BUILD_SHARED)
  set(OGUI_LIBRARY_TYPE "SHARED")
else()
  set(OGUI_LIBRARY_TYPE "STATIC")
endif()

# Create library
ADD_LIBRARY( opencvgui ${OGUI_LIBRARY_TYPE} ${OGUI_SRC_LIB} )
	
set_target_properties(opencvgui PROPERTIES
                      OUTPUT_NAME "OpenCVGUI"
                      VERSION ${OGUI_VERSION}
                      SOVERSION ${OGUI_VERSION_MAJOR}
                      POSITION_INDEPENDENT_CODE ON
                      FOLDER "OpenCVGUI")

target_include_directories(opencvgui PUBLIC
			${OGUI_INCLUDE_DIRS}
                           )
target_include_directories(opencvgui PRIVATE
                           "${OGUI_SRC_LIB}")

TARGET_LINK_LIBRARIES(opencvgui ${OpenCV_LIBS} glfw ${EXTRA_LIBS} ${GLEW_LIBRARIES} nanovg)

# Compile a sample project
ADD_EXECUTABLE(example src/example.cpp )
TARGET_LINK_LIBRARIES(example ${OpenCV_LIBS} opencvgui glfw ${EXTRA_LIBS} ${GLEW_LIBRARIES} nanovg)
