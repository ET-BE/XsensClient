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

#ifndef UDPSERVER_H
#define UDPSERVER_H

#include "streaming_protocol/streamer.h"
#include "streaming_protocol/ParserManager.h"
#include <xstypes/xssocket.h>
#include <xstypes/xsthread.h>
#include <atomic>
#include <mutex>
#include <thread>
#include <memory>
#include <queue>

/**
 * XSens UDP listener
 *
 * Based on `udp_server.cpp` from the XSens SDK examples. The original example has the data
 * only as a private property, so it cannot be used at all.
 *
 * We also replaced the Xsens thread by a regular std::thread.
 */
class XsensClient
{
public:

    using DatagramPtr = std::shared_ptr<Datagram>;

    /**
     * Create client object.
     *
     * The UDP server is not started by default!
     */
	explicit XsensClient(const XsString& address = "localhost", uint16_t port = 9763);
	~XsensClient();

    /**
     * Threaded callback for reading the UDP data.
     */
	void readMessages();

	void startThread();
	void stopThread();

    /**
     * Pop the last datagram from the stack
     *
     * @return Pointer to the first data in the queue - `nullptr` if queue is empty
     */
    DatagramPtr consumeData();

    /**
     * Pop all datagrams from the stack and return them
     *
     * @return List of pointers to the data - Empty if queue was empty
     */
    std::vector<DatagramPtr> consumeAllData();

protected:

    /**
     * Callback for when a new datagram has come in
     *
     * @param data Deserialized datagram package
     */
    virtual void onNewDatagram(const DatagramPtr& data);

	std::unique_ptr<XsSocket> socket_;
	uint16_t port_;
	XsString hostname_;

	std::unique_ptr<ParserManager> parser_manager_;

    std::unique_ptr<std::thread> xsens_thread_;
    std::atomic_bool running_;

    std::queue<DatagramPtr> data_queue_; ///< Data queue if incoming packets (processed)
    std::mutex data_queue_lock_; ///< Lock access to the data queue
};

#endif
