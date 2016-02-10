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

#include "stdafx.h"
#include "CSymbolEngineBetrounds.h"

#include "CBetroundCalculator.h"
#include "MagicNumbers.h"

CSymbolEngineBetrounds *p_symbol_engine_betrounds = NULL;

CSymbolEngineBetrounds::CSymbolEngineBetrounds() {
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
	//
	// This engine does not use any other engines.
}

CSymbolEngineBetrounds::~CSymbolEngineBetrounds() {
}

void CSymbolEngineBetrounds::InitOnStartup() {
}

void CSymbolEngineBetrounds::ResetOnConnection() {
}

void CSymbolEngineBetrounds::ResetOnHandreset() {
}

void CSymbolEngineBetrounds::ResetOnNewRound() {
}

int CSymbolEngineBetrounds::previous_round() {
  int betround = p_betround_calculator->betround();
  if (betround > kBetroundPreflop) {
    return (betround - 1);
  }
  // Otherwise: avoid out-of-range errors
  // and return something "meaningful"
  return kBetroundPreflop;
}

void CSymbolEngineBetrounds::ResetOnMyTurn() {
}

void CSymbolEngineBetrounds::ResetOnHeartbeat() {
}

bool CSymbolEngineBetrounds::EvaluateSymbol(const char *name, double *result, bool log /* = false */) {
  FAST_EXIT_ON_OPENPPL_SYMBOLS(name);
	if (memcmp(name, "betround", 8)==0 && strlen(name)==8) {
    // "betround" got pre-calculated because it is necessary
    // to detect hand-resets and trigger symbol-calculations
		*result = p_betround_calculator->betround();
		return true;
	} else if (memcmp(name, "previousround", 13)==0 && strlen(name)==13) {
		*result = previous_round();
		return true;
  // Below named constants mainly for verbose symbol multiplexing
	} else if (memcmp(name, "preflop", 7)==0 && strlen(name)==7) {
		*result = kBetroundPreflop;
		return true;
	} else if (memcmp(name, "flop", 4)==0 && strlen(name)==4) {
		*result = kBetroundFlop;
		return true;
	} else if (memcmp(name, "turn", 4)==0 && strlen(name)==4) {
		*result = kBetroundTurn;
		return true;
	} else if (memcmp(name, "river", 5)==0 && strlen(name)==5) {
		*result = kBetroundRiver;
		return true;
	}
	// Symbol of a different symbol-engine
	return false;
}

CString CSymbolEngineBetrounds::SymbolsProvided() {
  return "betround previosround preflop flop turn river ";
}
