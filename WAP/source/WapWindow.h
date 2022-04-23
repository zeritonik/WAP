#pragma once

#include "windows.h"
#include "logers.h"


class WapWindow
{
private:
	const HWND hwnd;
	const HDC hdc;

	virtual void waaCycle() = 0;

	/* messages ---*/
	virtual inline int onMouseMove() = 0;
	virtual inline int onMouseDown() = 0;
	virtual inline int onMouseUp() = 0;

	virtual inline int onKeyDown() = 0;
	virtual inline int onKeyUp() = 0;
	/*--- messages*/
public:
	/* static ---*/
	static const char* w_name;

	static const char* wc_name;
	static const WNDCLASSA wc;
	static const ATOM wc_atom;

	static LRESULT CALLBACK WapWindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	static bool checkWindowClass() { return wc_atom != NULL; }
	/*--- static*/


	WapWindow();
	void startWaa();

	inline bool checkWindow() const { return hwnd != NULL; };
	inline bool checkDC() const { return hdc != NULL; };
};