#pragma once

#include <windows.h>


class WapWindow
{
protected:
	enum MouseButton { MOUSE_L = 0, MOUSE_M, MOUSE_R };
private:
	/* static ---*/
	static const char* w_name;
	static const char* wc_name;
	static const char* nid_tip;

	static const WNDCLASSA wc;
	static const ATOM wc_atom;

	static LRESULT CALLBACK WapWindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	static void resetWallpaper();
	/*--- static */

	const HWND hwnd;
	const HDC hdc;

	NOTIFYICONDATAA nid = { 0 };

	virtual void waaCycle(bool*) = 0;

	/* messages ---*/
	virtual inline int onMouseMove(int x, int y) = 0;
	virtual inline int onMouseDown(MouseButton mb, int x, int y) = 0;
	virtual inline int onMouseUp(MouseButton mb, int x, int y) = 0;
	/*--- messages */
protected:
	bool running;
public:
	/* static ---*/
	static bool checkWindowClass() { return wc_atom != NULL; }
	/*--- static */


	WapWindow();
	virtual ~WapWindow();
	void startWaaDesktop();
	void startWaaWindow(HWND parent, int x, int y, int w, int h);

	inline HWND getHWND() const { return hwnd; };
	inline HDC getHDC() const { return hdc; };

	inline bool checkWindow() const { return hwnd != NULL; };
	inline bool checkDC() const { return hdc != NULL; };
};