// 0923.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "0923.h"
#include "resource.h"
#include <commdlg.h>
#include <locale.h>
#include <stdio.h>

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
    LoadStringW(hInstance, IDC_MY0923, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY0923));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY0923));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY0923);
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

//파일 열기 함수
void OutFromFile(TCHAR filename[], HWND hwnd){
    FILE* fptr;
    //파일이 유니코드인지 멀티바이트 코드 확인
#ifdef _UNICODE
    _tfopen_s(&fptr, filename, _T("r, ccs=UNICODE"));
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
    _tfopen_s(&fptr, filename, _T("r"));

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

    static TCHAR lpstrfile[100] = _T(""); //파일 저장할 버퍼
    TCHAR filter[] = _T("API 소스와 헤더\0*.cpp\0모든 파일\0*.*\0");

    int n = 0;
    
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
            case IDM_FILE_OPEN:
                memset(&ofn, 0, sizeof(OPENFILENAME));
                ofn.lStructSize = sizeof(OPENFILENAME);
                ofn.hwndOwner = hWnd;
                ofn.lpstrFilter = filter;
                ofn.lpstrInitialDir = _T("D:\\");
                ofn.lpstrFile = lpstrfile;
                ofn.nMaxFile = 100;
                
                if (GetOpenFileName(&ofn) != 0){
                    OutFromFile(ofn.lpstrFile, hWnd);
                    InvalidateRgn(hWnd, NULL, true);
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
                }
                else {
                    MessageBox(hWnd, _T("파일 저장 실패"), _T("에러"), MB_OK);
                }
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
            for (int n = 0; n < line; n++) {
                TextOut(hdc, 0, n * 20, buffer[n], _tcslen(buffer[n]));
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
