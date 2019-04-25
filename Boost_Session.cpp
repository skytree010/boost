#include "Boost_Session.h"

Boost_Session::Boost_Session(__int64 SessionID, boost::asio::io_service& io_service, Boost_Server* Server)
	: _Socket(io_service)
	, _SessionID(SessionID)
	, _Server(Server)
	, _RecvBuffer(MAX_RECV_BUFFER_SIZE)
	, _SendBuffer(100)
{
	_IoCount = 0;
	_SessionID = 0;
}

Boost_Session::~Boost_Session()
{

}

int Boost_Session::Init(uint64_t SessionID)
{
	_SessionID = SessionID;
}

boost::asio::ip::tcp::socket& Boost_Session::GetSocket()
{
	return _Socket;
}

void Boost_Session::Send(SendBuffer& Buf)
{
	_SendBuffer.push(&Buf);
	PostSend();
}

void Boost_Session::PostSend()
{
	SendBuffer* Buf;
	if (_SendBuffer.empty())
		return;
	while (1)
	{
		if (!_SendBuffer.pop(Buf))
			return;
		_SendingBuffer.push(Buf);
		boost::asio::async_write(_Socket, boost::asio::buffer(Buf->GetBufferptr(), Buf->GetBufferSize()),
											boost::bind(&Boost_Session::Send_Handler, this,
											boost::asio::placeholders::error,
											boost::asio::placeholders::bytes_transferred)
								);
	}
}

void Boost_Session::PostRecv()
{
	_Socket.async_read_some(_RecvBuffer, boost::bind(&Boost_Session::Recv_Handler,
		this, boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred));
}