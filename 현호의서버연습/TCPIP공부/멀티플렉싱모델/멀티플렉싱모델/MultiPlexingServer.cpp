#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment (lib,"ws2_32.lib")

using namespace std;

int main()
{
	WSADATA wsaData;
	SOCKET sockServer;
	SOCKADDR_IN addrServer;

	// 1. ���� �ʱ�ȭ
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		cout << "Failed Startup()" << endl;
		return 1;
	}

	// 2. server socket ����
	sockServer = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockServer == INVALID_SOCKET)
	{
		WSACleanup();
		cout << "Failed Server Socket()" << endl;
		return 1;
	}

	// 3. server �ּ� binding �� listen socket
	memset(&addrServer, 0, sizeof(addrServer));
	addrServer.sin_family = AF_INET;
	//addrServer.sin_addr.s_addr = htonl(INADDR_ANY);
	inet_pton(AF_INET, "127.0.0.1", &addrServer.sin_addr);
	addrServer.sin_port = htons(30002);

	if (bind(sockServer, (SOCKADDR*)&addrServer, sizeof(addrServer)))
	{
		cout << "bind Error" << endl;
		closesocket(sockServer);
		WSACleanup();
		return 1;
	}

	if (listen(sockServer, SOMAXCONN) == SOCKET_ERROR)
	{
		cout << "listen Error" << endl;
		closesocket(sockServer);
		WSACleanup();
		return 1;
	}

	cout << "START SERVER !!" << endl;

	// 4�ڡڡڡڡ�. select �Լ� ����

	/*
	FD_ZERO() : fd_set ����ü�� fd_count�� 0���� ������ִ� ��ũ�� �Լ�.
	FD_SET() : fd_set ����ü�� ���� ����� �־��ִ� ��ũ�� �Լ�.
	FD_ISSET() : fd_set ����ü�� �ش� ���� ����� �ִ��� �˷��ִ� ��ũ�� �Լ�.
	FD_CLR() : 	fd_set ����ü���� �ش� ���� ����� �����ϴ� ��ũ�� �Լ�.
	*/

	fd_set fdSocketInfos; // socket ������ �����ϰ� �ִ� ����ü
	fd_set fdReads;		  // ��ȭ�� �о�� ����ü

	int iRet;
	unsigned int i;
	SOCKET sockClient, sockTemp;
	SOCKADDR_IN addrClient;
	int addrSize;
	char recvBuffer[255];
	int recvSize;

	FD_ZERO(&fdSocketInfos);
	FD_SET(sockServer, &fdSocketInfos);

	while (true)
	{
		fdReads = fdSocketInfos;
		// ���ŷ - ������ ������ �ְ� ���
		iRet = select(0, &fdReads, 0, 0, 0);
		if (iRet == SOCKET_ERROR)
			break;

		for (i = 0; i < fdSocketInfos.fd_count; i++)
		{
			sockTemp = fdSocketInfos.fd_array[i];

			// ������ �����߿� ��ȭ�� �Ͼ ������ �ִ���
			if (FD_ISSET(sockTemp, &fdReads))
			{
				// Reads�� �Ͼ ������ ������ ��� accept
				if (sockTemp == sockServer)
				{
					addrSize = sizeof(addrClient);
					sockClient = accept(sockServer, (SOCKADDR*)&addrClient, &addrSize);
					FD_SET(sockClient, &fdSocketInfos);
					
					cout << i << " connected " << sockClient << "  " << addrClient.sin_addr.S_un.S_un_b.s_b1
						<< " , " << addrClient.sin_addr.S_un.S_un_b.s_b2 << " , " << addrClient.sin_addr.S_un.S_un_b.s_b3
						<< " , " << addrClient.sin_addr.S_un.S_un_b.s_b4 << " : " << addrClient.sin_port << endl;
				}
				else // client �� ��� recv ó��
				{
					recvSize = recv(sockTemp, recvBuffer, sizeof(recvBuffer), 0);
					cout << i << " : socket = " << sockTemp << " recv" << endl;
					if (recvSize > 0)
						send(sockTemp, recvBuffer, recvSize, 0);
					else {
						FD_CLR(sockTemp, &fdSocketInfos);
						closesocket(sockTemp);
						cout << "close socket : " << sockTemp << endl;
					}
				}
			}
		}
	}
	closesocket(sockServer);
	WSACleanup();

	return 0;
}