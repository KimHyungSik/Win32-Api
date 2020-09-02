
#include "framework.h"
#include "dataType1.h"
#include <stdio.h>
#include <vector>

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
    LoadStringW(hInstance, IDC_DATATYPE1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DATATYPE1));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DATATYPE1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_DATATYPE1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//=============================================================
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//=============================================================
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

//=====================================================
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)          
//                                                    
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//=====================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
    PAINTSTRUCT ps;
    HDC hdc;
    TCHAR str[20] = _T("date tpye");
    RECT rect;

    static int start_x_pos, start_y_pos, end_x_pos, end_y_pos;
    static TCHAR keystr[100][20] = { '\0' };
    static int cnt = 0, line = 1;
    static SIZE size;
    static bool isDrag = false, isPaint = false;

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

    case WM_CHAR:
        if (VK_BACK == wParam) {
            if (cnt > 0) {
                keystr[line - 1][cnt - 1] = '\0';
                cnt--;
            }
            else if (cnt == 0 && line > 1) {
                line -= 1;
                cnt = strlen(keystr[line - 1]);
            }
          
        }
        else if (VK_RETURN == wParam) {
            if (line < 20) {
                line += 1;
                cnt = 0;
            }
        }
        else {
            keystr[line-1][cnt] = (TCHAR)wParam;
            keystr[line-1][cnt + 1] = '\0';
            cnt++;
        }
        InvalidateRgn(hWnd, NULL,TRUE);

    case WM_CREATE:
        CreateCaret(hWnd, NULL, 3, 18);
        ShowCaret(hWnd);
        break;

    case WM_KEYDOWN:
        hdc = GetDC(hWnd);

        TextOut(hdc, 50, 50, _T("키보드가 눌렸습니다"),19);
        ReleaseDC(hWnd, hdc);
        break;

    case WM_MOUSEMOVE:
        if (isDrag) {
            end_x_pos = LOWORD(lParam);
            end_y_pos = HIWORD(lParam);
        }
        break;

    case WM_LBUTTONDOWN:
        start_x_pos = LOWORD(lParam);
        start_y_pos = HIWORD(lParam);
        isDrag = true;
        break;

    case WM_LBUTTONUP:
        isDrag = false;
        isPaint = true;
        InvalidateRgn(hWnd, NULL, TRUE);
        break;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            hdc = BeginPaint(hWnd, &ps);
            int sizeType = 0;
            sizeType = sizeof(WCHAR);
            sprintf_s((char*)str,sizeof(str), "cnt : %d , line : %d", cnt, line);

            rect.left = 10;
            rect.right = 200;
            rect.bottom = 100;
            rect.top = 0;
            DrawText(hdc, str, strlen(str), &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

            for (int i = 0; i < line; i++) {
                TextOut(hdc, 10, 100 + (i*20), keystr[i], strlen(keystr[i]));
            }
            GetTextExtentPoint(hdc, keystr[line - 1], _tcslen(keystr[line - 1]), &size);
            SetCaretPos(size.cx+10, 100 + ((line-1) * 20));

            
            
            if (isPaint) {
                Rectangle(hdc, start_x_pos, start_y_pos, end_x_pos, end_y_pos);
                MoveToEx(hdc, start_x_pos, start_y_pos, NULL);
                LineTo(hdc, end_x_pos, end_y_pos);
                Ellipse(hdc, start_x_pos, start_y_pos, end_x_pos, end_y_pos);
                isPaint = false;
            }

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
