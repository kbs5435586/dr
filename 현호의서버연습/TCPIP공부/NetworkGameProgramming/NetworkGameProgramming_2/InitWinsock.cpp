#pragma comment(lib,"ws2_32")
#include <WinSock2.h>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) // �ǽ�2-1
{
	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	else
	{
		cout << "<���� 2.2>" << endl;
		cout << "wsa.wVersion : " << wsa.wVersion << endl; // ���� ���� ( 0000 0010 0000 0010 )

		unsigned short lowVer = wsa.wVersion >> 8; // ���� 8��Ʈ �ι���
		unsigned short highVer = ~wsa.wVersion;

		cout << lowVer << endl;
		cout << highVer << endl;

		cout << "wsa.wHighVersion : " << wsa.wHighVersion << endl; // ���� ���� ����
		cout << "wsa.szDescription : " << wsa.szDescription << endl; // ���� ����
		cout << "wsa.szSystemStatus : " << wsa.szSystemStatus << endl; // ���� ����

		MessageBox(NULL, "���� �ʱ�ȭ ����", "�˸�", MB_OK);
	}

	// ���� ����
	WSACleanup();
	return 0;
}

/*
MAKEWORD(2, 2)�� �ϸ� ���� ���� 2.2�� ���� wsa.wVersion ���� 514�� �߰�
MAKEWORD(1, 1)�ϸ� 257�� �ߴµ� �� ������ MAKEWORD(2, 2)��
���� 8��Ʈ�� �� ������, ���� 8��Ʈ�� �� ������ ���°ǵ�
2, 2�� �ϸ� 0000 0010 / 0000 0010 �� ���� 512+2�ؼ� 514�Ǵ°Ű�
1, 1�� �ϸ� 0000 0001 / 0000 0001 �� ���� 256+1�ؼ� 257�� �Ǵ°�

�׷��� cout << wsa.wVersion �ؼ� 514, 257 ��½�Ű�� Ʋ���Ű�
�� ���� �� ���� ������ ��½��Ѷ�� �Ͻ�.
������ ���δ� 0000 0010 / 0000 0010 �� 514�����ϱ� �׷��� ������� ����
���� 8��Ʈ�� ���� 1�� �ٲ㼭 1111 1111 / 0000 0010 ���� �� ������ �̾Ƴ���
�� ���� �̾Ƴ��°� ��Ʈ ������ >> (����Ʈ) �̿��ؼ� �϶����.

���� 8��Ʈ�� ���� 11111111�� & ����, ���� 8��Ʈ�� 000000���� ����  ���� >> ����
*/