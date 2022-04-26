#pragma once

#include "wap.h"

class WaaWindow : public WapWindow
{
private:
	void waaCycle(bool* running);

	inline int onMouseMove() { return 0; };
	inline int onMouseDown() { WAA_INFO("MouseDown"); return 0; }
	inline int onMouseUp() { WAA_INFO("MouseUp"); return 0; };
public:
	inline WaaWindow() : WapWindow() {};
};