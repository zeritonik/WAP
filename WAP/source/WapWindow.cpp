#include "WapWindow.h"
#include <thread>


const char* WapWindow::w_name = "WAP window";
const char* WapWindow::wc_name = "WAP class";

const WNDCLASSA WapWindow::wc = {
	CS_OWNDC | CS_NOCLOSE | CS_DBLCLKS,
	(WNDPROC)WapWindow::WapWindowProc,
	0, 0,
	GetModuleHandle(NULL),
	NULL, NULL,
	NULL,
	NULL,
	"WAP class"
};

const ATOM WapWindow::wc_atom = RegisterClassA(&wc);


LRESULT CALLBACK WapWindow::WapWindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	WapWindow* wap_w = reinterpret_cast<WapWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	switch (msg)
	{
	case WM_CREATE:
		return 0;

	case WM_CLOSE:
		DestroyWindow(hwnd);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT: {
		PAINTSTRUCT ps;
		HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
		HDC hdc = BeginPaint(hwnd, &ps);
		FillRect(hdc, &ps.rcPaint, brush);
		EndPaint(hwnd, &ps);
		DeleteObject(brush);
	}
		return 0;

	case WM_MOUSEMOVE:
		return wap_w->onMouseMove();
	case WM_MBUTTONDOWN:
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_XBUTTONDOWN:
		return wap_w->onMouseDown();
	case WM_MBUTTONUP:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_XBUTTONUP:
		return wap_w->onMouseUp();

	case WM_CHAR:
		return wap_w->onKeyDown();
	case WM_KEYDOWN:
		return wap_w->onKeyDown();
	case WM_KEYUP:
		return wap_w->onKeyUp();

	default:
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
}


WapWindow::WapWindow()
	: hwnd(
		CreateWindowA(WapWindow::wc_name, WapWindow::w_name,
			WS_POPUP,
			0, 0, CW_USEDEFAULT, CW_USEDEFAULT,
			NULL, NULL,
			GetModuleHandle(NULL), NULL)
	), 
	hdc(GetDC(hwnd))
{
	SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

	WAP_INFO("Created window");
	#if defined(_DEBUG)
	if (!checkWindow())
		WAP_ERROR("Error creating window");
	if (!checkDC())
		WAP_ERROR("Error getting DC");
	#endif
}

void WapWindow::startWaa()
{
	ShowWindow(hwnd, SW_SHOWMAXIMIZED);
	std::thread waa_thread(&WapWindow::waaCycle, this);

	WAP_SPECIALINFO("Started WAA");

	MSG msg;
	while (GetMessage(&msg, 0, 0, 0) != 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	waa_thread.join();
}