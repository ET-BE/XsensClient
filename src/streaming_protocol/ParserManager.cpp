#include <XsensClient/streaming_protocol/ParserManager.h>
#include <XsensClient/datagrams.h>

#include <memory>

ParserManager::DatagramPtr ParserManager::createDgram(StreamingProtocol proto)
{
    switch (proto)
    {
        case SPPoseEuler:		return std::make_shared<EulerDatagram>();
        case SPPoseQuaternion:	return std::make_shared<QuaternionDatagram>();
        case SPPosePositions:	return std::make_shared<PositionDatagram>();
        case SPMetaScaling:		return std::make_shared<ScaleDatagram>();
        case SPMetaMoreMeta:	return std::make_shared<MetaDatagram>();
        case SPJointAngles:					return std::make_shared<JointAnglesDatagram>();
        case SPLinearSegmentKinematics:		return std::make_shared<LinearSegmentKinematicsDatagram>();
        case SPAngularSegmentKinematics:	return std::make_shared<AngularSegmentKinematicsDatagram>();
        case SPTrackerKinematics:			return std::make_shared<TrackerKinematicsDatagram>();
        case SPCenterOfMass:				return std::make_shared<CenterOfMassDatagram>();
        case SPTimeCode:					return std::make_shared<TimeCodeDatagram>();

        default:
            return nullptr;
    }
}

/*! Read single datagram from the incoming stream */
ParserManager::DatagramPtr ParserManager::readDatagram(const XsByteArray &data)
{
    auto type = static_cast<StreamingProtocol>(Datagram::messageType(data));

    auto datagram = createDgram(type);

    if (!datagram)
    {
        return nullptr;
    }

    datagram->deserialize(data);
    return datagram;
}
