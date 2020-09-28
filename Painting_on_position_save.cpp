// hw_0928.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "hw_0928.h"
#include "resource.h"
#include <commdlg.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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
    LoadStringW(hInstance, IDC_HW0928, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_HW0928));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_HW0928));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_HW0928);
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

TCHAR buffer[500][500];
int line = 0, count = 0;

#define FEN         1
#define BRUSH       2
#define LINE        1
#define ELLIPSE     2  
#define RECTANGLE   3




LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //폰트 대화상자 선언
    CHOOSEFONT font;

    //도형 그리기 선언
    static BOOL isDrag;
    static int startX, startY, endX, endY, oldX, oldY;
    static int obj_mode;

    int n = 0;

    HDC hdc;
    //라인 저장
    static int line_StartX[50];
    static int line_StartY[50];
    static int line_endX[50];
    static int line_endY[50];
    static int lineCount = 0;
    static int maxLine = 0;

    //사각형 저장
    static int rect_StartX[50];
    static int rect_StartY[50];
    static int rect_endX[50];
    static int rect_endY[50];
    static int rectCount = 0;
    static int maxRect = 0;

    //타원 저장
    static int ell_StartX[50];
    static int ell_StartY[50];
    static int ell_endX[50];
    static int ell_endY[50];
    static int ellCount = 0;
    static int maxEll = 0;

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
        case IDM_LINE:
            obj_mode = LINE;
            break;
        case IDM_ELLIPSE:
            obj_mode = ELLIPSE;
            break;
        case IDM_RETANGLE:
            obj_mode = RECTANGLE;
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_CREATE:
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        hdc = BeginPaint(hWnd, &ps);
        for (int i = 0; i < maxLine; i++) {
            MoveToEx(hdc, line_StartX[i], line_StartY[i], NULL);
            LineTo(hdc, line_endX[i], line_endY[i]);
        }
        for (int i = 0; i < maxRect; i++) {
            Rectangle(hdc, rect_StartX[i], rect_StartY[i], rect_endX[i], rect_endY[i]);
        }
        for (int i = 0; i < maxEll; i++) {
            Ellipse(hdc, ell_StartX[i], ell_StartY[i], ell_endX[i], ell_endY[i]);
        }
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_LBUTTONDOWN:
        startX = oldX = LOWORD(lParam);
        startY = oldY = HIWORD(lParam);
        isDrag = true;
        break;
    case WM_LBUTTONUP:
        if (isDrag) {
            switch (obj_mode) {
            case LINE:
                if (lineCount >= 50) {
                    lineCount = 0;
                }
                line_StartX[lineCount] = startX;
                line_StartY[lineCount] = startY;

                line_endX[lineCount] = endX;
                line_endY[lineCount] = endY;
                lineCount++;
                if(maxLine < 50)
                     maxLine++;
                break;
            case ELLIPSE:  
                if (ellCount >= 50) {
                    ellCount = 0;
                 }
                ell_StartX[ellCount] = startX;
                ell_StartY[ellCount] = startY;

                ell_endX[ellCount] = endX;
                ell_endY[ellCount] = endY;
                ellCount++;
                if (maxEll < 50)
                    maxEll++;
                break;
            case RECTANGLE:
                if (rectCount >= 50) {
                    rectCount = 0;
                }
                rect_StartX[rectCount] = startX;
                rect_StartY[rectCount] = startY;

                rect_endX[rectCount] = endX;
                rect_endY[rectCount] = endY;
                rectCount++;
                if (maxRect < 50)
                    maxRect++;
                break;
            }
        }
        isDrag = false;
        InvalidateRgn(hWnd, NULL, true);
        break;
    case WM_MOUSEMOVE:
        hdc = GetDC(hWnd);
        SetROP2(hdc, R2_XORPEN);
        SelectObject(hdc, (HPEN)GetStockObject(WHITE_PEN));
        endX = LOWORD(lParam);
        endY = HIWORD(lParam);
        if (isDrag) {
            switch (obj_mode) {
            case LINE:
                MoveToEx(hdc, startX, startY, NULL);
                LineTo(hdc, oldX, oldY);
                MoveToEx(hdc, startX, startY, NULL);
                LineTo(hdc, endX, endY);
                break;
            case ELLIPSE:
                Ellipse(hdc, startX, startY, oldX, oldY);
                Ellipse(hdc, startX, startY, endX, endY);
                break;
            case RECTANGLE:
                Rectangle(hdc, startX, startY, oldX, oldY);
                Rectangle(hdc, startX, startY, endX, endY);
                break;
            }
        }
        oldX = endX; oldY = endY;
        ReleaseDC(hWnd, hdc);
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
