#pragma once
#include <iostream>
#include <deque>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/atomic.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/lockfree/queue.hpp>
#include "protocol.h"
#include "Boost_Server.h"
#include "Packet.h"

class Boost_Session
{
public:
	Boost_Session(__int64 SessionID, boost::asio::io_service& io_service, Boost_Server* Server);
	~Boost_Session();

	int Init(uint64_t SessionID);

	boost::asio::ip::tcp::socket& GetSocket();
	
	void Send(Packet& Buf);


private:
	void PostSend();
	void PostRecv();
	void Release();
	void Recv_Handler(const boost::system::error_code& error, size_t bytes_transferred);
	void Send_Handler(const boost::system::error_code& error, size_t bytes_transferred);
	bool PacketHandler();

	uint64_t _SessionID;
	boost::atomic<bool> _SendFlag;
	boost::asio::ip::tcp::socket _Socket;
	Boost_Server* _Server;
	boost::circular_buffer<BYTE> _RecvBuffer;
	boost::lockfree::queue<Packet*> _SendBuffer;
	boost::lockfree::queue<Packet*> _SendingBuffer;
};


