project(XsensClient)
CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

option(BUILD_EXAMPLE "Enable to also build the example application" OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(XsensSDK REQUIRED)

add_subdirectory(src)
if(BUILD_EXAMPLE)
    add_subdirectory(example)
endif()

#
# Install the library and the include directory.
# Also install the target exports and the custom *-config.cmake script, which is used to run FindADS for applications
# using this library.
#

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(ConfigPackageLocation ${CMAKE_INSTALL_LIBDIR}/XsensClient/cmake)

install(TARGETS XsensClient
        EXPORT XsensClient-targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(EXPORT XsensClient-targets
        NAMESPACE Xsens::
        DESTINATION ${ConfigPackageLocation})

install(DIRECTORY include/XsensClient
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

configure_package_config_file(cmake/XsensClient-config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/XsensClient-config.cmake
        INSTALL_DESTINATION ${ConfigPackageLocation}
        PATH_VARS CMAKE_INSTALL_INCLUDEDIR)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/XsensClient-config.cmake"
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/FindXsensSDK.cmake"
        DESTINATION ${ConfigPackageLocation})
