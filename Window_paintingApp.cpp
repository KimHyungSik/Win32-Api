#include <windows.h>

LRESULT CALLBACK wndMsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPWSTR    lpCmdLine,
	int       nCmdShow)
{
	HWND hwnd;
	MSG msg;
	WNDCLASS wndClass;

	static LPCTSTR szAppName = "EasyText";
	static LPCTSTR szTitlenName = "Hello API";

	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = wndMsgProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = szAppName;

	RegisterClass(&wndClass);


	hwnd = CreateWindow(
		szAppName,
		TEXT("FIRST API START"),
		WS_EX_LAYERED,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL
		, hInstance
		, NULL);
	if (!hwnd) {
		return false;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;

}

LRESULT CALLBACK wndMsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rect;
	POINT MousePoint;

	static int count = 0;
	static int x_pos, y_pos;
	static char x[100], y[100];
	static bool isChecked = false;

	char Scount[256];
	wsprintf(Scount, "%d", count);

	switch (uMsg) {
	case WM_CREATE:
		MessageBox(hWnd, TEXT("메시지 뼤에엠"), TEXT("메시지 뼤에엠"), MB_YESNO);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, 10, 10, TEXT("뺌에에에에ㅔㅔㅔ엠"), strlen("뺌에에에에ㅔㅔㅔ엠"));
		EndPaint(hWnd, &ps);
		break;

	case WM_MOUSEMOVE:
		count++;
		hdc = GetDC(hWnd);
		if (isChecked) {
			MoveToEx(hdc, 200, 200, NULL);
			LineTo(hdc, 500, 500);
			MoveToEx(hdc, x_pos, y_pos, NULL);
			x_pos = LOWORD(lParam);
			y_pos = HIWORD(lParam);
			LineTo(hdc, x_pos, y_pos);
		}
		TextOut(hdc, 200, 10, TEXT(Scount), strlen(Scount));
		ReleaseDC(hWnd, hdc);
		break;

	case WM_LBUTTONUP:
		isChecked = false;
		break;

	case WM_LBUTTONDOWN:
		x_pos = LOWORD(lParam);
		y_pos = HIWORD(lParam);
		isChecked = true;
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return LRESULT();
}
