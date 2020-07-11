#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment (lib, "ws2_32.lib")

using namespace std;

int main()
{
	WSADATA wsaData;
	SOCKET listenSocket;
	SOCKADDR_IN servAddr;

	// 1. ���� �ʱ�ȭ
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		cout << "Failed WSAStartup()" << endl;
		return 1;
	}

	// 2. ���� ����
	listenSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSocket == INVALID_SOCKET)
	{
		WSACleanup();
		cout << "Failed socket()" << endl;
		return 1;
	}

	// 3. ���� �ڽ��� �ּҸ� ����
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(30002);

	if (bind(listenSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		cout << "Binding Error" << endl;
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}
	if (listen(listenSocket, 5) == SOCKET_ERROR)
	{
		cout << "Listen Error" << endl;
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	cout << "Server Start !" << endl << endl;

	SOCKADDR_IN clientAddr;
	SOCKET clientSocket;
	int sizeClientAddr = sizeof(clientAddr);
	char sendData[255] = "Hi !";
	int recvData[255];
	char recvByte;
	int recvCount = 0;

	// 4. ������ �õ��� Ŭ���̾�Ʈ ����� ��� ó��
	while (true)
	{
		// 4-1 Ŭ���̾�Ʈ ���� ��ٸ���
		clientSocket = accept(listenSocket, (SOCKADDR*)&clientAddr, &sizeClientAddr);
		if (clientSocket == INVALID_SOCKET)
			cout << "Failed accept()" << endl;
		recvCount = 0;

		cout << "accept client socket : " << clientSocket << endl;

		// 4-2 ����� Ŭ���̾�Ʈ���� send'
		send(clientSocket, sendData, strlen(sendData) + 1, 0);

		// 4-3 ������ client�� ������ ���������� recv ��⸦ �ݺ��Ѵ�.
		cout << "recv count : ";
		while ((recvByte = recv(clientSocket, (char*)recvData, sizeof(recvData), 0)) > 0)
		{
			// ������ �״�� ������
			cout << ++recvCount << " ";
			send(clientSocket, (char*)recvData, recvByte, 0);
		}
		// 4-4 ������ ���� ó��
		cout << endl;
		closesocket(clientSocket);
		cout << "close socket : " << clientSocket;
	}

	closesocket(listenSocket);
	WSACleanup();
	return 0;
}