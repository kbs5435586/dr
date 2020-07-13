#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

using namespace std;

#pragma comment (lib,"ws2_32.lib")

int main()
{
	WSADATA wsaData;
	SOCKET listenSocket;
	SOCKADDR_IN servAddr;

	// 1. 윈속 초기화
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		cout << "Failed WSAStartup()" << endl;
		return 1;
	}

	// 2. 소켓 생성
	listenSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSocket == INVALID_SOCKET)
	{
		WSACleanup();
		cout << "Failed socket()" << endl;
		return 1;
	}

	// 3. 서버 주소 셋팅
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &servAddr.sin_addr);
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

	cout << "Server Start !" << endl;

	SOCKADDR_IN clientAddr;
	SOCKET clientSocket;
	int sizeClientAddr = sizeof(sizeClientAddr);
	char sendData[255] = "Hi";
	char recvByte;
	char recvData[255];
	int recvCount = 0;

	char gameResult[4][4] = {
	{0,0,0,0} ,
	{0,0,-1,1}, //가위
	{0,1,0,-1}, // 바위
	{0,-1,1,0} }; //보

	char result = 0;
	char itemClient = 0;
	char itemServer = 0;
	int flow = 5;
	
	// 4. 접속이 시도된 클라이언트 연결과 통신 처리
	// 4-1. 클라이언트 연결 기다리기
	clientSocket = accept(listenSocket, (SOCKADDR*)&clientAddr, &sizeClientAddr);
	if (clientSocket == INVALID_SOCKET)
		cout << "accept() Error" << endl;
	recvCount = 0;

	cout << "accept client socket : " << clientSocket << endl;
	// 4-2. 연결된 클라이언트에게 Hi send
	send(clientSocket, sendData, strlen(sendData) + 1, 0);



}