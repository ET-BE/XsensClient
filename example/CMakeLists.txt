add_executable(example mainXsens.cpp)

if(NOT TARGET Xsens::XsensClient)
    find_package(XsensClient)
endif()

target_link_libraries(example Xsens::XsensClient)
