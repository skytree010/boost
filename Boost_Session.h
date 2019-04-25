#pragma once
#include <iostream>
#include <deque>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/circular_buffer.hpp>

#include "protocol.h"

class Boost_Server;

class Boost_Session
{
public:
	Boost_Session(__int64 SessionID, boost::asio::io_service& io_service, Boost_Server* Server);
	~Boost_Session();

	int Init();

	boost::asio::ip::tcp::socket& GetSocket();
	
	void PostSend();
	
	void PostRecv();

private:
	void Recv_Handler(const boost::system::error_code& error, size_t bytes_transferred);
	void Send_Handler(const boost::system::error_code& error, size_t bytes_transferred);
	__int64 _SessionID;
	unsigned long _IoCount;
	boost::asio::ip::tcp::socket _Socket;
	Boost_Server* _Server;
	boost::circular_buffer<char> _RecvBuffer;
	std::deque<char*> _SendBuffer;
	char **SendBuf;
};


class Boost_Server
{

};

