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

	// 3. 서버 연결
	// 3.1 연결할 서버 주소 셋팅
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr);
	serverAddress.sin_port = htons(30002);

	// 3.2 서버에 연결 시도
	if (connect(Socket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		cout << "Failed connect()" << endl;

	// 4. 서버로부터 통신 대기
	int receiveAddrSize;
	char receiveData[255];

	receiveAddrSize = recv(Socket, receiveData, sizeof(receiveData), 0);
	if (receiveAddrSize == -1)
		cout << "Failed recv()" << endl;

	cout << "recv " << receiveAddrSize << " message : " << receiveData << endl;

	// 5. 소켓 종료 -> 윈속 종료
	closesocket(Socket);
	WSACleanup();

	system("pause");
	return 0;
}