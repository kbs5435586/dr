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

	// 1. 윈속 초기화
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		cout << "Failed WSAStartup()" << endl;

	// 2. 소켓 생성
	Socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (Socket == INVALID_SOCKET)
		cout << "Failed socket()" << endl;

	// 3. 서버 자신의 주소를 셋팅
	// 3.1 서버 주소 셋팅
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress.sin_port = htons(30002);

	if (bind(Socket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		cout << "Binding Error" << endl;
	if (listen(Socket, 5) == SOCKET_ERROR)
		cout << "listen Error" << endl;

	// 3.2 클라이언트 연결 대기
	SOCKADDR_IN clientAddr;
	SOCKET clientSocket;
	int sizeCliAddr = sizeof(clientAddr);
	clientSocket = accept(Socket, (SOCKADDR*)&clientAddr, &sizeCliAddr);
	if (clientSocket == SOCKET_ERROR)
		cout << "Failed accept()" << endl;

	// 4. 서버로부터 통신 대기
	char sendData[255] = "hello";
	send(clientSocket, sendData, strlen(sendData) + 1, 0);

	// 5. 소켓 종료 -> 윈속 종료
	closesocket(clientSocket);
	closesocket(Socket);
	WSACleanup();

	system("pause");
	return 0;
}