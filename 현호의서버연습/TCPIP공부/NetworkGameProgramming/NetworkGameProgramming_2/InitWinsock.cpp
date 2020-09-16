#pragma comment(lib,"ws2_32")
#include <WinSock2.h>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) // 실습2-1
{
	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	else
	{
		cout << "<윈속 2.2>" << endl;
		cout << "wsa.wVersion : " << wsa.wVersion << endl; // 윈속 버전 ( 0000 0010 0000 0010 )

		unsigned short lowVer = wsa.wVersion >> 8; // 상위 8비트 부버전
		unsigned short highVer = ~wsa.wVersion;

		cout << lowVer << endl;
		cout << highVer << endl;

		cout << "wsa.wHighVersion : " << wsa.wHighVersion << endl; // 윈소 상위 버전
		cout << "wsa.szDescription : " << wsa.szDescription << endl; // 윈속 설명
		cout << "wsa.szSystemStatus : " << wsa.szSystemStatus << endl; // 윈속 상태

		MessageBox(NULL, "윈속 초기화 성공", "알림", MB_OK);
	}

	// 윈속 종료
	WSACleanup();
	return 0;
}

/*
MAKEWORD(2, 2)를 하면 윈속 버전 2.2를 쓰고 wsa.wVersion 값이 514가 뜨고
MAKEWORD(1, 1)하면 257이 뜨는데 그 이유가 MAKEWORD(2, 2)가
하위 8비트에 주 버전을, 상위 8비트에 부 버전이 들어가는건데
2, 2를 하면 0000 0010 / 0000 0010 이 들어가서 512+2해서 514되는거고
1, 1을 하면 0000 0001 / 0000 0001 이 들어가서 256+1해서 257이 되는것

그래서 cout << wsa.wVersion 해서 514, 257 출력시키면 틀린거고
주 버전 부 버전 나눠서 출력시켜라고 하심.
교수님 말로는 0000 0010 / 0000 0010 이 514나오니까 그렇게 출력하지 말고
상위 8비트를 전부 1로 바꿔서 1111 1111 / 0000 0010 만들어서 주 버전을 뽑아내고
부 버전 뽑아내는건 비트 연산자 >> (쉬프트) 이용해서 하라고함.

하위 8비트에 전부 11111111로 & 연산, 상위 8비트에 000000으로 연산  이후 >> 연산
*/