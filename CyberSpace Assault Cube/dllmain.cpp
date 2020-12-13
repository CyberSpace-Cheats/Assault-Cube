#include "includes.h"
#include "Hack.h"

bool b_menu = false;

int windowWidth;
int windowHeight;

void MainThread()
{
    SendLog("CyberSpace", "Loaded...");

    Hack::Init();

    Detours::Hook();

    Detours::MouseHook();

    while (true)
    {
        RECT size{ 0 };

        GetWindowRect(FindWindowA(NULL, "AssaultCube"), &size);

        windowWidth = size.right - size.left;
        windowHeight = size.bottom - size.top;

        int fullscreen = *(int*)(0x510CB0);

        if (fullscreen == 0)
        {
            windowHeight -= 29;
            windowWidth -= 5;
        }

        if (GetAsyncKeyState(VK_INSERT) & 1)
            b_menu = !b_menu;

        Sleep(1);
    }
}

void OpenConsole()
{
    AllocConsole();
    SetConsoleTitleA("CyberSpace - Assault Cube");
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hModule);
        // OpenConsole();
        const auto thread = CreateThread(0, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(MainThread), 0, 0, 0);
        if (thread)
            CloseHandle(thread);
    }
    return TRUE;
}