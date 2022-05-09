#
# Find the Xsens Developer Toolkit and make a linkable target
#
# Inputs:
#       `XsensSDK_ROOT` (Optional): Set to the install directory of the SDK
#
# Outputs:
#       `Xsens::XsensSDK` target
#

if (NOT XsensSDK_ROOT AND NOT XsensSDK_INCLUDE_DIR)
    if (ENV{XsensSDK_ROOT})
        set(XsensSDK_ROOT $ENV{XsensSDK_ROOT})
    else ()
        set(_known_versions "1.0.10" "1.0.9" "1.0.8" "1.0.7")
        foreach (_version ${_known_versions})
            set(XsensSDK_ROOT "C:/Program Files/Xsens/Xsens MVN Developer Toolkit ${_version}")
            if (EXISTS "${XsensSDK_ROOT}/")
                break()
            endif ()
        endforeach ()
        # Last version will be kept in case no directory exists
    endif ()
endif ()
mark_as_advanced(XsensSDK_ROOT)

# Find include directory
find_path(XsensSDK_INCLUDE_DIR
        NAMES "xstypes.h"
        DOC "Xsens SDK include directory"
        PATHS "${XsensSDK_ROOT}/x64/include")
mark_as_advanced(XsensSDK_INCLUDE_DIR)

# Find library
if (NOT XsensSDK_LIBRARY)
    find_library(XsensSDK_LIBRARY
            NAMES "xstypes64"
            DOC "Xsens SDK library"
            PATH_SUFFIXES "x64"
            PATHS "${XsensSDK_ROOT}/x64/lib")
    mark_as_advanced(XsensSDK_LIBRARY)
endif ()

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(XsensSDK
        REQUIRED_VARS XsensSDK_LIBRARY XsensSDK_INCLUDE_DIR)

# Make target
if (XsensSDK_FOUND)
    set(XsensSDK_INCLUDE_DIRS "${XsensSDK_INCLUDE_DIR}" "${XsensSDK_SRC}")
    set(XsensSDK_LIBRARIES "${XsensSDK_LIBRARY}")

    # For header-only libraries
    if (NOT TARGET Xsens::XsensSDK)
        add_library(Xsens::XsensSDK UNKNOWN IMPORTED)

        set_target_properties(Xsens::XsensSDK PROPERTIES
                INTERFACE_INCLUDE_DIRECTORIES "${XsensSDK_INCLUDE_DIRS}")

        set_target_properties(Xsens::XsensSDK PROPERTIES
                IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
                IMPORTED_LOCATION "${XsensSDK_LIBRARY}")
    endif ()
endif ()
