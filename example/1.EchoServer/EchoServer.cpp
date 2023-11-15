#pragma once
#ifndef __ECHOSERVER__
#define __ECHOSERVER__

#include <tchar.h>
#include <iostream>
#include <string>
#include <WinSock2.h>
#include <process.h>

#pragma comment (lib,"ws2_32.lib")

const int PORT_NUM = 32000;
const int PCK_BUFSIZE = 1024;

struct WSAPlayerIO {

	WSAOVERLAPPED overlapped;
	WSABUF wsaBuf;
	SOCKET socket;
};



u_int __stdcall PlayerIOworkThread(LPVOID pData);
void ErrHandling(const char* message);
HANDLE CreateNewCompletionPort(DWORD dwConcurThreadCount);
BOOL AssociateDeviceWithCompletionPort(HANDLE socket, HANDLE hIocp, DWORD dwCompletionkey);

u_int __stdcall PlayerIOworkThread(LPVOID pData)
{
	HANDLE params = pData;
	DWORD transferredBytes;
	DWORD flags = 0;
	unsigned __int64 completionKey;
	WSAPlayerIO* PlayerIO;
	while (true) {

		if (GetQueuedCompletionStatus(params, &transferredBytes, &completionKey,
			(LPOVERLAPPED*)(&PlayerIO), INFINITE) == 0)
		{
			ErrHandling("Get queue from IOCP Error!");
			closesocket(PlayerIO->socket);
			delete PlayerIO;
			return 1;
		}

		if (transferredBytes == 0) {
			ErrHandling("RecvBytes is zero");
			closesocket(PlayerIO->socket);

			delete PlayerIO;
			continue;
		}

		PlayerIO->wsaBuf.len = transferredBytes;

		std::cout << "메시지 수신 : " << PlayerIO->wsaBuf.buf << std::endl;


		if (WSASend(PlayerIO->socket, &PlayerIO->wsaBuf, 1, &transferredBytes, flags,
			NULL, NULL) == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSA_IO_PENDING) {
				std::cout << WSAGetLastError() << std::endl;
				ErrHandling("WSASendError!");

			}

		}


		PlayerIO->wsaBuf.len = PCK_BUFSIZE;
		PlayerIO->wsaBuf.buf = new char[PCK_BUFSIZE];
		memset(PlayerIO->wsaBuf.buf, 0, strlen(PlayerIO->wsaBuf.buf));

		if (WSARecv(PlayerIO->socket, &PlayerIO->wsaBuf, 1, &transferredBytes, &flags, &PlayerIO->overlapped, NULL) == SOCKET_ERROR) {

			if (WSAGetLastError() != WSA_IO_PENDING) {
				std::cout << WSAGetLastError() << std::endl;
				ErrHandling("WSARecvError!");
			}
		}
	}

	return 1;
}

void ErrHandling(const char* message)
{
	std::cout << message << std::endl;
	exit(1);
}

HANDLE CreateNewCompletionPort(DWORD dwConcurThreadCount)
{
	return CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, dwConcurThreadCount);
}

BOOL AssociateDeviceWithCompletionPort(HANDLE socket, HANDLE hIocp, DWORD dwCompletionkey)
{
	hIocp = CreateIoCompletionPort(socket, hIocp, dwCompletionkey, 0);

	return socket == NULL ? FALSE : TRUE;
}

int _tmain(int argc, _TCHAR* argv[])
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		ErrHandling("startup error!");
	}

	SOCKET ListenSocket = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (ListenSocket == INVALID_SOCKET) {
		ErrHandling("ListenSocket error!");
	}

	HANDLE hIOCP = CreateNewCompletionPort(0);

	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);

	DWORD threadCount = sysInfo.dwNumberOfProcessors * 2;
	for (int i = 0; i < threadCount; i++) {
		_beginthreadex(nullptr, 0, PlayerIOworkThread, hIOCP, 0, NULL);

	}

	sockaddr_in servAddr;
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(PORT_NUM);
	servAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	if (bind(ListenSocket, (sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
		ErrHandling("bind error!");
		closesocket(ListenSocket);

	}

	if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
		ErrHandling("listen error!");
		closesocket(ListenSocket);
	}

	sockaddr_in clntaddr;
	memset(&clntaddr, 0, sizeof(clntaddr));
	int clntAddrLen = sizeof(clntaddr);
	WSAPlayerIO* PlayerIO = nullptr;
	DWORD receiveLen;
	DWORD flags = 0;

	while (true) {

		SOCKET clientSocket = accept(ListenSocket, (sockaddr*)&clntaddr, &clntAddrLen);
		if (INVALID_SOCKET == clientSocket) {
			ErrHandling("socket error!");
		}
		std::cout << "Welcome !" << std::endl;
		if (AssociateDeviceWithCompletionPort((HANDLE)clientSocket, hIOCP, 0) == FALSE) {
			ErrHandling("Associate Device cp Error!");
		}

		PlayerIO = new WSAPlayerIO();
		memset(&(PlayerIO->overlapped), 0, sizeof(PlayerIO->overlapped));
		PlayerIO->socket = clientSocket;

		PlayerIO->wsaBuf.len = PCK_BUFSIZE;
		PlayerIO->wsaBuf.buf = new char[PCK_BUFSIZE];
		memset(PlayerIO->wsaBuf.buf, 0, strlen(PlayerIO->wsaBuf.buf));


		if (WSARecv(clientSocket, &PlayerIO->wsaBuf, 1, &receiveLen, &flags, &PlayerIO->overlapped, NULL) == SOCKET_ERROR) {

			if (WSAGetLastError() != WSA_IO_PENDING) {

				ErrHandling("WSARecv Error!");
			}
		}

	}

	closesocket(ListenSocket);
	WSACleanup();

	return 0;
}


#endif // #ifndef __ECHOSERVER__
