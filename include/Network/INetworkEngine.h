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

	// 엔진 종료
	virtual bool Shutdown() = 0;

	// Desc : 데이터 전송
	virtual bool SendRequest(BasePacket* pPacket) = 0;

	// 강제로 세션을 종료하고 싶을 때
	virtual bool Disconnect(int Serial) = 0;

	// 내부적으로 타이머 이벤트를 발생시킬 수 있는지 확인
	virtual bool CheckTimerImpl() { return false; } //default timer 사용
	virtual long AddTimer(unsigned int TimerID, unsigned int StartTime, unsigned int Period) { return false; } //타이머 생성
	virtual bool CancelTimer(int timerID) { return false; }

	//다른 서버와 연결 가능하도록 컨넥터를 추가.
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

// CREATENETWORKENGINE는 함수 포인터
// INetworkEngine*를 반환하고, 두 개의 매개변수를 받는 함수 포인터. 
// 매개변수는 bool 타입과 IEngine* 타입.
// INetworkEngine* (*)(bool, IEngine*)
// typedef INetworkEngine* (CREATENETWORKENGINE)(bool Server, IEngine* pEngine);
using CREATENETWORKENGINE = auto (*)(bool, IEngine*)->INetworkEngine*;

// extern "C"			: C 스타일의 함수명 맹글링.
// NETWORKENGINEDECL	: 확장 선언자. __declspec(dllimport) 같은 특정 플랫폼에서 제공되는 기능을 사용할때 사용.
// NetworkEngine * CreateNetworkEngine(bool Server, IEngine * pEngine)
// : 함수의 선언

#ifdef __linux__
extern "C" INetworkEngine * CreateNetworkEngine(bool Server, IEngine * pEngine);
#else
extern "C" NETWORKENGINEDECL INetworkEngine * CreateNetworkEngine(bool Server, IEngine * pEngine);
#endif // #ifdef __linux__


#endif // #ifndef __INETWORKENGINE__