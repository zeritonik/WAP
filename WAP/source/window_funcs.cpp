#include "window_funcs.h"
#include "logers.h"



static BOOL CALLBACK initWorkerCallback(HWND hwnd, LPARAM lparam)
{
	if (FindWindowExA(hwnd, NULL, "SHELLDLL_DefView", NULL)) {
		*reinterpret_cast<HWND*>(lparam) = hwnd;
		WAP_SPECIALINFO("Found parent worker");
		return false;
	}
	return true;
}

static HWND initWorker()
{
	HWND progman = FindWindowA("Progman", NULL);
	SendMessage(progman, 0x052c, NULL, NULL);

	HWND parent_worker = NULL;
	EnumWindows(initWorkerCallback, reinterpret_cast<LPARAM>(&parent_worker));
	if (parent_worker == NULL) {
		WAP_WARN("Can't find parent worker");
		return NULL;
	}

	HWND worker_w = FindWindowExA(NULL, parent_worker, "WorkerW", NULL);
	return worker_w;
}


HWND getWorkerW()
{
	static HWND worker_w = NULL;

	if (worker_w == NULL) {
		worker_w = initWorker();
	}

	return worker_w;
}



static BOOL CALLBACK getSysListViewCallback(HWND hwnd, LPARAM lparam)
{
	HWND sd_df = FindWindowExA(hwnd, NULL, "SHELLDLL_DefView", NULL);
	if (sd_df) {
		*reinterpret_cast<HWND*>(lparam) = FindWindowExA(sd_df, NULL, "SysListView32", NULL);
		return false;
	}
	return true;
}

HWND getSysListView()
{
	HWND slv = 0;
	EnumWindows(getSysListViewCallback, (LPARAM)&slv);
	return slv;
}