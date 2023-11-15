#pragma once
#ifndef __NETWORKENGINE__
#define __NETWORKENGINE__

#include "INetworkEngine.h"
#include <map>

class ProactorAcceptor;

using mapAcceptor = std::map<int, ProactorAcceptor*>;

class NetworkEngine : public INetworkEngine
{
public:
	NetworkEngine(IEngine* pEngine);
	virtual ~NetworkEngine(void);

	virtual bool Shutdown() override;

	virtual bool SendRequest(BasePacket* pPacket) override;

	virtual bool Disconnect(int serial) override;

	virtual bool CheckTimerImpl() override; //default timer »ç¿ë
	virtual long AddTimer(unsigned int timerID, unsigned int startTime, unsigned int period) override;
	virtual bool CancelTimer(int timerID) override;

	virtual int AddConnector(int connectorIndex, char* szIP, unsigned short port) override;
	virtual int AddListener(char* szIP, unsigned short port, bool bDefaultListener = false) override;
protected:
	bool NetworkOpen();

	int acceptorIndex_;
	int connectorIndex_;
	int workThreadGroupID_;
	mapAcceptor mapAcceptor_;
private:
	// TODO
	// ACETimeOutHandler m_TimeOutHandler;
};

#endif // #ifndef __NETWORKENGINE__