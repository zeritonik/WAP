#include "WaaWindow.h"

void WaaWindow::waaCycle(bool* running, HDC hdc)
{
	HBRUSH brush = CreateSolidBrush(RGB(100, 200, 150));
	int x, y;
	while (*running) {
		x = rand() % 1720;
		y = rand() % 880;
		RECT rect = {x, y, x + 200, y + 200};
		FillRect(hdc, &rect, brush);
		Sleep(200);
	}

	DeleteObject(brush);
}