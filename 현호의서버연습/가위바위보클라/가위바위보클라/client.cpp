#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <time.h>

#pragma comment (lib,"ws2_32.lib")

using namespace std;

int main()
{
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;

	// 1.���� �ʱ�ȭ
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		cout << "Failed Startup()" << endl;
		return 1;
	}

	// 2. ���� ����
	hSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (hSocket == INVALID_SOCKET)
	{
		cout << "Failed Socket()" << endl;
		WSACleanup();
	}

	// 3. ���� ����
	// 3-1. ������ ���� �ּ� ����
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &servAddr.sin_addr);
	servAddr.sin_port = htons(30002);

	// 3-2. ������ ���� �õ�
	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		cout << "Failed connect()" << endl;
		closesocket(hSocket);
		WSACleanup();
		return 1;
	}

	// 4. �����κ��� ��� ���
	int recvByte;
	char recvData[255];
	char sendData[255];

	recvByte = recv(hSocket, recvData, sizeof(recvData), 0);
	if (recvByte == SOCKET_ERROR)
		cout << "Failed recv()" << endl;

	cout << "recv : " << recvData << endl;

	// 5. ����� ������ ���������� ����
	char resultRock[4][4] = {
		{0,0,0,0},
		{0,0,-1,1},
		{0,1,0,-1},
		{0,-1,1,0}
	};

	char result = 0;
	char itemClient = 0;
	char itemServer = 0;
	int flow = 5;

	srand((unsigned int)time(NULL));

	while (flow--)
	{
		cout << "����� ������? : ";
		cin >> itemClient;
		if (itemClient > 0 && itemClient < 4)
		{
			sendData[0] = itemClient;
			// �ڽ��� �� ���ٺ��� ������ ����
			send(hSocket, sendData, 1, 0);
		}

		// server�� ���ٺ� ���� recv ���
		recvByte = recv(hSocket, recvData, sizeof(recvData), 0);
		if (recvByte < 1)
			break;
		itemServer = recvData[0];
		result = resultRock[itemClient][itemServer];

		// ��� ȭ�� ���
		
		Sleep(2000);
		sendData[0] = 1;
		// server�� ���� client�� ���� ó��
		send(hSocket, sendData, 1, 0);
	}

	// 5. ���� ���� -> ���� ����
	closesocket(hSocket);
	WSACleanup();

	system("pause");
	return 0;
}
