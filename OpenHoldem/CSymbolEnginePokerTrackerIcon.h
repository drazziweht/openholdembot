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
 private:
  double LookupStat(CString symbol, int chair);
 private:
  void ClearAllStats();
  void ComputeIcon(int chair);
  int PokerTrackerIcon(int chair);
  bool PlayerMatchesIconDefinition(int chair, int icon);
 private:
	int _precomputed_icon[kMaxNumberOfPlayers];
  // stats for current player
  // to be looked up once, but used multiple times
};

extern CSymbolEnginePokerTrackerIcon *p_symbol_engine_pokertracker_icon;

#endif INC_CSYMBOLENGINEPOKERTRACKERICON_H