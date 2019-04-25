#include "SendBuffer.h"

SendBuffer::SendBuffer()
{
	Buffer = new char[DEFAULT_BUFFER_SIZE];
}

SendBuffer::SendBuffer(int Size)
{
	Buffer = new char[Size];
}

SendBuffer::~SendBuffer()
{
	delete[] Buffer;
}

char* SendBuffer::GetBufferptr()
{
	return Buffer;
}

int SendBuffer::GetBufferSize()
{
	return Size;
}