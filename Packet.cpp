#include "Packet.h"

Packet::Packet()
{
	Buffer = new char[DEFAULT_PACKET_SIZE];
}

Packet::~Packet()
{
	delete[] Buffer;
}

char* Packet::GetBufferptr()
{
	return Buffer;
}

int Packet::GetBufferSize()
{
	return Size;
}