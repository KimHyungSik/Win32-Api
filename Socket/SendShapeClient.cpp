#include "framework.h"
#include "2020202.h"
#include <WinSock2.h>
#include <tchar.h>
#include <stdio.h>

#define MAX_LOADSTRING 100
#define WM_ASYNC	WM_USER+2

#define LINE 1
#define ELL 2
#define RE 3


// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY2020202, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY2020202));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
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

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY2020202));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY2020202);
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

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    static HWND hButton;
    static WSADATA wsadata;
    static SOCKET sv;
    static SOCKADDR_IN addr;
    char buffer[100];
    static int msgLen;
    static int Draw = 1;
    static POINT ptStart, ptEnd;

    switch (message)
    {

    case WM_CREATE:
        hButton = CreateWindow(
            _T("button"),
            _T("전송"),
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            200,
            0,
            100,
            25,
            hWnd,
            (HMENU)100,
            hInst,
            NULL
        );
        EnableWindow(hButton, FALSE);
        WSAStartup(MAKEWORD(2, 2), &wsadata);
        sv = socket(AF_INET, SOCK_STREAM, 0);
        addr.sin_family = AF_INET;
        addr.sin_port = 56000;
        addr.sin_addr.S_un.S_addr = inet_addr("192.168.0.8");

        if (connect(sv, (LPSOCKADDR)&addr, sizeof(addr)) == -1) return 0;

        WSAAsyncSelect(sv, hWnd, WM_ASYNC, FD_READ);

        break;
    case WM_ASYNC:
        switch (lParam) {
        case FD_READ:
            msgLen = recv(sv, buffer, 100, 0);
            buffer[msgLen] = NULL;
            sscanf_s(buffer, "%d %d %d %d %d", &Draw, &ptStart.x, &ptStart.y, &ptEnd.x, &ptEnd.y);
            InvalidateRgn(hWnd, NULL, TRUE);
            break;
        }
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case ID_LINE:
                Draw = LINE;
                break;
            case ID_ELL:
                Draw = ELL;
                break;
            case ID_RE:
                Draw = RE;
                break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case 100:
                sprintf(buffer, "%d %d %d %d %d", Draw, ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
                send(sv, buffer, strlen(buffer) + 1, NULL);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_LBUTTONDOWN:
        ptStart.x = ptEnd.x = LOWORD(lParam);
        ptStart.y = ptEnd.y = HIWORD(lParam);
        break;
    case WM_MOUSEMOVE:
        if (wParam == MK_LBUTTON)
        {
            ptEnd.x = LOWORD(lParam);
            ptEnd.y = HIWORD(lParam);
            InvalidateRgn(hWnd, NULL, TRUE);
        }
        break;

    case WM_PAINT:
        {
            hdc = BeginPaint(hWnd, &ps);
            switch (Draw)
            {
            case LINE:
                MoveToEx(hdc, ptStart.x, ptStart.y, NULL);
                LineTo(hdc, ptEnd.x, ptEnd.y);
                break;
            case ELL:
                Ellipse(hdc, ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
                break;
            case RE:
                Rectangle(hdc, ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
                break;
            }
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_LBUTTONUP:
        ptEnd.x = LOWORD(lParam);
        ptEnd.y = HIWORD(lParam);
        InvalidateRgn(hWnd, NULL, TRUE);
        EnableWindow(hButton, TRUE);
        break;
    case WM_DESTROY:
        closesocket(sv);
        WSACleanup();
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
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
