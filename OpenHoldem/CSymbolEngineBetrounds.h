//*******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*******************************************************************************
//
// Purpose:
//
//*******************************************************************************

#ifndef INC_CSYMBOLENGINEBETROUNDS_H
#define INC_CSYMBOLENGINEBETROUNDS_H

#include "CVirtualSymbolEngine.h"

class CSymbolEngineBetrounds: public CVirtualSymbolEngine {
 public:
	CSymbolEngineBetrounds();
	~CSymbolEngineBetrounds();
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
  int previous_round();
};

extern CSymbolEngineBetrounds *p_symbol_engine_betrounds;

#endif INC_CSYMBOLENGINEBETROUNDS_H