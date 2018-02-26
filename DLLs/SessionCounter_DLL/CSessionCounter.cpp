//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: providing an unique session-ID for each instance of OpenHoldem
//
//******************************************************************************

#define SESSIONCOUNTER_DLL_EXPORTS

#include "CSessionCounter.h"
#include <afxwin.h>
#include "..\Preferences_DLL\Preferences.h"
#include "..\WindowFunctions_DLL\window_functions.h"
#include "..\..\Shared\MagicNumbers\MagicNumbers.h"

CSessionCounter::CSessionCounter() {	
	_session_id = kUndefined;
	// Try to get a _session_id that is not in use.
	for (int i=0; i<MAX_SESSION_IDS; i++) {
		// Name the mutexes A..Y
		CString mutex_name = CString(Preferences()->mutex_name()) + "_" + CString(char('A' + i));
		hMutex = CreateMutex(0, FALSE, mutex_name);
		if (GetLastError() != ERROR_ALREADY_EXISTS)	{
			_session_id = i;
			return;
		}	else {
			CloseHandle(hMutex);
		}
	}
  // Otherwise: We failed. No ID available.
	MessageBox_Error_Warning("Could not grab a session ID.\n"
		"Too many instances of OpenHoldem\n",
		"SessionCounter Error");
	PostQuitMessage(-1);
}

CSessionCounter::~CSessionCounter() {
	// Release the mutex for our _session_id
	CloseHandle(hMutex);
}

CSessionCounter *p_session_counter = NULL;

SESSIONCOUNTER_DLL_API CSessionCounter *SessionCounter() {
  if (p_session_counter == NULL) {
    p_session_counter = new CSessionCounter;
  }
  assert(p_session_counter != NULL);
  return p_session_counter;
}