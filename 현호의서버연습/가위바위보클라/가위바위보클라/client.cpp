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

	// 1.윈속 초기화
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		cout << "Failed Startup()" << endl;
		return 1;
	}

	// 2. 소켓 생성
	hSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (hSocket == INVALID_SOCKET)
	{
		cout << "Failed Socket()" << endl;
		WSACleanup();
	}

	// 3. 서버 연결
	// 3-1. 연결할 서버 주소 셋팅
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &servAddr.sin_addr);
	servAddr.sin_port = htons(30002);

	// 3-2. 서버에 연결 시도
	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		cout << "Failed connect()" << endl;
		closesocket(hSocket);
		WSACleanup();
		return 1;
	}

	// 4. 서버로부터 통신 대기
	int recvByte;
	char recvData[255];
	char sendData[255];

	recvByte = recv(hSocket, recvData, sizeof(recvData), 0);
	if (recvByte == SOCKET_ERROR)
		cout << "Failed recv()" << endl;

	cout << "recv : " << recvData << endl;

	// 5. 연결된 서버와 가위바위보 진행
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
}
