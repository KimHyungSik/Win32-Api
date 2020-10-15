#include "framework.h"
#include "EditBox2.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK       EditBox_Porc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK       Dialog_(HWND, UINT, WPARAM, LPARAM);

void RollingBall(HWND);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_EDITBOX2, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_EDITBOX2));

    MSG msg;

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_EDITBOX2));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_EDITBOX2);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; 

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
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case ID_DLG_OPEN:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, EditBox_Porc);
                break;
            case IDM_DIALOG_OPEN:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG2), hWnd, Dialog_);
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

BOOL CALLBACK EditBox_Porc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    TCHAR word[100];
    switch (message) {
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDCANCEL:
        case IDOK:
            EndDialog(hDlg, 0);
            break;

        case IDC_BUTTON_COPY:
            GetDlgItemText(hDlg, IDC_EDIT_PRINT, word, 100);
            SetDlgItemText(hDlg, IDC_EDIT_COPY, word);
            break;

        case IDC_RESET:
            SetDlgItemText(hDlg, IDC_EDIT_PRINT, _T("") );
            SetDlgItemText(hDlg, IDC_EDIT_COPY, _T(""));
            break;
        }
        return 1;
    }
    return 0;
}
BOOL first = TRUE;

void RollingBall(HWND hWnd) {
    static POINT pt = { 30,30 };
    static POINT oldpt = { 0,0 };
    static RECT rect;
    static int stepX = 10, stepY = 10;

    HDC hdc;
    GetClientRect(hWnd, &rect);
    hdc = GetDC(hWnd);
    SetROP2(hdc, R2_XORPEN);
    SelectObject(hdc, GetStockObject(WHITE_PEN));
    SelectObject(hdc, GetStockObject(WHITE_PEN));
    if (!first) {
        Ellipse(hdc, oldpt.x - 20, oldpt.y - 20, oldpt.x + 20, oldpt.y + 20);
    }
    else
        first = false;


    pt.x = pt.x + stepX;
    if (pt.x + 20 > rect.right || pt.x - 20 < 20) {
        stepX *= -1;
    }

    pt.y = pt.y + stepY;
    if (pt.y + 20 > rect.bottom || pt.y - 20 < 20) {
        stepY *= -1;
    }

    Ellipse(hdc, pt.x - 20, pt.y - 20, pt.x + 20, pt.y + 20);

    oldpt = pt;
    ReleaseDC(hWnd, hdc);
}


BOOL CALLBACK Dialog_(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    HWND hButton;
    switch (message)
    {
  
    case WM_TIMER:
        RollingBall(hDlg);
        return 1;

    case WM_INITDIALOG:
        hButton = GetDlgItem(hDlg, IDC_BUTTON_PAUSE);
        EnableWindow(hButton, false);
        first = TRUE;
        return 1;


    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDCANCEL2:
        case IDOK2:
            EndDialog(hDlg, 0);
            break;

        case IDCLOSE:
            EndDialog(hDlg, 0);
            break;

        case IDC_BUTTON_START:
            hButton = GetDlgItem(hDlg, IDC_BUTTON_START);
            EnableWindow(hButton, false);
            hButton = GetDlgItem(hDlg, IDC_BUTTON_PAUSE);
            EnableWindow(hButton, true);
            SetTimer(hDlg, 1, 100, NULL);
            break;

        case IDC_BUTTON_PAUSE:
            KillTimer(hDlg, 1);
            hButton = GetDlgItem(hDlg, IDC_BUTTON_PAUSE);
            EnableWindow(hButton, false);
            hButton = GetDlgItem(hDlg, IDC_BUTTON_START);
            EnableWindow(hButton, true);
            break;

        }
        return 1;
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
