#pragma once
#ifndef __ECHOCLIENT__
#define __ECHOCLIENT__

#include <tchar.h>
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#pragma comment(lib,"ws2_32.lib")



const int PORT_NUM = 32000;
const int PCK_BUFSIZE = 1024;

void ErrHandling(const char* pMsg);

void ErrHandling(const char* pMsg)
{
	std::cout << pMsg << std::endl;
	exit(1);
}

int _tmain(int argc, _TCHAR* argv[])
{

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		ErrHandling("wsastartup error!");
	}

	SOCKET sock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (sock == INVALID_SOCKET) {
		ErrHandling("socket init error!");
	}

	sockaddr_in addr_in;
	memset(&addr_in, 0, sizeof(addr_in));
	addr_in.sin_family = AF_INET;
	addr_in.sin_port = htons(PORT_NUM);
	inet_pton(AF_INET, "127.0.0.1", &addr_in.sin_addr);

	if (connect(sock, (sockaddr*)&addr_in, sizeof(addr_in)) == SOCKET_ERROR) {
		ErrHandling("connect error!");
	}

	DWORD flag = 0;
	while (true) {

		char SendBuf[PCK_BUFSIZE] = { 0, };
		std::cin >> SendBuf;
		std::cout << strlen(SendBuf) << std::endl;
		int sendbytes = send(sock, SendBuf, strlen(SendBuf), 0);
		if (sendbytes < 0) {
			ErrHandling("connect is finish");
		}
		std::cout << "send 잘 됨 ?" << std::endl;
		char RecvBuf[PCK_BUFSIZE] = { 0, };
		int recvbytes = recv(sock, RecvBuf, PCK_BUFSIZE, 0);
		if (recvbytes > 0) {
			std::cout << RecvBuf << std::endl;
		}
		else {
			ErrHandling("recv error!");
		}
	}

	return 0;
}

#endif // #ifndef __ECHOCLIENT__
