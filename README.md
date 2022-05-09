# Xsens Client

This repository is for a C++ library around the Xsens software development kit, 
based on the CMake build system.

It contains a client class to easily get Xsens data over UDP. The code is based
on the example shipped with the SDK.  
A CMake find script is also installed to wrap around the official SDK.

## Requirements

 * The official Xsens SDK: https://www.xsens.com/software-downloads

## Build and install

After cloning or downloading this repository, build and install with:

```shell
mkdir build && cd build
cmake ..
cmake --build .
cmake --install .
```

## Usage

After installing this library, include it into your CMake application with:

```cmake
find_package(XsensClient)
target_link_libraries(application Xsens::XsensClient)
```

Or you are only interested in the Xsens SDK itself:

```cmake
find_package(XsensClient)
target_link_libraries(application Xsens::XsensSDK)
```
