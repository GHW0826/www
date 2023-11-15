#include "NetworkEngine.h"
#include "pch.h"

/// <summary>
/// Acceptor Open
/// </summary>
/// <returns></returns>
bool NetworkEngine::NetworkOpen()
{
	for (auto& acceptor : mapAcceptor_) {
		/*
		ProactorAcceptor* pAcceptor = acceptor.second;

		ACE_INET_Addr listen_addr;
		listen_addr.set(pAcceptor->GetPort());

		if (0 != pAcceptor->open(listen_addr, 0, 1, ACE_DEFAULT_BACKLOG, 1, 0, 1, 1, 1024)) {
			delete pAcceptor;
			return false;
		}
		*/
	}

	return true;
}
