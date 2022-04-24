#include "types.h"

#include "WapWindow.h"
#include "window_funcs.h"

#include "logers.h"
#include <thread>


// STATIC ->

const char* WapWindow::w_name = "WAP window";
const char* WapWindow::wc_name = "WAP class";
const char* WapWindow::nid_tip = "Click to quit WAP";

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

	case UM_WAPNOTIFY:
		switch (lparam)
		{
		case WM_LBUTTONUP:
			DestroyWindow(hwnd);
			return 0;
		default:
			return 0;
		}

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
		return wap_w->onMouseDown();
	case WM_MBUTTONUP:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		return wap_w->onMouseUp();

	default:
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
}


void WapWindow::resetWallpaper()
{
	char* wallp_path = new char[MAX_PATH];
	SystemParametersInfoA(SPI_GETDESKWALLPAPER, MAX_PATH, wallp_path, NULL);
	SystemParametersInfoA(SPI_SETDESKWALLPAPER, NULL, wallp_path, SPIF_UPDATEINIFILE);
	delete[] wallp_path;
}

// <- STATIC



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
	HWND worker_w = getWorkerW();

	#if defined(_DEBUG)
	if (!checkWindow())
		WAP_ERROR("Error creating window");
	if (!checkDC())
		WAP_ERROR("Error getting DC");
	#endif

	SetParent(hwnd, worker_w);
	SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

	nid.cbSize = sizeof(nid);
	nid.hWnd = hwnd;
	nid.uVersion = NOTIFYICON_VERSION;

	nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	nid.uCallbackMessage = UM_WAPNOTIFY;
	nid.hIcon = LoadIcon(GetModuleHandle(NULL), (LPCWSTR)WAP_ICON);
	WAP_INFO("Loaded icon");
	strcpy_s<sizeof(nid.szTip) / sizeof(char)>(nid.szTip, WapWindow::nid_tip);

	WAP_SPECIALINFO("Created window");
}

WapWindow::~WapWindow()
{
	Shell_NotifyIconA(NIM_DELETE, &nid);
	WAP_INFO("Deleted notify icon");

	DeleteObject(nid.hIcon);
	WAP_INFO("Deleted icon");

	WAP_WARN("Deleted window");
}


void WapWindow::startWaa()
{
	running = true;

	ShowWindow(hwnd, SW_SHOWMAXIMIZED);
	Shell_NotifyIconA(NIM_ADD, &nid);
	WAP_INFO("Showed notify icon");

	std::thread waa_thread(&WapWindow::waaCycle, this, &running, hdc);
	WAP_SPECIALINFO("Started WAA");

	WAP_INFO("Started msg cycle");
	MSG msg;
	while (GetMessage(&msg, 0, 0, 0) != 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	running = false;
	WAP_WARN("Closing WAP window");

	waa_thread.join();
	WAP_SPECIALINFO("Finished WAA");

	WapWindow::resetWallpaper();
	WAP_INFO("Reset wallpaper");
}