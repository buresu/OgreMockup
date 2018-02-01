cmake_minimum_required(VERSION 3.1)

project(OgreMockup VERSION 1.0.0)

set(QTDIR $ENV{QTDIR})
set(OGRE_HOME $ENV{OGRE_HOME})
set(ASSIMP_DIR $ENV{ASSIMP_DIR})

set(CMAKE_PREFIX_PATH ${QTDIR} ${OGRE_HOME} ${ASSIMP_DIR})
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules ${OGRE_HOME}/CMake/)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.10")

find_package(Qt5Widgets REQUIRED)
find_package(OGRE REQUIRED)
find_package(assimp REQUIRED)
find_package(OpenGL REQUIRED)

if(${OGRE_STATIC})
find_package(zlib REQUIRED)
find_package(freetype REQUIRED)
find_package(ZZip REQUIRED)
endif()

if(${OGRE_STATIC})
add_definitions(-DOGRE_STATIC)
endif()

include_directories("${PROJECT_SOURCE_DIR}/src")
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${OGRE_INCLUDE_DIRS})
include_directories(${ASSIMP_INCLUDE_DIRS})

link_directories(${OGRE_LIBRARY_DIRS})
link_directories(${OGRE_PLUGIN_DIR})
link_directories(${ASSIMP_LIBRARY_DIRS})

file(GLOB_RECURSE SOURCES "src/*[.cpp|.hpp]")
file(GLOB_RECURSE RESOURCES "res/materials/*.material" "res/models/*.mesh")

add_executable(OgreMockup ${SOURCES} ${RESOURCES})

target_link_libraries(OgreMockup Qt5::Widgets)
target_link_libraries(OgreMockup ${OGRE_LIBRARIES})
target_link_libraries(OgreMockup ${OGRE_RenderSystem_GL_LIBRARIES})
target_link_libraries(OgreMockup ${OGRE_RenderSystem_GL3Plus_LIBRARIES})
target_link_libraries(OgreMockup ${ASSIMP_LIBRARIES})
target_link_libraries(OgreMockup ${OPENGL_LIBRARY})

if(${OGRE_STATIC})
target_link_libraries(OgreMockup ${FREETYPE_LIBRARIES})
target_link_libraries(OgreMockup ${ZLIB_LIBRARIES})
target_link_libraries(OgreMockup ${ZZip_LIBRARIES})
endif()

if(APPLE)
find_library(COCOA_FRAMEWORK Cocoa)
target_link_libraries(OgreMockup ${COCOA_FRAMEWORK})
endif()

if(WIN32)
set_target_properties(OgreMockup PROPERTIES
  WIN32 TRUE
)
elseif(APPLE)
set_target_properties(OgreMockup PROPERTIES
  MACOSX_BUNDLE TRUE
  RESOURCE "${RESOURCES}"
)
endif()
