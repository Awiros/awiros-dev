cmake_minimum_required(VERSION 2.8)
project(awiros-apps C CXX)
set (PROJECT_VERSION 2.1)

set(AWI_ENGYN_DIR "/home/awiros-docker/engyn-build/include")

#------ OPTIONS -------
option(BUILD_WITH_TF "Build lib with tensorfow" OFF)
option(BUILD_LOCAL "Build awiros without docker" OFF)
option(BUILD_WITH_CURL "Build lib with curl" ON)
option(BUILD_WITH_DARKNET "Build lib with darkent" ON)
option(DEBUG           "print info with building" OFF)

option(BUILD_WITH_CUDA "Build with cuda lirary" OFF)
option(BUILD_WITH_FAISS "Build lib with faiss clustering lib" OFF)
option(BUILD_WITH_MXNET "Build lib with mxnet lib" OFF)
option(BUILD_WITH_TENSORRT "Build lib with tensorRT lib" OFF)


#---------------------------

set (CMAKE_CXX_STANDARD 14)
set(CMAKE_VERBOSE_MAKEFILE OFF)

set(CMAKE_JS_INC "$ENV{HOME}/.cmake-js/node-x64/$ENV{AWIROS_NODE_VERSION}/include/node;/home/awiros-docker/awiros-apps/node_modules/nan")

#TODO: Set debug Flag also
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/Release)

set(curdir "/home/awiros-docker/awiros-apps/apps")
FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
SET(dirlist "apps")
SET(dir_cpp "*.cpp")
SET(dir_h "*.h")


find_package(OpenCV 4 REQUIRED)

FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
        IF(NOT ${child} STREQUAL "build")
            file(GLOB SOURCE_FILES ${child}/${dir_cpp} ${child}/${dir_h})
            add_library(${child} SHARED ${SOURCE_FILES})
            target_include_directories(${child} SYSTEM 
                PRIVATE ${CMAKE_JS_INC} 
                PUBLIC "."
                PUBLIC ${AWI_ENGYN_DIR}
                ${OpenCV_INCLUDE_DIRS}
                )
            
            if (BUILD_WITH_TF)
              include(../cmake/tf.cmake)
            endif()
            
            if (BUILD_WITH_DARKNET)
	            include(../cmake/darknet.cmake)
            endif()

            if(BUILD_WITH_CUDA)
              include(../cmake/cuda.cmake)
            endif(BUILD_WITH_CUDA)

            if(BUILD_WITH_FAISS)
              include(../cmake/faiss.cmake)
            endif(BUILD_WITH_FAISS)

            if(BUILD_WITH_MXNET)
              include(../cmake/mxnet.cmake)
            endif(BUILD_WITH_MXNET)

            if(BUILD_WITH_TENSORRT)
              include(../cmake/tensorRT.cmake)
            endif(BUILD_WITH_TENSORRT)

            execute_process (
              COMMAND bash -c "cat /home/awiros-docker/awiros-apps/apps/app-registry.txt | grep ${child} | cut -d ' ' -f1 | tr -d '\n'"
              OUTPUT_VARIABLE app_id
            )
            target_link_libraries(${child} "/home/awiros-docker/engyn-build/Release/libawiros-engyn.so" ${OpenCV_LIBS})
            set_target_properties(${child} PROPERTIES PREFIX "lib" OUTPUT_NAME "${app_id}" SUFFIX ".so")
            message(${child})
        ENDIF()
    ENDIF()
ENDFOREACH()
