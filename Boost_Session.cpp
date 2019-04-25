#include "Boost_Session.h"

Boost_Session::Boost_Session(__int64 SessionID, boost::asio::io_service& io_service, Boost_Server* Server)
	: _Socket(io_service)
	, _SessionID(SessionID)
	, _Server(Server)
	, _RecvBuffer(MAX_RECV_BUFFER_SIZE)
{
	SendBuf = new char*[MAX_SEND_BUFFER_COUNT];
	for (int i = 0; i < MAX_SEND_BUFFER_COUNT; i++)
	{
		SendBuf[i] = new char[MAX_SEND_BUFFER_SIZE];
	}
}

Boost_Session::~Boost_Session()
{
	for (int i = 0; i < MAX_SEND_BUFFER_COUNT; i++)
	{
		delete[] SendBuf[i];
	}
	delete[] SendBuf;
}

int Boost_Session::Init()
{

}

boost::asio::ip::tcp::socket& Boost_Session::GetSocket()
{
	return _Socket;
}

void Boost_Session::PostSend()
{

}

void Boost_Session::PostRecv()
{
	_Socket.async_read_some(_RecvBuffer, boost::bind(&Boost_Session::Recv_Handler,
		this, boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred));
}