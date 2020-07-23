#include <WinSock2.h>
#include <WS2tcpip.h>

#include <iostream>
#include <string.h>
#include <Windows.h>
#include <process.h>

#pragma comment (lib,"ws2_32.lib")

using namespace std;

unsigned int __stdcall ThreadFunc(void* _pArgs) // Ŭ����
{
	int recvByte;
	char recvData[255];
	int recvCount = 0;
	SOCKET socket = *(SOCKET*)_pArgs;

	while ((recvByte = recv(socket, recvData, sizeof(recvData), 0)) > 0)
	{
		// ������ �״�� ������.
		recvData[recvByte] = NULL;
		cout << "���� ä�� : " << recvData << endl;
	}

	cout << "close Thread" << endl;
	return 0;
}

int main()
{
	int i = 0;
	unsigned int threadID;

	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;

	// 1. ���� �ʱ�ȭ
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		cout << "Failed WSAStartup()" << endl;
		return 1;
	}

	// 2. socket ����
	hSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (hSocket == INVALID_SOCKET)
	{
		WSACleanup();
		cout << "Failed socket()" << endl;
		return 1;
	}

	// 3. ���� �ּ� ����
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &servAddr.sin_addr);
	servAddr.sin_port = htons(30002);

	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		cout << "listen Error" << endl;
		closesocket(hSocket);
		WSACleanup();
		return 1;
	}

	cout << "Connect ..." << endl;

	HANDLE hThread;

	hThread = (HANDLE)_beginthreadex(NULL, 0, &ThreadFunc, (void*)&hSocket, 0, &threadID);
	cout << "start recv thread" << endl;

	char chat[255];
	int chatSize;
	while (true)
	{
		scanf_s("%s", chat, (unsigned)sizeof(chat));
		if (strcmp(chat, "exit") == 0)
			break;
		chatSize = strnlen_s(chat, sizeof(chat));
		if (chatSize > 0)
			send(hSocket, chat, chatSize, 0);
	}

	CloseHandle(hThread);

	// 5. ���� ����
	closesocket(hSocket);
	WSACleanup();
	return 0;
}