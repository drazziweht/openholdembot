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

#include "resource.h"	  

#include "CAutoConnector.h"
#include "CHeartBeatThread.h"
#include "CSessionCounter.h"
#include "CSharedMem.h"
#include "CWatchdog.h"
#include "CTablePositioner.h"
#include "..\DLLs\Formula_DLL\CFormulaParser.h"

class CTableMapLoader;

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
  CAutoConnector*   AutoConnector()   { return &_autoconnector; }
  CFormulaParser*   FormulaParser()   { return &_formula_parser; }
  CHeartbeatThread* HeartBeatThread(); //!!!!!
  CSessionCounter*  SessionCounter()  { return &_sessioncounter; }
  CSharedMem*       SharedMem()       { return &_shared_mem; }
  CTableMapLoader*  TableMapLoader();
  CTablePositioner* TablePositioner() { return &_table_positioner; }
  CWatchdog*        WatchDog()        { return &_watchdog; }
 private:
	void FinishInitialization();
  void InitializeThreads();
private:
  CSessionCounter _sessioncounter;
  CSharedMem _shared_mem;
  CWatchdog _watchdog; //!!!!! -> hearbeat?
  CFormulaParser _formula_parser;
  CAutoConnector _autoconnector;
  CTablePositioner _table_positioner;//!!!!! -> hearbeat?
  //!!!!!CHeartBeatThread
};

COpenHoldemApp* OpenHoldem(); //!!!!!

extern COpenHoldemApp theApp;//!!!!!

