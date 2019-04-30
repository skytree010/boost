#pragma once
#include <iostream>

class PacketPool
{
public:
	
	~PacketPool();
	Packet* Alloc();
	void Free(Packet* packet);
	static PacketPool* CreateInstance();
private:
	static PacketPool* _Instanceptr;
	PacketPool();

};



class Packet
{
private:
	const int DEFAULT_PACKET_SIZE = 100;
public:
	Packet();
	~Packet();

	char* GetBufferptr();
	int GetBufferSize();
	void SetHeader(unsigned int HeaderType);

private:
	char* Buffer;
	int Size;

};