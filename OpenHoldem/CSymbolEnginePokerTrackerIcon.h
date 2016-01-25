//*******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*******************************************************************************
//
// Purpose: support for PT_icon that is no longer
//   available in PT4.
//
//*******************************************************************************

#ifndef INC_CSYMBOLENGINEPOKERTRACKERICON_H
#define INC_CSYMBOLENGINEPOKERTRACKERICON_H

#include "CVirtualSymbolEngine.h"

class CSymbolEnginePokerTrackerIcon: public CVirtualSymbolEngine {
 public:
	CSymbolEnginePokerTrackerIcon();
	~CSymbolEnginePokerTrackerIcon();
 public:
	// Mandatory reset-functions
	void InitOnStartup();
	void ResetOnConnection();
	void ResetOnHandreset();
	void ResetOnNewRound();
	void ResetOnMyTurn();
	void ResetOnHeartbeat();
 public:
	// Public accessors	
	bool EvaluateSymbol(const char *name, double *result, bool log = false);
	CString SymbolsProvided();
 public: //!!!!!?????
  int PlayerIcon(const int chair);
 private:
  void ClearAllStats();
 private:
	int _icon[kMaxNumberOfPlayers];
};

extern CSymbolEnginePokerTrackerIcon *p_symbol_engine_pokertracker_icon;

#endif INC_CSYMBOLENGINEPOKERTRACKERICON_H