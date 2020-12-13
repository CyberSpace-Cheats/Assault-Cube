#pragma once

#define send(x, ...) printf_s(x, __VA_ARGS__)

static void SendLog(const char* author, const char* message, int color = 0)
{
	uintptr_t modBase = reinterpret_cast<uintptr_t>(GetModuleHandleA(NULL));
	ACPrintF = reinterpret_cast<_ACPrintF>(modBase + 0x6B060);

	char* sFormat = const_cast<char*>("%s:\f%d %s");

	ACPrintF(sFormat, author, color, message);
}