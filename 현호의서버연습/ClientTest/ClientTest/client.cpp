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

	// 3. ���� ����
	// 3.1 ������ ���� �ּ� ����
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr);
	serverAddress.sin_port = htons(30002);

	// 3.2 ������ ���� �õ�
	if (connect(Socket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		cout << "Failed connect()" << endl;

	// 4. �����κ��� ��� ���
	int receiveAddrSize;
	char receiveData[255];

	receiveAddrSize = recv(Socket, receiveData, sizeof(receiveData), 0);
	if (receiveAddrSize == -1)
		cout << "Failed recv()" << endl;

	cout << "recv " << receiveAddrSize << " message : " << receiveData << endl;

	// 5. ���� ���� -> ���� ����
	closesocket(Socket);
	WSACleanup();

	system("pause");
	return 0;
}