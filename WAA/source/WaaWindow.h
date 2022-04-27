#pragma once

#include "wap.h"

class WaaWindow : public WapWindow
{
private:
	void waaCycle(bool* running);

	inline int onMouseMove(int x, int y) { return 0; };
	inline int onMouseDown(MouseButton mb, int x, int y) { return 0; };
	inline int onMouseUp(MouseButton mb, int x, int y) { return 0; };
public:
	inline WaaWindow() : WapWindow() {};
};