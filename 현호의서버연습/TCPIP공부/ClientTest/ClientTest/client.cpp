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
	Socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); // SOCK_STREAM : �������̰� �ŷ��� �� �ִ� ��Ʈ��ũ
	if (Socket == INVALID_SOCKET)
		cout << "Failed socket()" << endl;

	// 3. ���� ����
	// 3.1 ������ ���� �ּ� ����
	memset(&serverAddress, 0, sizeof(serverAddress)); // ��Ա� ���� �ڡڡڡڡ�
	serverAddress.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr); // 127.0.0.1 : ����ȣ��Ʈ, �ڽ��� ��ǻ�� 
															  // ���� ��ǻ�͸� ���� ��ǻ���ΰ� ó�� ����� �� �־� �׽�Ʈ �������� �ַ� ���
	serverAddress.sin_port = htons(30002); // ȣ��Ʈ �� ��Ʈ�p �� : ���� �� ������ ��Ʈ��ũ ǥ�ؿ� �����ִ� ����
	// �� ������ ���� serverAddress ����ü �ȿ� �ּ� ü��, IP, ��Ʈ ����


	// 3.2 ������ ���� �õ�
	if (connect(Socket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) // connect (�� ������ �̿��ؼ�, �� �ּҷ� ������ �ض�, �ּ��� ũ��)
		cout << "Failed connect()" << endl;

	// 4. �����κ��� ��� ���
	int receiveAddrSize;
	char receiveData[255];

	receiveAddrSize = recv(Socket, receiveData, sizeof(receiveData), 0); // recv(��� �������� �ްڴ�, ���� ������ ũ��, )
	if (receiveAddrSize == -1) // ���� ������ �� -1 ��ȯ, ���� ������ �� recv ũ�� ��ȯ
		cout << "Failed recv()" << endl;

	// connect, recv �Ѵ� ���ŷ �Լ�(���� �Լ�)
	// �� �Լ��� ������ �Ǵ� ������ ���� �ڵ�� ������ �ȵ�

	cout << "recv " << receiveAddrSize << " message : " << receiveData << endl;

	// 5. ���� ���� -> ���� ����
	closesocket(Socket); // ���� �����̶� ����
	WSACleanup(); // ���� �ʱ�ȭ�� ����

	system("pause");
	return 0;
}



/*
����(Buffer)��?

�ϳ��� ��ġ���� �ٸ� ��ġ�� �����͸� ������ ��쿡 ���ڰ��� �������� ���ۼӵ��� 
ó���ӵ��� ���� �����Ͽ� ��ȣ�ϰ� ������ �������� ����ϴ� ��￵���� ���� �Ǵ� ���� ������� �Ѵ�. 
���� �߾�ó����ġ�� �ܸ��̳� �ٸ� �������ġ������ ������ �ۼ��ſ��� ����� �������μ� ���۸� �ʿ�� �Ѵ�
*/