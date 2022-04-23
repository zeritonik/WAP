#include "wap.h"

class WaaWindow : public WapWindow
{
private:
	inline void waaCycle() { ; }

	inline int onMouseMove() { return 0; };
	inline int onMouseDown() { return 0; }
	inline int onMouseUp() { return 0; };

	inline int onKeyDown() { return 0; };
	inline int onKeyUp() { return 0; };
public:
	inline WaaWindow() : WapWindow() {};
};

int main()
{
	WAA_WARN("Start");

	WaaWindow wnd;
	wnd.startWaa();

	return 0;
}