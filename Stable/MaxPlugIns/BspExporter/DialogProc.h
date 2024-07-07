#ifndef _DIALOGPROC_H
#define _DIALOGPROC_H

#include "windows.h"

INT_PTR CALLBACK RBSExportDlgProc(HWND hWnd, UINT msg,WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK AboutBoxDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

extern int g_nTreeDepth;
extern bool g_bExportRBS,g_bExportXML,g_bExportObjects;

#endif