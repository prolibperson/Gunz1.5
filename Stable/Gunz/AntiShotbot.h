#ifndef ANTISHOTBOTLOGGER_H
#define ANTISHOTBOTLOGGER_H

#include <windows.h>

LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam);
void AntiShotbotLogger();

#else
LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam);
void AntiShotbotLogger();
#endif
