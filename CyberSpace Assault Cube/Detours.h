#pragma once

typedef BOOL(__stdcall* twglSwapBuffers)(HDC hDc);
static twglSwapBuffers owglSwapBuffers = nullptr;

namespace Detours
{
	void Hook();
	void UnHook();

	void MouseHook();
	void ReleaseMouseHook();
}