/*! \file
	\section FileCopyright Copyright Notice
	This is free and unencumbered software released into the public domain.

	Anyone is free to copy, modify, publish, use, compile, sell, or
	distribute this software, either in source code form or as a compiled
	binary, for any purpose, commercial or non-commercial, and by any
	means.

	In jurisdictions that recognize copyright laws, the author or authors
	of this software dedicate any and all copyright interest in the
	software to the public domain. We make this dedication for the benefit
	of the public at large and to the detriment of our heirs and
	successors. We intend this dedication to be an overt act of
	relinquishment in perpetuity of all present and future rights to this
	software under copyright law.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
	IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
	OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
	ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
	OTHER DEALINGS IN THE SOFTWARE.
*/

#include <XsensClient/XsensClient.h>

XsensClient::XsensClient(const XsString& address, uint16_t port) :
    running_(false)
{
    port_ = port;
    hostname_ = address;

    parser_manager_ = std::make_unique<ParserManager>();
    socket_ = std::make_unique<XsSocket>(IpProtocol::IP_UDP, NetworkLayerProtocol::NLP_IPV4);

    XsResultValue res = socket_->bind(hostname_, port_);

    if (res != XRV_OK) {
        std::cout << "Failed to bind UDP server..." << std::endl;
    }
}

XsensClient::~XsensClient() {
    stopThread();
}

void XsensClient::readMessages() {
    XsByteArray buffer;

    std::cout << "Waiting to receive packets from the client on port " << port_ << " ..." << std::endl << std::endl;

    while (running_.load()) {
        socket_->read(buffer, 100); // Limited timeout so we can kill the thread neatly

        if (!buffer.empty()) {

            auto data = parser_manager_->readDatagram(buffer);

            if (data) {
                onNewDatagram(data);
            }
        }

        buffer.clear();
        XsTime::msleep(1);
    }
}

void XsensClient::onNewDatagram(const DatagramPtr& data) {
    std::lock_guard<std::mutex> guard(data_queue_lock_);
    data_queue_.push(data);

    while (data_queue_.size() > 100) {
        data_queue_.pop(); // Limit the queue size
    }
}

void XsensClient::startThread() {
    if (xsens_thread_) {
        return; // Already running
    }

    running_.store(true); // Let thread run

    xsens_thread_ = std::make_unique<std::thread>(&XsensClient::readMessages, this);
}

void XsensClient::stopThread() {
    if (!xsens_thread_) {
        return; // Not yet running
    }

    running_.store(false);

    if (xsens_thread_) {
        xsens_thread_->join();
        xsens_thread_.reset();
    }
}

XsensClient::DatagramPtr XsensClient::consumeData() {

    std::lock_guard<std::mutex> guard(data_queue_lock_);

    if (data_queue_.empty()) {
        return nullptr;
    }

    auto data = data_queue_.front();

    data_queue_.pop();

    return data;
}

std::vector<XsensClient::DatagramPtr> XsensClient::consumeAllData() {

    std::vector<XsensClient::DatagramPtr> list;

    DatagramPtr data;

    // Keeping getting data until an empty one is returned
    do {
        data = consumeData();
        if (data) {
            list.push_back(data);
        }
    } while (data);

    return list;
}
