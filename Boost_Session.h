#pragma once
#include <iostream>
#include <deque>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/lockfree/queue.hpp>
#include "protocol.h"
#include "Boost_Server.h"
#include "SendBuffer.h"

class Boost_Session
{
public:
	Boost_Session(__int64 SessionID, boost::asio::io_service& io_service, Boost_Server* Server);
	~Boost_Session();

	int Init(uint64_t SessionID);

	boost::asio::ip::tcp::socket& GetSocket();
	
	void Send(SendBuffer& Buf);


private:
	void PostSend();

	void PostRecv();
	void Recv_Handler(const boost::system::error_code& error, size_t bytes_transferred);
	void Send_Handler(const boost::system::error_code& error, size_t bytes_transferred);
	uint64_t _SessionID;
	uint8_t _IoCount;
	boost::asio::ip::tcp::socket _Socket;
	Boost_Server* _Server;
	boost::circular_buffer<char> _RecvBuffer;
	//std::deque<SendBuffer*> _SendBuffer;
	boost::lockfree::queue<SendBuffer*> _SendBuffer;
};


