//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: keyboarddll.cpp : Defines the entry point for the DLL application.
//
//******************************************************************************


#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
#endif

#include <windows.h>
#include <math.h>
#include <atlstr.h>
#include "keyboarddll.h"

const int Shift   = 1;
const int Control = 2;
const int Alt     = 4;

void PlayKeyboardEvent(int vkey, int bscan)
{
	// This should hopefully fix the stuck control key problem.
	keybd_event(VK_CONTROL, 0, (bscan & Control) ? 0 : KEYEVENTF_KEYUP, 0);
	keybd_event(VK_SHIFT,   0, (bscan & Shift)   ? 0 : KEYEVENTF_KEYUP, 0);
	keybd_event(VK_MENU,    0, (bscan & Alt)     ? 0 : KEYEVENTF_KEYUP, 0);

	
	if ((GetKeyState(vkey) & 0x80))
	{
		Sleep(10);
		keybd_event(vkey,  bscan,  KEYEVENTF_KEYUP, 0);
		Sleep(10);
	}

	keybd_event(vkey,  bscan,  0, 0);
	Sleep(10);
	keybd_event(vkey,  bscan,  KEYEVENTF_KEYUP, 0);

	if (bscan & Control)
		keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
	if (bscan & Shift)
		keybd_event(VK_SHIFT,   0, KEYEVENTF_KEYUP, 0);
	if (bscan & Alt)
		keybd_event(VK_MENU,    0, KEYEVENTF_KEYUP, 0);
}

KEYBOARDDLL_API int SendString(const HWND hwnd, const RECT rect, const CString s, const bool use_comma,
	const HWND restore_focus, const POINT restore_cursor)
{

	// Set focus to target window
//	SetForegroundWindow(hwnd);

	// Send each character of the string via PlayKeyboardEvent
	char ch_str[100];
	sprintf_s(ch_str, 100, "%s", s.GetString());

	int	vkey = 0;

	int i = 0, strlength = (int)strlen(ch_str);
	short KeyScan;
	for (int i=0; i<strlength; i++)
	{
		Sleep(32);
		if (use_comma && ch_str[i]=='.')
			ch_str[i] = ',';
		KeyScan = VkKeyScan(ch_str[i]);
		PlayKeyboardEvent(LOBYTE(KeyScan), HIBYTE(KeyScan));
	}
	Sleep(18);

	// Restore focus.
	if (restore_focus!=NULL)
	{
		SetForegroundWindow(restore_focus);
	}

	// Restore cursor.
	if (!(restore_cursor.x==-1234567 && restore_cursor.y==-1234567))
	{
		SetCursorPos(restore_cursor.x, restore_cursor.y);
	}

	return (int) true;
}

KEYBOARDDLL_API int SendKey(const HWND hwnd, const RECT rect, UINT vkey,
							const HWND restore_focus, const POINT restore_cursor)
{
	INPUT			input[2];

	// Add vkey to the input struct
	ZeroMemory(&input[0],sizeof(INPUT));
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = vkey;

	ZeroMemory(&input[1],sizeof(INPUT));
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = vkey;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;

	// Set focus to target window
//	SetForegroundWindow(hwnd);

	// Send input
	SendInput(2, input, sizeof(INPUT));

	// Restore focus.
	if (restore_focus!=NULL)
	{
		SetForegroundWindow(restore_focus);
	}

	// Restore cursor.
	if (!(restore_cursor.x==-1234567 && restore_cursor.y==-1234567))
	{
		SetCursorPos(restore_cursor.x, restore_cursor.y);
	}

	return (int) true;
}

KEYBOARDDLL_API void ProcessMessage(const char *message, const void *param)
{
	if (message==NULL)  return;
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		//MessageBox(NULL, "kbd", "1", 0);
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return true;
}
