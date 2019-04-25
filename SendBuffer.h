#pragma once
#include <iostream>

class SendBuffer
{
private:
	const int DEFAULT_BUFFER_SIZE = 100;
public:
	SendBuffer();
	SendBuffer(int Size);
	~SendBuffer();

	char* GetBufferptr();
	int GetBufferSize();
private:
	char* Buffer;
	int Size;

};