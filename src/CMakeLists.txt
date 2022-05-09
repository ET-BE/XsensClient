add_library(XsensClient
        XsensClient.cpp
        streaming_protocol/ParserManager.cpp
        streaming_protocol/angularsegmentkinematicsdatagram.cpp
        streaming_protocol/centerofmassdatagram.cpp
        streaming_protocol/datagram.cpp
        streaming_protocol/eulerdatagram.cpp
        streaming_protocol/JointAnglesDatagram.cpp
        streaming_protocol/linearsegmentkinematicsdatagram.cpp
        streaming_protocol/metadatagram.cpp
        streaming_protocol/positiondatagram.cpp
        streaming_protocol/QuaternionDatagram.cpp
        streaming_protocol/scaledatagram.cpp
        streaming_protocol/streamer.cpp
        streaming_protocol/timecodedatagram.cpp
        streaming_protocol/trackerkinematicsdatagram.cpp)

add_library(Xsens::XsensClient ALIAS XsensClient)

target_include_directories(XsensClient PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
        $<INSTALL_INTERFACE:include>)

target_link_libraries(XsensClient PUBLIC
        Xsens::XsensSDK)
