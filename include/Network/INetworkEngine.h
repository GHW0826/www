#pragma once
#ifndef __INETWORKENGINE__
#define __INETWORKENGINE__


// dll, so
#ifdef __linux__
// linux
#define NETWORKENGINEDECL __attribute__((visibility("default")))
#else
// window
#define NETWORKENGINEDECL __declspec(dllimport)
#endif // #ifdef __linux__


class IEngine;
class BasePacket;

class NETWORKENGINEDECL INetworkEngine
{
public:
	INetworkEngine(IEngine* pEngine)
		: pEngine_(pEngine)
	{}
	virtual ~INetworkEngine() {};

	virtual bool Init(int ioThreadCnt) = 0;

	virtual bool Start(char* szIP, unsigned short Port) = 0;

	// ���� ����
	virtual bool Shutdown() = 0;

	// Desc : ������ ����
	virtual bool SendRequest(BasePacket* pPacket) = 0;

	// ������ ������ �����ϰ� ���� ��
	virtual bool Disconnect(int Serial) = 0;

	// ���������� Ÿ�̸� �̺�Ʈ�� �߻���ų �� �ִ��� Ȯ��
	virtual bool CheckTimerImpl() { return false; } //default timer ���
	virtual long AddTimer(unsigned int TimerID, unsigned int StartTime, unsigned int Period) { return false; } //Ÿ�̸� ����
	virtual bool CancelTimer(int timerID) { return false; }

	//�ٸ� ������ ���� �����ϵ��� �����͸� �߰�.
	virtual int AddConnector(int ConnectorId, char* IP, unsigned short port) { return -1; }

	/// <summary>
	/// create server listener
	/// </summary>
	/// <param name="ip"></param>
	/// <param name="port"></param>
	/// <param name="bDefaultListener"></param>
	/// <returns></returns>
	virtual int AddListener(char* IP, unsigned short Port, bool bDefaultListener = false) { return -1; }


	/// <summary>
	/// engine getter
	/// </summary>
	/// <returns></returns>
	IEngine* GetEngine() { return pEngine_; }
protected:
	IEngine* pEngine_;
};

// CREATENETWORKENGINE�� �Լ� ������
// INetworkEngine*�� ��ȯ�ϰ�, �� ���� �Ű������� �޴� �Լ� ������. 
// �Ű������� bool Ÿ�԰� IEngine* Ÿ��.
// INetworkEngine* (*)(bool, IEngine*)
// typedef INetworkEngine* (CREATENETWORKENGINE)(bool Server, IEngine* pEngine);
using CREATENETWORKENGINE = auto (*)(bool, IEngine*)->INetworkEngine*;

// extern "C"			: C ��Ÿ���� �Լ��� �ͱ۸�.
// NETWORKENGINEDECL	: Ȯ�� ������. __declspec(dllimport) ���� Ư�� �÷������� �����Ǵ� ����� ����Ҷ� ���.
// NetworkEngine * CreateNetworkEngine(bool Server, IEngine * pEngine)
// : �Լ��� ����

#ifdef __linux__
extern "C" INetworkEngine * CreateNetworkEngine(bool Server, IEngine * pEngine);
#else
extern "C" NETWORKENGINEDECL INetworkEngine * CreateNetworkEngine(bool Server, IEngine * pEngine);
#endif // #ifdef __linux__


#endif // #ifndef __INETWORKENGINE__