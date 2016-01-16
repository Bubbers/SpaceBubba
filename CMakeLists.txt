cmake_minimum_required(VERSION 2.8)
# Project Name
PROJECT(Super-Bubba-Awesome-Space)

set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/build")
set(OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build/")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGLEW_STATIC")
if(WIN32)
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIRECTORY}")
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}")
SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIRECTORY}")
SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}")
SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIRECTORY}")
SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}")
endif(WIN32)


include_directories("${PROJECT_SOURCE_DIR}/includes")

file(GLOB files_SOURCE
        "${PROJECT_SOURCE_DIR}/src/particle/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/components/*.cpp"
        )


#########################################################
# Testing
#########################################################
add_subdirectory(Bubba-3D)
add_subdirectory(src)
