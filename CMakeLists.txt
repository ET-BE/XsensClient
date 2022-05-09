project(XsensClient)
CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

option(BUILD_EXAMPLE "Enable to also build the example application" OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

find_package(XsensSDK)

add_subdirectory(src)
if(BUILD_EXAMPLE)
    add_subdirectory(example)
endif()
