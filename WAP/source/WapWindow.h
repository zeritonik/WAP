#pragma once

#include <windows.h>


class WapWindow
{
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

	virtual void waaCycle(bool*, HDC) = 0;

	/* messages ---*/
	virtual inline int onMouseMove() = 0;
	virtual inline int onMouseDown() = 0;
	virtual inline int onMouseUp() = 0;

	/*--- messages */
protected:
	bool running;
public:
	/* static ---*/
	static bool checkWindowClass() { return wc_atom != NULL; }
	/*--- static */


	WapWindow();
	virtual ~WapWindow();
	void startWaa();

	inline HWND getHWND() const { return hwnd; };
	inline HDC getHDC() const { return hdc; };

	inline bool checkWindow() const { return hwnd != NULL; };
	inline bool checkDC() const { return hdc != NULL; };
};