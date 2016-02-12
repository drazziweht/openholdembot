//*******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*******************************************************************************
//
// Purpose: Various chair symbols, ecept dealerchair, userchair and raischair,
//   which are of special importance and get handled by other symbol-engines.
//
//*******************************************************************************

#ifndef INC_CSYMBOLENGINEBLINDCHAIRS_H
#define INC_CSYMBOLENGINEBLINDCHAIRS_H

#include "CVirtualSymbolEngine.h"

class CSymbolEngineBlindChairs: public CVirtualSymbolEngine {
 public:
	CSymbolEngineBlindChairs();
	~CSymbolEngineBlindChairs();
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
 public:
  int smallblind_chair()       { return _smallblind_chair; }
  int bigblind_chair()         { return _bigblind_chair; }
 private:
  void CalculateSmallBlindChair();
  void CalculateBigBlindChair();
 private:
  int _smallblind_chair;
  int _bigblind_chair;
 private:
  int _nchairs;
};

extern CSymbolEngineBlindChairs *p_symbol_engine_blind_chairs;

#endif INC_CSYMBOLENGINEBLINDCHAIRS_H
