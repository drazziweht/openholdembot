//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: mousedll.cpp : Defines the entry point for the DLL application.
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
#include "mousedll.h"

MOUSEDLL_API int MouseClick(const HWND hwnd, const RECT rect, const MouseButton button, const int clicks,
							const HWND restore_focus, const POINT restore_cursor)
{

	POINT pt = RandomizeClickLocation(rect);
	double fScreenWidth = ::GetSystemMetrics( SM_CXSCREEN )-1;
	double fScreenHeight = ::GetSystemMetrics( SM_CYSCREEN )-1;

	// Translate click point to screen/mouse coords
	ClientToScreen(hwnd, &pt);
	double fx = pt.x*(65535.0f/fScreenWidth);
	double fy = pt.y*(65535.0f/fScreenHeight);

	// Set up the input structure
	INPUT		press[1];

		ZeroMemory(&press[0],sizeof(INPUT));
		press[0].type = INPUT_MOUSE;
		press[0].mi.dx = (LONG) fx;
		press[0].mi.dy = (LONG) fy;

		switch (button)
		{
		case MouseLeft:
			press[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN;
			break;
		case MouseMiddle:
			press[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_MIDDLEDOWN;
			break;
		case MouseRight:
			press[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_RIGHTDOWN;
			break;
		}

	INPUT		release[1];

		ZeroMemory(&release[0],sizeof(INPUT));
		release[0].type = INPUT_MOUSE;
		release[0].mi.dx = (LONG) fx;
		release[0].mi.dy = (LONG) fy;

		switch (button)
		{
		case MouseLeft:
			release[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTUP;
			break;
		case MouseMiddle:
			release[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_MIDDLEUP;
			break;
		case MouseRight:
			release[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_RIGHTUP;
			break;
		}

	// Send input
	CheckAndReleaseMouseButtons( fx, fy);
	
	for (int i = 0; i<clicks; i++)
	{
		SendInput(1, press, sizeof(INPUT));
		Sleep(12);
		SendInput(1, release, sizeof(INPUT));
		Sleep(68);
	}
		
	Sleep(150);

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

MOUSEDLL_API int MouseClickDrag(const HWND hwnd, const RECT rect, const HWND restore_focus, const POINT restore_cursor)
{


	POINT			pt;
	double			fx, fy;

	double fScreenWidth = ::GetSystemMetrics( SM_CXSCREEN )-1;
	double fScreenHeight = ::GetSystemMetrics( SM_CYSCREEN )-1;

	// Set up the input structure
	// left click, drag to right, un-left click
	pt.x = rect.left;
	pt.y = rect.top + (rect.bottom - rect.top)/2;
	ClientToScreen(hwnd, &pt);
	fx = pt.x*(65535.0f/fScreenWidth);
	fy = pt.y*(65535.0f/fScreenHeight);

	CheckAndReleaseMouseButtons( fx, fy);

	INPUT			input[3];
	ZeroMemory(&input[0],sizeof(INPUT));
	input[0].type = INPUT_MOUSE;
	input[0].mi.dx = (LONG) fx;
	input[0].mi.dy = (LONG) fy;
	input[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN;

	pt.x = rect.right;
	pt.y = rect.top + (rect.bottom - rect.top)/2;
	ClientToScreen(hwnd, &pt);
	fx = pt.x*(65535.0f/fScreenWidth);
	fy = pt.y*(65535.0f/fScreenHeight);

	ZeroMemory(&input[1],sizeof(INPUT));
	input[1].type = INPUT_MOUSE;
	input[1].mi.dx = (LONG) fx;
	input[1].mi.dy = (LONG) fy;
	input[1].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;

	ZeroMemory(&input[2],sizeof(INPUT));
	input[2].type = INPUT_MOUSE;
	input[2].mi.dwFlags = MOUSEEVENTF_LEFTUP;


//	SetForegroundWindow(hwnd);

	// Send input
	SendInput(3, input, sizeof(INPUT));
	Sleep(200);

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

MOUSEDLL_API void ProcessMessage(const char *message, const void *param)
{
	if (message==NULL)  return;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return true;
}

const POINT RandomizeClickLocation(const RECT rect) 
{
	POINT p = {0};
	
	// normal random distribution - much better than uniform random distribution !
	GetClickPoint(rect.left + (rect.right-rect.left)/2, 
				  rect.top + (rect.bottom-rect.top)/2, 
				  (rect.right-rect.left)/2, 
				  (rect.bottom-rect.top)/2, 
				  &p);

	return p;
}

const void GetClickPoint(const int x, const int y, const int rx, const int ry, POINT *p) 
{
	p->x = x + (int) (RandomNormalScaled(2*rx, 0, 1) + 0.5) - (rx);
	p->y = y + (int) (RandomNormalScaled(2*ry, 0, 1) + 0.5) - (ry);
}

// random number - 0 -> scale, with normal distribution
// ignore results outside 3.5 stds from the mean
const double RandomNormalScaled(const double scale, const double m, const double s) 
{
	double res = -99;
	while (res < -3.5 || res > 3.5) res = RandomNormal(m, s);
	return (res / 3.5*s + 1) * (scale / 2.0);
}

const double RandomNormal(const double m, const double s) 
{
	/* mean m, standard deviation s */
	double x1 = 0., x2 = 0., w = 0., y1 = 0., y2 = 0.;

	do {
		x1 = 2.0 * ((double) rand()/(double) RAND_MAX) - 1.0;
		x2 = 2.0 * ((double) rand()/(double) RAND_MAX) - 1.0;
		w = x1 * x1 + x2 * x2;
	} while ( w >= 1.0 );

	w = sqrt( (-2.0 * log( w ) ) / w );
	y1 = x1 * w;
	y2 = x2 * w;

	return( m + y1 * s ); 

} 

void CheckAndReleaseMouseButtons(double screen_x, double screen_y)
{
	// Release all mouse buttons if one of them is pressed.
	// Should solve a rare stuck mouse button bug, similar to stuck key problem.

	if ((GetKeyState(VK_LBUTTON) & 0x80)||
		(GetKeyState(VK_MBUTTON) & 0x80)||
		(GetKeyState(VK_RBUTTON) & 0x80))
	{
		INPUT			input[1];		
		ZeroMemory(&input[0],sizeof(INPUT));
		input[0].type = INPUT_MOUSE;
		input[0].mi.dx =(LONG) screen_x;
		input[0].mi.dy =(LONG) screen_y;
		input[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTUP | MOUSEEVENTF_RIGHTUP | MOUSEEVENTF_MIDDLEUP ;
		
		SendInput(1, input, sizeof(INPUT));
		Sleep(20);
	}
}