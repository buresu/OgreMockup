cmake_minimum_required(VERSION 3.12)

project(OgreMockup VERSION 1.0.0)

set(QTDIR $ENV{QTDIR})
set(OGRE_HOME $ENV{OGRE_HOME})

set(CMAKE_PREFIX_PATH ${QTDIR} ${OGRE_HOME})
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules ${OGRE_HOME}/CMake/)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.12")

find_package(Qt5 5.15 COMPONENTS Widgets REQUIRED)
find_package(OGRE 1.12 COMPONENTS REQUIRED)

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

link_directories(${OGRE_LIBRARY_DIRS})
link_directories(${OGRE_PLUGIN_DIR})

file(GLOB_RECURSE SOURCES "src/*[.cpp|.hpp]")
file(GLOB_RECURSE RESOURCES "res/materials/*.material" "res/models/*.mesh")

add_executable(OgreMockup ${SOURCES} ${RESOURCES})

target_link_libraries(OgreMockup Qt5::Widgets)
target_link_libraries(OgreMockup OgreMain Codec_Assimp RenderSystem_GL RenderSystem_GL3Plus)

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
