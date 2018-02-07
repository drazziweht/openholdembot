#pragma once
//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose:
//
//******************************************************************************

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"	   // main symbols

#include "..\..\Reference Mouse DLL\mousedll.h"
#include "..\..\Reference Keyboard DLL\keyboarddll.h"


class COpenHoldemApp : public CWinApp {
 public:
	COpenHoldemApp();
	~COpenHoldemApp();
	void MyLoadStdProfileSettings(UINT nMaxMRU);
	virtual BOOL InitInstance();
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
 public:
	void StoreLastRecentlyUsedFileList();
 private:
	void LoadLastRecentlyUsedFileList();
	void OpenLastRecentlyUsedFile();
 public:
   //!!!!!remove
	mouse_process_message_t	_dll_mouse_process_message;
	mouse_click_t           _dll_mouse_click;
	mouse_clickdrag_t       _dll_mouse_click_drag;
	keyboard_process_message_t _dll_keyboard_process_message;
	keyboard_sendstring_t   _dll_keyboard_sendstring;
	keyboard_sendkey_t      _dll_keyboard_sendkey;
	HMODULE	_mouse_dll;
	HMODULE	_keyboard_dll;
public:
  CSessionCounter*  SessionCounter() { return &_sessioncounter; }
  CSharedMem*       SharedMem()      { return &_shared_mem; }
  CWatchdog*        Watchdog()       { return _watchdog; }
  CHeartBeatThread* HeartBeatThread(); //!!!!!
 private:
	void FinishInitialization();
  void InitializeThreads();
private:
  CSessionCounter _sessioncounter;
  CSharedMem _shared_mem;
  CWatchdog _watchdog;
  //!!!!!CHeartBeatThread
};

COpenHoldemApp* OpenHoldem(); //!!!!!

extern COpenHoldemApp theApp;//!!!!!

