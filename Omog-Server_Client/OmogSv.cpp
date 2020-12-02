#include "framework.h"
#include "OmogSv04.h"
#include <WinSock2.h>
#include <tchar.h>
#include <stdio.h>

#define WM_ASYNC       WM_USER+2 
#define MAX_LOADSTRING 100

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
    LoadStringW(hInstance, IDC_OMOGSV04, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_OMOGSV04));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_OMOGSV04));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_OMOGSV04);
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

#define BOX_SIZE 30
int isCheck[22][22];

int five_checkX(int x, int y, char count, bool direction, char team) {
    if (count == 5) return count;

    if (direction) x ++;
    else x--;

    if (isCheck[x][y] == team) five_checkX(x, y, count + 1, direction, team);
    else return count;
}

int five_checkY(int x, int y, char count, bool direction, char team) {
    if (count == 5) return count;

    if (direction) y--;
    else y++;

    if (isCheck[x][y] == team) five_checkY(x, y, count + 1, direction, team);
    else return count;
}

int five_checkXY1(int x, int y, char count, bool direction, char team) {
    if (count == 5) return count;

    if (direction) { y--; x--; }
    else { y++; x++; }

    if (isCheck[x][y] == team) five_checkXY1(x, y, count + 1, direction, team);
    else return count;
}

int five_checkXY2(int x, int y, char count, bool direction, char team) {
    if (count == 5) return count;

    if (direction) { y--; x++; }
    else { y++; x--; }

    if (isCheck[x][y] == team) five_checkXY2(x, y, count + 1, direction, team);
    else return count;
}

bool five_check(int x, int y, char team) {
    if (five_checkX(x, y, 1, 1, team) + five_checkX(x, y, 1, 0, team) > 5 ||
        five_checkY(x, y, 1, 1, team) + five_checkY(x, y, 1, 0, team) > 5 ||
        five_checkXY1(x, y, 1, 1, team) + five_checkXY1(x, y, 1, 0, team) > 5 ||
        five_checkXY2(x, y, 1, 1, team) + five_checkXY2(x, y, 1, 0, team) > 5)
        return true;
    return false;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    HPEN hPen;
    static HBRUSH orginal_brush, new_brush;
    static int flag, Winner;
    static WSADATA wsadata;
    static SOCKET sv, clt;
    static SOCKADDR_IN adr, clt_adr;
    int size, msgLen, x, y, i, j;
    char buff[100];

    switch (message)
    {
    case WM_CREATE:
        WSAStartup(MAKEWORD(2, 2), &wsadata);
        sv = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        adr.sin_family = AF_INET;
        adr.sin_port = 55000;
        adr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
        bind(sv, (LPSOCKADDR)&adr, sizeof(adr));
        WSAAsyncSelect(sv, hWnd, WM_ASYNC, FD_ACCEPT);

        if (listen(sv, 5) == -1) return 0;
        flag = 1;
        Winner = 0;
        new_brush = CreateSolidBrush(RGB(0, 0, 0));
        break;
    case WM_ASYNC:
        switch (lParam) {
        case FD_ACCEPT:
            size = sizeof(clt_adr);
            clt = accept(sv, (LPSOCKADDR)&clt, &size);
            WSAAsyncSelect(clt, hWnd, WM_ASYNC, FD_READ);
            break;
        case FD_READ:
            msgLen = recv(clt, buff, 100, 0);
            sscanf(buff, "%d %d %d", &x, &y, &Winner);
            if (x < 22 && y < 22 && isCheck[x][y] == 0) {
                isCheck[x][y] =2;
                if (Winner == 1) {
                    MessageBox(hWnd, _T("백돌이 승리 했습니다."), _T("승리"), MB_OK);
                    DestroyWindow(hWnd);
                }
            }
            flag = 1;
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
            hdc = BeginPaint(hWnd, &ps);
            for (int i = 0; i < 21; i = i++)
            {
                for (int j = 0; j < 21; j++) {
                    Rectangle(hdc, i * BOX_SIZE, j * BOX_SIZE, (i + 1) * BOX_SIZE, (j + 1) * BOX_SIZE);
                    if (isCheck[i][j] == 1) {
                        orginal_brush = (HBRUSH)SelectObject(hdc, new_brush);
                        Ellipse(hdc, (i * BOX_SIZE) + 3, (j * BOX_SIZE) + 3, ((i + 1) * BOX_SIZE) - 3, ((j + 1) * BOX_SIZE) - 3);
                        new_brush = (HBRUSH)SelectObject(hdc, orginal_brush);
                    }
                    if (isCheck[i][j] == 2) {
                        Ellipse(hdc, (i * BOX_SIZE) + 3, (j * BOX_SIZE) + 3, ((i + 1) * BOX_SIZE) - 3, ((j + 1) * BOX_SIZE) - 3);
                    }
                }
            }
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_LBUTTONDOWN:
        x = LOWORD(lParam) / BOX_SIZE;
        y = HIWORD(lParam) / BOX_SIZE;
        if (flag == 1) {
            if (x < 22 && y < 22 && isCheck[x][y] == 0) {
                isCheck[x][y] = 1;
                if (five_check(x, y, 1))
                {
                    if (flag) {
                        MessageBox(hWnd, _T("흑돌이 승리 했습니다."), _T("승리"), MB_OK);
                        Winner = 1;
                    }
                    DestroyWindow(hWnd);
                }
                sprintf_s(buff, "%d %d %d", x, y, Winner);
                send(clt, buff, strlen(buff) + 1, NULL);
                flag = 0;
            }
        }
        InvalidateRgn(hWnd, NULL, true);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

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
