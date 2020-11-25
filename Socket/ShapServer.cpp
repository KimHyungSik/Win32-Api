#include "framework.h"
#include "ShapServer.h"
#include <WinSock2.h>
#include <tchar.h>
#include <stdio.h>

#define MAX_LOADSTRING 100

#define IDC_BUTTON_SEND_ 200
#define LINE             1
#define ELLIPSE          2
#define RECTANGLE        3   

HINSTANCE hInst;                                
WCHAR szTitle[MAX_LOADSTRING];                  
WCHAR szWindowClass[MAX_LOADSTRING];            

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
    LoadStringW(hInstance, IDC_SHAPSERVER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SHAPSERVER));

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

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SHAPSERVER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SHAPSERVER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

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
    static SOCKET sv, clt;
    static SOCKADDR_IN addr = { 0 }, clt_addr = {0};
    int size;
    static POINT ptStart, ptEnd;
    static int Draw;
    char buffer[100];

    switch (message)
    {
    case WM_CREATE:
        // 1. 버튼을 소스로 생성
        hButton = CreateWindow(
            _T("button"),
            _T("전송"), 
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
            200, 200, 200, 100, 
            hWnd, 
            (HMENU)IDC_BUTTON_SEND_,
            hInst,NULL);
        EnableWindow(hButton, FALSE);

        //2. 소켓 프로그래밍
        // 1) 소켓 설정
        WSAStartup(MAKEWORD(2, 2), &wsadata);

        // 2)서버 소켓 생성
        sv = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        // 3)소켓에 IP와 PORT을 연결(binding)
        addr.sin_family = AF_INET;
        addr.sin_port = 55000;
        addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

        if (bind(sv, (LPSOCKADDR)&addr, sizeof(addr)) == -1) {
            MessageBox(hWnd, _T("바인딩 에러"), _T("binding error"), MB_ERR_INVALID_CHARS);
            return 1;
        }

        //4) 서버대기, 대기큐 생성listen
        if (listen(sv, 5) == SOCKET_ERROR) {
            MessageBox(hWnd, _T("서버 대기 큐 생성 에러"), _T("listen error"), MB_ERR_INVALID_CHARS);
            return 1;
        }

        // 5) 클라이언트 접속 허가 : 대기 큐를 무한정 반복
        size = sizeof(clt_addr);
        do {
            clt = accept(sv, (LPSOCKADDR)&clt_addr, &size);

        } while (clt == INVALID_SOCKET);

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
            case ID_ELLIPSE:
                Draw = ELLIPSE;
                break;
            case ID_RECTANGLE:
                Draw = RECTANGLE;
                break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case IDC_BUTTON_SEND_:
                sprintf_s(buffer, "%d %d %d %d %d", Draw, ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
                send(clt, buffer, strlen(buffer) + 1, NULL);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            hdc = BeginPaint(hWnd, &ps);
            switch (Draw) {
            case LINE:
                MoveToEx(hdc, ptStart.x, ptStart.y, NULL);
                LineTo(hdc, ptEnd.x, ptEnd.y);
                break;
            case ELLIPSE:
                Ellipse(hdc, ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
                break;
            case RECTANGLE:
                Rectangle(hdc, ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
                break;
            }
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_LBUTTONDOWN:
        ptStart.x = ptEnd.x = LOWORD(lParam);
        ptStart.y = ptEnd.y = HIWORD(lParam);
        break;
    case WM_MOUSEMOVE:
        if (wParam == MK_LBUTTON) {
            ptEnd.x = LOWORD(lParam);
            ptEnd.y = HIWORD(lParam);
            InvalidateRgn(hWnd, NULL, TRUE);
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
        closesocket(clt);
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
