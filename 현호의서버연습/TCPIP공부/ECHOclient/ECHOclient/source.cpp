#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

using namespace std;

#pragma comment (lib, "ws2_32.lib")

int main()
{
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;

	// 1. ���� �ʱ�ȭ
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		cout << "Failed WSAStaartup()" << endl;
		return 1;
	}

	// 2. ���� ����
	hSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (hSocket == INVALID_SOCKET)
	{
		cout << "Failed socket()" << endl;
		WSACleanup();
	}

	// 3. ���� ����
	// 3.1 ������ ���� �ּ� ����
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &servAddr.sin_addr);
	servAddr.sin_port = htons(30002);

	// 3.2 ������ ���� �õ�
	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		cout << "Failed connect" << endl;
		closesocket(hSocket);
		WSACleanup();
		return 1;
	}

	// 4. �����κ��� ��� ���
	int recvSize;
	char recvData[255];
	char sendData[255];
	recvSize = recv(hSocket, recvData, sizeof(recvData), 0);
	if (recvSize == SOCKET_ERROR)
		cout << "recv() Error " << endl;
	cout << "recv : " << recvData << endl;

	// 5. ����� ������ ���
	int count = 10;
	while (count--)
	{
		sendData[0] = count;
		send(hSocket, sendData, 1, 0);
		cout << "send data : " << (int)sendData[0] << endl;
		recvSize = recv(hSocket, recvData, sizeof(recvData), 0);
		if (recvSize == SOCKET_ERROR)
		{
			cout << "recv() Error" << endl;
			break;
		}
		cout << "recv Data : " << (int)recvData[0] << endl;
	}

	// 5. ���� ����
	closesocket(hSocket);
	WSACleanup();
	system("pause");
	return 0;
}