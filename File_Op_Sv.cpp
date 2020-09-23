// Project1.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Project1.h"
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
    LoadStringW(hInstance, IDC_PROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PROJECT1));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROJECT1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PROJECT1);
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
//파일 열기 함수
void OutFromFile(TCHAR filename[], HWND hwnd) {
    FILE* fptr;
    //파일이 유니코드인지 멀티바이트 코드 확인
#ifdef _UNICODE
    _tfopen_s(&fptr, filename, _T("r,ccs=UNICODE"));
#else
    _tfopen_s(&fptr, filename, _T("r"));
#endif
    while (_fgetts(buffer[line], 500, fptr) != NULL) {
        if (buffer[line][_tcslen(buffer[line]) - 1] == (TCHAR)("\n"))
            buffer[line][_tcslen(buffer[line]) - 1] = NULL;
        line++;
    }
    fclose(fptr);
}

//파일 저장 함수
void SaveFile(TCHAR filename[]) {
    FILE* fptr;

    // 저장 locale 위치 지정
    _tsetlocale(LC_ALL, _T("korean"));

    // 저장할 파일 지정
    _tfopen_s(&fptr, filename, _T("w"));

    for (int i = 0; i <= line; i++) {
        _fputts(buffer[i], fptr);
        _fputts(_T("\n"), fptr);
    }
    fclose(fptr);
}




LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //열기 대화상자 선언
    OPENFILENAME ofn;


    //저장 대화상자 선언
    OPENFILENAME sfn;

    //폰트 대화상자 선언
    CHOOSEFONT font;

    //도형 그리기 선언
    static BOOL isDrag;
    static int startX, startY, endX, endY, oldX, oldY;
    static int obj_mode;

    //색상 지정상자 선언
    static COLORREF temp[16], retColor;
    CHOOSECOLOR COLOR;

    //브러쉬
    HBRUSH hBrush, oldBrush;

    static COLORREF fcolor;
    HFONT hFont, oldFont;
    static LOGFONT logFont;

    static TCHAR lpstrfile[100] = _T(""); //파일 저장할 버퍼
    TCHAR filter[] = _T("API 소스와 헤더\0*.cpp\0모든 파일\0*.*\0");

    int n = 0;

    HDC hdc;

    //글자 입력
    static SIZE size;

    //POPUP MENU
    HMENU hMenu, hPopup;

    //메뉴 수정
    static HMENU hSubMenu;


    switch (message)
    {
    case WM_CONTEXTMENU:
        hMenu = LoadMenu(hInst, MAKEINTRESOURCE(IDR_POPUP_MENU));
        hPopup = GetSubMenu(hMenu, 0);

        TrackPopupMenu(hPopup, TPM_RIGHTBUTTON, LOWORD(lParam), HIWORD(lParam),0,hWnd,NULL);
        DestroyMenu(hPopup);
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
        case IDM_FILE_OPEN:
            memset(&ofn, 0, sizeof(OPENFILENAME));
            ofn.lStructSize = sizeof(OPENFILENAME);
            ofn.hwndOwner = hWnd;
            ofn.lpstrFilter = filter;
            ofn.lpstrInitialDir = _T("D:\\");
            ofn.lpstrFile = lpstrfile;
            ofn.nMaxFile = 100;

            if (GetOpenFileName(&ofn) != 0) {
                OutFromFile(ofn.lpstrFile, hWnd);
                InvalidateRgn(hWnd, NULL, true);
                EnableMenuItem(hSubMenu, IDM_FILE_OPEN,MF_GRAYED );
                EnableMenuItem(hSubMenu, IDM_FILE_SAVE, MF_ENABLED);
            }
            else {
                MessageBox(hWnd, _T("파일 열기 실패"), _T("에러"), MB_OK);
            }
            break;
        case IDM_FILE_SAVE:
            memset(&sfn, 0, sizeof(OPENFILENAME));
            sfn.lStructSize = sizeof(OPENFILENAME);
            sfn.hwndOwner = hWnd;
            sfn.lpstrFilter = filter;
            sfn.lpstrInitialDir = _T("D:\\");
            sfn.lpstrFile = lpstrfile;
            sfn.nMaxFile = 100;

            if (GetSaveFileName(&sfn) != 0) {
                SaveFile(sfn.lpstrFile);
                EnableMenuItem(hSubMenu, IDM_FILE_OPEN, MF_ENABLED);
                EnableMenuItem(hSubMenu, IDM_FILE_SAVE, MF_GRAYED);
            }
            else {
                MessageBox(hWnd, _T("파일 저장 실패"), _T("에러"), MB_OK);
            }
            break;
        case IDM_DATA_INPUT:
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
        case IDM_FONT_COLOR:
            memset(&font, 0, sizeof(CHOOSEFONT));
            font.lStructSize = sizeof(CHOOSEFONT);
            font.hwndOwner = hWnd;
            font.lpLogFont = &logFont;
            font.Flags = CF_EFFECTS | CF_SCREENFONTS;
            if (ChooseFont(&font) != 0) {
                fcolor = font.rgbColors;
                InvalidateRgn(hWnd, NULL, true);
            }
            break;
        case IDM_FACE_COLOR:
            for (n = 0; n < 16; n++)
                temp[n] = RGB(rand() % 256, rand() % 256, rand() % 256);
            memset(&COLOR, 0, sizeof(CHOOSECOLOR));
            COLOR.lStructSize = sizeof(CHOOSECOLOR);
            COLOR.hwndOwner = hWnd;
            COLOR.lpCustColors = temp;
            COLOR.Flags = CC_FULLOPEN;
            if (ChooseColor(&COLOR) != 0) {
                retColor = COLOR.rgbResult;
            }
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_CREATE:
        hMenu = GetMenu(hWnd);
        hSubMenu = GetSubMenu(hMenu, 0);
        
        EnableMenuItem(hSubMenu, IDM_FILE_OPEN, MF_ENABLED);
        EnableMenuItem(hSubMenu, IDM_FILE_SAVE, MF_GRAYED);
        
        CreateCaret(hWnd, NULL, 2, 15);
        ShowCaret(hWnd);
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        hdc = BeginPaint(hWnd, &ps);

        //폰트 생성
        hFont = CreateFontIndirect(&logFont);

        //생성한 폰트 선택
        oldFont = (HFONT)SelectObject(hdc, hFont);

        //색상 지정
        SetTextColor(hdc, fcolor);
        for (n = 0; n < line; n++) {
            TextOut(hdc, 0, n * 20, buffer[n], _tcslen(buffer[n]));
        }
        GetTextExtentPoint(hdc, buffer[line], _tcslen(buffer[line]), &size);
        SetCaretPos(size.cx, n * 20);
        TextOut(hdc, 0, n * 20, buffer[n], _tcslen(buffer[n]));
        SelectObject(hdc, oldFont);
        DeleteObject(hFont);
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_CHAR:
        if (wParam == VK_BACK){
            if (count > 0) {
                count--;
                buffer[line][count] = NULL;
            }
            else if (count == 0 && line > 0) {
                line--;
                count = _tcslen(buffer[line]);
            }
         }
        else if (wParam == VK_RETURN) {
            count=0;
            line++;
        }
        else {
            buffer[line][count] = wParam;
            buffer[line][++count] = NULL;
        }
        InvalidateRgn(hWnd, NULL, true);
        break;
    case WM_LBUTTONDOWN:
        startX = oldX = LOWORD(lParam);
        startY = oldY = HIWORD(lParam);
        isDrag = true;
        break;
    case WM_LBUTTONUP:
        isDrag = false;
        break;
    case WM_MOUSEMOVE:
        hdc = GetDC(hWnd);
        hBrush = CreateSolidBrush(retColor);
        oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
        endX = LOWORD(lParam);
        endY = HIWORD(lParam);
        if(isDrag) {
            switch (obj_mode) {
            case LINE:
                MoveToEx(hdc, startX, startY, NULL);
                LineTo(hdc, oldX, oldY);
                MoveToEx(hdc, startX, startY, NULL);
                LineTo(hdc,endX, endY);
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
        SelectObject(hdc, oldBrush);
        DeleteObject(hBrush);
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
