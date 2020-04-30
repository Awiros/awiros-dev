cmake_minimum_required(VERSION 2.8)
project(awiros-apps C CXX)
set (PROJECT_VERSION 2.1)

set(AWI_ENGYN_DIR "/home/awiros-docker/engyn-build/include")

set (CMAKE_CXX_STANDARD 14)
set(CMAKE_VERBOSE_MAKEFILE OFF)

set(CMAKE_JS_INC "$ENV{HOME}/.cmake-js/node-x64/$ENV{AWIROS_NODE_VERSION}/include/node;/home/awiros-docker/awiros-apps/node_modules/nan")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/Release)

find_package(OpenCV 4 REQUIRED)

file(GLOB SOURCE_FILES "*.cpp")
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} SYSTEM 
    PRIVATE ${CMAKE_JS_INC} 
    PUBLIC "."
    PUBLIC ${AWI_ENGYN_DIR}
    )

  target_link_libraries(${PROJECT_NAME} "/home/awiros-docker/engyn-build/Release/libawiros-engyn.so")
#target_link_libraries(${PROJECT_NAME} "/home/awiros-docker/awiros-engyn/build/Release/awiros-engyn.a")
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")

set(libpath "/home/awiros-docker/awiros-apps/apps/build/Release")
file(GLOB libs "${libpath}/*.so")
FOREACH(child ${libs})
    message(${child})
    target_link_libraries(${PROJECT_NAME} "${child}")
ENDFOREACH()

include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

