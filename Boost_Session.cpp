#include "Boost_Session.h"

Boost_Session::Boost_Session(__int64 SessionID, boost::asio::io_service& io_service, Boost_Server* Server)
	: _Socket(io_service)
	, _SessionID(SessionID)
	, _Server(Server)
	, _RecvBuffer(MAX_RECV_BUFFER_SIZE)
{
	_SessionID = 0;
	_SendFlag = false;
}

Boost_Session::~Boost_Session()
{

}

int Boost_Session::Init(uint64_t SessionID)
{
	_SessionID = SessionID;
	_SendFlag = false;
}

boost::asio::ip::tcp::socket& Boost_Session::GetSocket()
{
	return _Socket;
}

void Boost_Session::Send(Packet& Buf)
{
	_SendBuffer.push(&Buf);
	PostSend();
}

void Boost_Session::PostSend()
{
	Packet* Buf;
	bool Compare = false;
	bool Result = true;
	if (_SendFlag.compare_exchange_strong(Compare, Result) == false)
		return;
	if (!_SendBuffer.pop(Buf))
		return;
	_SendingBuffer.push(Buf);
	boost::asio::async_write(_Socket, boost::asio::buffer(Buf->GetBufferptr(), Buf->GetBufferSize()),
									boost::bind(&Boost_Session::Send_Handler, this,
									boost::asio::placeholders::error,
									boost::asio::placeholders::bytes_transferred)
							);
}

void Boost_Session::PostRecv()
{
	_Socket.async_read_some(_RecvBuffer, boost::bind(&Boost_Session::Recv_Handler,
		this, boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred));
}

void Boost_Session::Recv_Handler(const boost::system::error_code& error, size_t bytes_transferred)
{
	if (error)
	{
		if (error == boost::asio::error::eof)
		{
			std::cout << "클라이언트와 연결 끊김" << std::endl;
		}
		else
		{
			std::cout << "error No: " << error.value() << " error Message: " << error.message() << std::endl;
		}
		_Server->ReleaseSession(this);
	}
	else
	{
		PacketHandler();
		PostRecv();
	}
}

void Boost_Session::Send_Handler(const boost::system::error_code& error, size_t bytes_transferred)
{
	Packet* Buf;
	bool Compare = true;
	bool Result = false;
	if (_SendFlag.compare_exchange_strong(Compare, Result) == false)
		return;// 에러상황
	if (error)
	{
		if (error == boost::asio::error::eof)
		{
			std::cout << "클라이언트와 연결 끊김" << std::endl;
		} 
		else
		{
			std::cout << "error No: " << error.value() << " error Message: " << error.message() << std::endl;
		}
		_Server->ReleaseSession(this);
	}
	else
	{
		if (!_SendingBuffer.pop(Buf))
			return;//에러상황
		Buf->Delete();
		if (_SendBuffer.empty())
			return;
		PostSend();
	}
	
}

void Boost_Session::Release()
{
	Packet* Buf;
	while (1)
	{
		if (!_SendBuffer.pop(Buf))
			break;
		Buf->Delete();
	}
	while (1)
	{
		if (!_SendingBuffer.pop(Buf))
			break;
		Buf->Delete();
	}
}