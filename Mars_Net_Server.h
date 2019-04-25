#pragma once
#pragma comment(lib, "ws2_32")
#pragma comment(lib, "mswsock")
#include <WinSock2.h>
#include <Windows.h>
#include <process.h>
#include "..\RingBuffer\Mars_Circular_Queue.h"
#include "..\SerialBuffer\Mars_Serial_Buffer.h"
#include "..\DataStructure\Mars_Lockfree_Queue.h"
#include "..\DataStructure\Mars_Lockfree_Stack.h"
#include "..\DataStructure\Mars_Memory_Pool.h"
#include "..\CrashDump\Mars_Crash_Dump.h"
#include "..\SystemLog\Mars_SysLog.h"
#include "..\Profiler\Mars_Profiler.h"

#define df_MAX_PACKET_SIZE 200
#define df_BUF_SIZE 100
class Mars_Net_Server
{
public:
	struct st_Session
	{
		__int64 SessionID;
		unsigned long Index;
		unsigned long ReleaseFlag;
		unsigned long IoCount;
		unsigned long SendFlag;
		SOCKET sock;
		Mars_Lockfree_Queue<Mars_Serial_Buffer*> *SendQ;
		Mars_Lockfree_Stack<Mars_Serial_Buffer*> *SendingBuf;
		Mars_Circular_Queue *RecvBuf;
		OVERLAPPED Send_Overlap;
		OVERLAPPED Recv_Overlap;
	};
	//����Ʈ�� ������
	__int64 GetMemorypoolUseSize();
	__int64 GetMemorypoolAllocSize();
	__int64 GetSessionQueAllocSize();
	__int64 GetSessionQueUseSize();
	__int64 GetSessionStackAllocSize();
	__int64 GetSessionStackUseSize();
	//DWORD GetSendTPS();
	//DWORD GetRecvTPS();
	__int64 _AcceptCount = 0;
	unsigned long long _SessionCount = 0;
public:
	Mars_Net_Server();
	~Mars_Net_Server();
	bool Start(const WCHAR *Open_IP, WORD Port, WORD Worker_Count, bool Nagle, WORD Max_Session);
	//	void Stop();
	bool Disconnect(__int64 SessionID);
	int GetClientCount();
	bool SendPacket(__int64 SessionID, Mars_Serial_Buffer *Packet);
	//bool LoginSend(__int64 SessionID);

	//////////////////////////////////////////////////////////////////////////
	//Accept�� ����ó�� �Ϸ��� ȣ��
	//////////////////////////////////////////////////////////////////////////
	virtual void OnClientJoin(ULONG IP, WORD Port, __int64 SessionID) = 0;
	//////////////////////////////////////////////////////////////////////////
	//Disconnect�� ȣ��
	//////////////////////////////////////////////////////////////////////////
	virtual void OnClientLeave(__int64 SessionID) = 0;
	//////////////////////////////////////////////////////////////////////////
	//Accept�� return �� true�� ���ӿϷ�ó�� false�� ���Ӱź�
	//////////////////////////////////////////////////////////////////////////
	virtual bool OnConnectionRequest(ULONG IP, WORD Port) = 0;
	//////////////////////////////////////////////////////////////////////////
	//��Ŷ ���ſϷ�� ȣ��
	//////////////////////////////////////////////////////////////////////////
	virtual void OnRecv(__int64 SessionID, Mars_Serial_Buffer *Packet) = 0;
	//////////////////////////////////////////////////////////////////////////
	//��Ŷ �۽ſϷ�� ȣ��
	//////////////////////////////////////////////////////////////////////////
	virtual void OnSend(__int64 SessionID, int sendsize) = 0;
	//////////////////////////////////////////////////////////////////////////
	//��Ŀ������ GQCS �ٷ� �ϴܿ��� ȣ��
	//////////////////////////////////////////////////////////////////////////
	virtual void OnWorkerThreadBegin() = 0;
	//////////////////////////////////////////////////////////////////////////
	//��Ŀ������ 1���� ���� �� ȣ��
	//////////////////////////////////////////////////////////////////////////
	virtual void OnWorkerThreadEnd() = 0;
	//////////////////////////////////////////////////////////////////////////
	//������ ȣ��
	//////////////////////////////////////////////////////////////////////////
	virtual void OnError(int errorcode, const WCHAR *ErrorString) = 0;
private:
	static unsigned WINAPI AcceptThread(LPVOID p);
	static unsigned WINAPI WorkerThread(LPVOID p);
	void WorkerFunc();
	void AcceptFunc();
	void PostRecv(st_Session* Sesptr);
	void ReleaseSession(st_Session* Sesptr);
	void PostSend(st_Session* Sesptr);
private:
	Mars_Memory_Pool<Mars_Lockfree_Queue<Mars_Serial_Buffer*>::st_Queue_Node> *_pSessionQueMemory;
	Mars_Memory_Pool<Mars_Lockfree_Stack<Mars_Serial_Buffer*>::st_Stack_Node> *_pSessionStackMemory;
	st_Session *_pSessionArray;
	Mars_Lockfree_Queue<st_Session*> *_pFreeSession;
	Mars_Memory_Pool<Mars_Serial_Buffer> *_pPacketPool;
	SOCKET _sListenSocket;
	HANDLE _hiocp;
	int _MaxSessionCount;
	bool _Nagle;
	__int64 _SessionIncrement = 1;
	DWORD _SendTimeTick;
	DWORD _SaveSend = 0;
	DWORD _SendTPS = 0;
	DWORD _RecvTimeTick;
	DWORD _SaveRecv = 0;
	DWORD _RecvTPS = 0;
private:
	const int _DefaultQueSize = 1234;
};