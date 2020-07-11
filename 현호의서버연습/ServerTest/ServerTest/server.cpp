#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

using namespace std;

#pragma comment (lib, "ws2_32.lib")

int main()
{
	WSADATA wsaData;
	SOCKET Socket;
	SOCKADDR_IN serverAddress;

	// 1. ���� �ʱ�ȭ
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		cout << "Failed WSAStartup()" << endl;

	// 2. ���� ����
	Socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (Socket == INVALID_SOCKET)
		cout << "Failed socket()" << endl;

	// 3. ���� �ڽ��� �ּҸ� ����
	// 3.1 ���� �ּ� ����
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress.sin_port = htons(30002);

	if (bind(Socket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		cout << "Binding Error" << endl;
	if (listen(Socket, 5) == SOCKET_ERROR)
		cout << "listen Error" << endl;

	// 3.2 Ŭ���̾�Ʈ ���� ���
	SOCKADDR_IN clientAddr;
	SOCKET clientSocket;
	int sizeCliAddr = sizeof(clientAddr);
	clientSocket = accept(Socket, (SOCKADDR*)&clientAddr, &sizeCliAddr);
	if (clientSocket == SOCKET_ERROR)
		cout << "Failed accept()" << endl;

	// 4. �����κ��� ��� ���
	char sendData[255] = "hello";
	send(clientSocket, sendData, strlen(sendData) + 1, 0);

	// 5. ���� ���� -> ���� ����
	closesocket(clientSocket);
	closesocket(Socket);
	WSACleanup();

	system("pause");
	return 0;
}