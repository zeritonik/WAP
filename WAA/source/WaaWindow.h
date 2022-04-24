#pragma once

#include "wap.h"

class WaaWindow : public WapWindow
{
private:
	void waaCycle(bool* running, HDC hdc);

	inline int onMouseMove() { return 0; };
	inline int onMouseDown() { return 0; }
	inline int onMouseUp() { return 0; };
public:
	inline WaaWindow() : WapWindow() {};
};