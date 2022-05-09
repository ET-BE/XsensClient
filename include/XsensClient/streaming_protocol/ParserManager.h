#ifndef RTOSIM_PARSERMANAGER_H
#define RTOSIM_PARSERMANAGER_H

#include "datagram.h"

/**
 * Xsens UDP message analyser
 *
 * Alternative version of ParserManager that actually outputs
 * data instead of only printing it.
 */
class ParserManager
{
public:

    using DatagramPtr = std::shared_ptr<Datagram>;

    ParserManager() = default;
    ~ParserManager() = default;

    static DatagramPtr readDatagram(const XsByteArray &data);

protected:

    static DatagramPtr createDgram(StreamingProtocol proto);
};


#endif //RTOSIM_PARSERMANAGER_H
