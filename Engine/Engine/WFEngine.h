#pragma once
#ifndef __WFENGINE__
#define __WFENGINE__

#include "IEngine.h"

class IPacketProtocolManager;
class ILogicDispatcher;
class IServerConnectionManager;
class INetworkEngine;

class WFEngine : public IEngine
{
public:
	bool						IsServer()						{ return isServer_; }
	INetworkEngine*				GetNetworkEngine()				{ return pNetworkEngine_; }
	ILogicDispatcher*			GetLogicDispatcher()			{ return pLogicDispatcher_; }
	IPacketProtocolManager*		GetPacketProtocolManager()		{ return pPacketProtocolManager_; }
	IServerConnectionManager*	GetServerConnectionManager()	{ return pServerConnectionManager_; }
protected:
	NET_ERROR_CODE CreateNetworkEngine(char* szModuleName, bool server = false);
private:
	WFEngine();

	bool isServer_;
	HINSTANCE engineHandle_;

	INetworkEngine*				pNetworkEngine_;
	ILogicDispatcher*			pLogicDispatcher_;
	IPacketProtocolManager*		pPacketProtocolManager_;
	IServerConnectionManager*	pServerConnectionManager_;
};

#endif // #ifndef __WFENGINE__
