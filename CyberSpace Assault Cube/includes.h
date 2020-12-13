#pragma once

typedef char* (__cdecl* _ACPrintF)(char* sFormat, ...);
static _ACPrintF ACPrintF = nullptr;

#define _USE_MATH_DEFINES

#include <windows.h>
#include <iostream>
#include <intrin.h>
#include <Psapi.h>
#include <vector>
#include <cmath>
#include <algorithm>
#include <sstream>

#include <gl/GL.h>
#pragma comment(lib, "opengl32.lib")

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_opengl2.h"

#include "Detours/detours.h"
#pragma comment(lib, "Detours/detours.lib")

#include "message.h"
#include "Detours.h"