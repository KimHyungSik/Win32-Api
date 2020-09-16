// Omog_1.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Omog_1.h"
#include <iostream>

#define MAX_LOADSTRING 100

#define BOX_SIZE 30

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
static int isCheck[22][22] = { 0, };

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
    LoadStringW(hInstance, IDC_OMOG1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_OMOG1));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_OMOG1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_OMOG1);
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

int five_checkX(int x, int y, char count, bool position, char team) {
    if (count == 5) return count;

    int Tx;

    if (position) Tx = x + 1;
    else  Tx = x - 1 ;

    if (isCheck[Tx][y] == team) five_checkX(Tx, y, count + 1, position, team);
    else return count;
}

int five_checkY(int x, int y, char count, bool position, char team) {
    if (count == 5) return count;

    if (position) y--;
    else y++;

    if (isCheck[x][y] == team) five_checkY(x, y, count + 1, position, team);
    else return count;
}

int five_checkXY1(int x, int y, char count, bool position, char team) {
    if (count == 5) return count;

    if (position) {
        y--;
        x--;
    }
    else {
        y++;
        x++;
    }

    if (isCheck[x][y] == team) five_checkXY1(x, y, count + 1, position, team);
    else return count;
}

int five_checkXY2(int x, int y, char count, bool position, char team) {
    if (count == 5) return count;

    if (position) { y--; x++; }
    else { y++; x--; }

    if (isCheck[x][y] == team) five_checkXY2(x, y, count + 1, position, team);
    else return count;
}


bool five_check(int x, int y, char team) {
    if (five_checkX(x, y, 1, 1, team) + five_checkX(x, y, 1, 0, team) > 5||
        five_checkY(x, y, 1, 1, team) + five_checkY(x, y, 1, 0, team) > 5 ||
        five_checkXY1(x, y, 1, 1, team) + five_checkXY1(x, y, 1, 0, team) > 5 ||
        five_checkXY2(x, y, 1, 1, team) + five_checkXY2(x, y, 1, 0, team) > 5)
        return true;
    return false;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){

   
    static bool turn = 0;
    static HBRUSH orginal_brush, new_brush;
    int x, y;

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
    case WM_CREATE:
        new_brush = CreateSolidBrush(RGB(0, 0, 0));
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
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
        if (x < 22 && y < 22 && isCheck[x][y] == 0) {
            isCheck[x][y] = 1 + turn;
            if (five_check(x, y, turn+1))
            {
                if (!turn) {
                    MessageBox(hWnd, _T("흑돌이 승리 했습니다."), _T("승리"), MB_OK);
                }
                else {
                    MessageBox(hWnd, _T("백돌이 승리 했습니다."), _T("승리"), MB_OK);
                }
                DestroyWindow(hWnd);
            }
            turn = !turn;
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
