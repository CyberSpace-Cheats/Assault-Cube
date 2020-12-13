#include "includes.h"
#include "Menu.h"
#include "Hack.h"

using namespace Menu;

extern bool b_menu;

bool init = true;

extern int windowWidth;
extern int windowHeight;

int _centerX;
int _centerY;

WNDPROC g_oriWndProc = NULL;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

BOOL __stdcall hkGetCursorPos(LPPOINT lpPoint);
BOOL __stdcall hkSetCursorPos(int X, int Y);

LRESULT CALLBACK hWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM dwNewLong)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, dwNewLong) && b_menu)
	{
		return 1l;
	}
	
	if (b_menu)
		return 1l;

	return CallWindowProc(g_oriWndProc, hWnd, uMsg, wParam, dwNewLong);
}

BOOL __stdcall hwglSwapBuffers(HDC hDc)
{
	if (init)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.IniFilename = nullptr;
		io.LogFilename = nullptr;

		io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

		ImGui::StyleColorsDark();

		g_oriWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(FindWindowA(NULL, "AssaultCube"), GWLP_WNDPROC, LONG_PTR(hWndProc)));

		ImGui_ImplWin32_Init(FindWindowA(NULL, "AssaultCube"));
		ImGui_ImplOpenGL2_Init();

		SendLog("CyberSpace", "OpenGL hookado!", 1);

		init = false;
	}

	ImGuiIO& io = ImGui::GetIO(); (void)io;

	if (b_menu)
	{
		io.MouseDrawCursor = true;
	}
	else
		io.MouseDrawCursor = false;

	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGuiStyle& style = ImGui::GetStyle();

	style.ScrollbarSize = 9.0f;
	style.WindowRounding = 1.0f;
	style.ChildRounding = 1.0f;
	style.PopupRounding = 1.0f;
	style.FrameRounding = 1.0f;
	style.ScrollbarRounding = 1.0f;
	style.GrabRounding = 1.0f;
	style.TabRounding = 1.0f;
	style.AntiAliasedLines = true;

	Hack::Loop();

	DrawMenu();

	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	return owglSwapBuffers(hDc);
}

void Detours::Hook()
{
	const auto opengl32 = GetModuleHandleA("opengl32.dll");
	if (opengl32 > 0)
	{
		uintptr_t AddressToHook = reinterpret_cast<uintptr_t>(GetProcAddress(opengl32, "wglSwapBuffers"));
		owglSwapBuffers = (twglSwapBuffers)AddressToHook;

		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)owglSwapBuffers, hwglSwapBuffers);
		DetourTransactionCommit();
	}
}

void Detours::UnHook()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&(PVOID&)owglSwapBuffers, hwglSwapBuffers);
	DetourTransactionCommit();

	SendLog("CyberSpace", "OpenGL restaurado!", 1);
}

auto oGetCursorPos		= reinterpret_cast<decltype(GetCursorPos)*>(GetCursorPos);
auto oSetCursorPos		= reinterpret_cast<decltype(SetCursorPos)*>(SetCursorPos);

MODULEINFO GetModuleInfo(const char* szModuleName)
{
	MODULEINFO modInfo = { 0 };
	auto szMod = GetModuleHandleA(szModuleName);
	if (!szMod)
		return modInfo;

	GetModuleInformation(GetCurrentProcess(), szMod, &modInfo, sizeof(modInfo));
	return modInfo;
}

BOOL __stdcall hkGetCursorPos(LPPOINT lpPoint)
{
	auto returnAddr = reinterpret_cast<uintptr_t>(_ReturnAddress());

	MODULEINFO modInfo = GetModuleInfo("cyberspace.dll");

	DWORD base = (DWORD)modInfo.lpBaseOfDll;
	DWORD size = (DWORD)modInfo.SizeOfImage;

	if (b_menu)
	{
		if (returnAddr >= base && returnAddr < (base + size))
		{
			return oGetCursorPos(lpPoint);
		}

		lpPoint->x = _centerX;
		lpPoint->y = _centerY;

		return true;
	}

	return oGetCursorPos(lpPoint);
}

BOOL __stdcall hkSetCursorPos(int X, int Y)
{
	auto returnAddr = reinterpret_cast<uintptr_t>(_ReturnAddress());

	MODULEINFO modInfo = GetModuleInfo("cyberspace.dll");

	DWORD base = (DWORD)modInfo.lpBaseOfDll;
	DWORD size = (DWORD)modInfo.SizeOfImage;

	if (b_menu)
	{

		if (returnAddr >= base && returnAddr < (base + size))
		{
			return oSetCursorPos(X, Y);
		}

		return true;
	}

	_centerX = X;
	_centerY = Y;

	return oSetCursorPos(X, Y);
}

void Detours::MouseHook()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)oGetCursorPos, hkGetCursorPos);
	DetourAttach(&(PVOID&)oSetCursorPos, hkSetCursorPos);
	DetourTransactionCommit();

	SendLog("CyberSpace", "Mouse hookado!", 1);
}

void Detours::ReleaseMouseHook()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&(PVOID&)oGetCursorPos, hkGetCursorPos);
	DetourDetach(&(PVOID&)oSetCursorPos, hkSetCursorPos);
	DetourTransactionCommit();

	SendLog("CyberSpace", "Mouse restaurado!", 1);
}