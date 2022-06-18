#include "WaaWindow.h"

int main()
{
	WaaWindow waa(25);

#if not defined(_DEBUG)
	ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif

	waa.startWaaDesktop();

	return 0;
}