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

	static int i = 0;

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

	case WM_LBUTTONUP:
		MessageBox(hWnd, TEXT("마우스 뼤에엠"), TEXT("마우스 뼤에엠"), MB_YESNO);
		break;

	case WM_LBUTTONDOWN:
		hdc = GetDC(hWnd);
		GetClientRect(hWnd, &rect);
		DrawText(hdc, "뺌에에에에ㅔㅔㅔ엠", strlen("뺌에에에에ㅔㅔㅔ엠"),&rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		ReleaseDC(hWnd, hdc);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return LRESULT();
}
