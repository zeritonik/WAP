#include "hook.h"
#include <string>


#pragma data_seg(".hook_data")

HINSTANCE dll_handle = 0;

HWND hooker = 0;
// hooks
HHOOK mouse_hook = 0;

#pragma data_seg()


BOOL WINAPI DllMain(HINSTANCE hdll, DWORD reason, LPVOID reserved)
{
	dll_handle = hdll;
	return true;
}


LRESULT CALLBACK MouseHookProc(int code, WPARAM wparam, LPARAM lparam)
{
	if (code >= 0) {
		MOUSEHOOKSTRUCT* mhs = reinterpret_cast<MOUSEHOOKSTRUCT*>(lparam);
		SendMessage(hooker, (UINT)wparam, NULL, MAKELPARAM(mhs->pt.x, mhs->pt.y));
	}
	return CallNextHookEx(mouse_hook, code, wparam, lparam);
}


bool setHooks(HWND hooker_w, HWND target)
{
	if (hooker == 0 && mouse_hook == 0) {
		DWORD target_thread = GetWindowThreadProcessId(target, NULL);
		mouse_hook = SetWindowsHookEx(WH_MOUSE, MouseHookProc, dll_handle, target_thread);
		hooker = hooker_w;
		return (bool)mouse_hook;
	}
	return false;
}

bool removeHooks()
{
	hooker = 0;

	UnhookWindowsHookEx(mouse_hook);
	mouse_hook = 0;

	return true;
}