#ifndef HOOKING_H
#define HOOKING_H

#include <windows.h>

extern uintptr_t g_LoadLibraryA_JMP;

uintptr_t WINAPI jumphook(uintptr_t AddressToPerformJump, uintptr_t AddressOfMyFunction, size_t LengthOfTheAreaToPerformTheJump);

HMODULE WINAPI hLoadLibraryA(LPCSTR lpLibFileName);

void ZPerformHooks();

#endif // HOOKING_H
