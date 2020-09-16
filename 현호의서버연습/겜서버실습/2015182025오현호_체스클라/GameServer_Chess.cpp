// GameServer_Chess.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "GameServer_Chess.h"

using namespace std;

#define MAX_LOADSTRING 100

// 서버 변수, 함수:
void OpenServer(HWND);
constexpr int BUF_SIZE = 1024;
constexpr short PORT = 3500;
SOCKET S_SOCKET, C_SOCKET;

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// 메인 함수
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, // H는 핸들, 
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_GAMESERVERCHESS, szWindowClass, MAX_LOADSTRING); // 창 제목
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GAMESERVERCHESS));

    MSG msg;

    //WPARAM      wParam; 키보드 관련 정보
    //LPARAM      lParam; 마우스 관련 정보

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0)) // while(1) 과 같은 기능
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW; // 가로 세로 변화 감지
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GAMESERVERCHESS));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL; //MAKEINTRESOURCEW(IDC_GAMESERVERCHESS); // 상단 메뉴
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,  // || 이용하여 사용할거 추가
      CW_USEDEFAULT, 0, 816, 839, nullptr, nullptr, hInstance, nullptr); // 실제 윈도우 창 만드는 부분

   /*
   WS_OVERLAPPEDWINDOW = 윈도우창 각종 설정들

   앞 CW_USEDEFAULT, 0 = 윈도우 창 생성위치
   뒤 CW_USEDEFAULT, 0 = 윈도우 창 크기
   
   */

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int PieceX = 0, PieceY = 0;
    OpenServer(hWnd);
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_KEYDOWN:
        while (true)
        {
            SOCKADDR_IN client_a;
            INT a_size = sizeof(client_a);
            SOCKET c_socket = WSAAccept(S_SOCKET, (sockaddr*)&client_a, &a_size, NULL, NULL);
            cout << "new client accepted\n";
            while (true)
            {
                char buff[BUF_SIZE + 1];

                WSABUF wsabuf;
                wsabuf.buf = buff;
                wsabuf.len = BUF_SIZE;

                DWORD num_recv;
                DWORD flag = 0;
                WSARecv(c_socket, &wsabuf, 1, &num_recv, &flag, NULL, NULL);
                if (0 == num_recv) break;
                cout << "Received " << num_recv << " Bytes [" << wsabuf.buf << "]\n";

                DWORD num_sent;
                wsabuf.len = num_recv;
                WSASend(c_socket, &wsabuf, 1, &num_sent, 0, NULL, NULL);
                cout << "Sent " << wsabuf.len << " Bytes [" << buff << "]\n";
            }
            cout << "Client Connection Close" << endl;
            closesocket(c_socket);
        }
        switch (wParam)
        {
        case VK_UP:
            if (PieceY == 0)
                break;
            else
                PieceY -= 100;
            break;
        case VK_DOWN:
            if (PieceY == 700)
                break;
            else
                PieceY += 100;
            break;
        case VK_LEFT:
            if (PieceX == 0)
                break;
            else
                PieceX -= 100;
            break;
        case VK_RIGHT:
            if (PieceX == 700)
                break;
            else
                PieceX += 100;
            break;
        }
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

        HBRUSH nowBrush;
        HBRUSH oldBrush;

        nowBrush = (HBRUSH)CreateSolidBrush(RGB(128, 64, 0));
        oldBrush = (HBRUSH)SelectObject(hdc, nowBrush);

        Rectangle(hdc, 0, 0, 800, 800);

        nowBrush = (HBRUSH)CreateSolidBrush(RGB(172, 160, 79));
        oldBrush = (HBRUSH)SelectObject(hdc, nowBrush);

        for (int i = 0; i < 4; ++i)
        {
            Rectangle(hdc, 0, i * 200, 100, (i * 200) + 100);
            Rectangle(hdc, 200, i * 200, 300, (i * 200) + 100);
            Rectangle(hdc, 400, i * 200, 500, (i * 200) + 100);
            Rectangle(hdc, 600, i * 200, 700, (i * 200) + 100);

            Rectangle(hdc, 100, (i * 200) + 100, 200, (i * 200) + 200);
            Rectangle(hdc, 300, (i * 200) + 100, 400, (i * 200) + 200);
            Rectangle(hdc, 500, (i * 200) + 100, 600, (i * 200) + 200);
            Rectangle(hdc, 700, (i * 200) + 100, 800, (i * 200) + 200);
        }

        nowBrush = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
        oldBrush = (HBRUSH)SelectObject(hdc, nowBrush);

        Ellipse(hdc, 0 + PieceX, 0 + PieceY, 100 + PieceX, 100 + PieceY);

        SelectObject(hdc, oldBrush);
        DeleteObject(nowBrush);
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        closesocket(S_SOCKET);
        WSACleanup();
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void OpenServer(HWND hWnd)
{
    WSADATA WSAdata;
    WSAStartup(MAKEWORD(2, 0), &WSAdata);

    S_SOCKET = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);
    SOCKADDR_IN server_a;
    memset(&server_a, 0, sizeof(server_a));
    server_a.sin_family = AF_INET;
    server_a.sin_port = htons(PORT);
    server_a.sin_addr.s_addr = INADDR_ANY;

    ::bind(S_SOCKET, (sockaddr*)&server_a, sizeof(server_a));
    listen(S_SOCKET, SOMAXCONN);
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
   
    return (INT_PTR)FALSE;
}
