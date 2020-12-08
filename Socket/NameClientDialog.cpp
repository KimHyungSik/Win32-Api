#include "framework.h"
#include "1208.h"
#include <winsock2.h>
#include <stdio.h>
#include <tchar.h>

#define MAX_LOADSTRING 100
#define WM_ASYNC WM_USER+2

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    ClinetDialog(HWND, UINT, WPARAM, LPARAM);
HWND hWnd;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, ClinetDialog);
    return 0;
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY1208));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY1208);
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


    switch (message){
    case WM_CREATE:
      

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case ID_CLIENT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, ClinetDialog);
                break;
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
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK ClinetDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static WSADATA wsadata;
    static SOCKET sv;
    static SOCKADDR_IN adr;

    static char sendBuff[1024];
    static char recvBuf[1024];
    static TCHAR name_string[1024];
    static TCHAR hakbug_string[1024];
    static TCHAR msg[1024];

    int msgLen;

    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        WSAStartup(MAKEWORD(2, 2), &wsadata);
        sv = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        adr.sin_family = AF_INET;
        adr.sin_port = 57000;
        adr.sin_addr.S_un.S_addr = inet_addr("192.168.0.8");
        WSAAsyncSelect(sv, hDlg, WM_ASYNC, FD_READ);

        if (connect(sv, (LPSOCKADDR)&adr, sizeof(adr)) == -1) return 0;
        break;
        return (INT_PTR)TRUE;

    case WM_ASYNC:
        switch (lParam) {
        case FD_READ:
            msgLen = recv(sv, recvBuf, 1024, 0);
            recvBuf[msgLen] = NULL;
#ifdef _UNICODE
            msgLen = MultiByteToWideChar(CP_ACP, 0, recvBuf, strlen(recvBuf), NULL, NULL);
            MultiByteToWideChar(CP_ACP, 0, recvBuf, strlen(recvBuf), msg, msgLen);
            msg[msgLen] = NULL;
#else
            strcpy_s(msg, recvBuf);
            msg[msgLen] = NULL;
#endif
            SetDlgItemText(hDlg, IDC_READ,msg);
            break;
        }
        break;

    case WM_COMMAND:

        if (LOWORD(wParam) == IDOK)
        {
            GetDlgItemText(hDlg, IDC_NAME, name_string, 1024);
            GetDlgItemText(hDlg, IDC_HAKBUN, hakbug_string, 1024);
            _stprintf_s(msg, 1024, _T("이름 : %s, 학번 : %s"), name_string, hakbug_string);

#ifdef _UNICODE

            msgLen = WideCharToMultiByte(CP_ACP, 0, msg, _tcslen(msg), NULL, 0, NULL, NULL);
            WideCharToMultiByte(CP_ACP, 0, msg, _tcslen(msg), (LPSTR)sendBuff, msgLen, NULL, NULL);
#else         
            strcpy_s(sendBuff, msg);
            sendBuff[1023] = NULL;
#endif

            send(sv, (LPSTR)sendBuff, 102 + 1, NULL);
            return (INT_PTR)TRUE;
        }
        if (LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
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
