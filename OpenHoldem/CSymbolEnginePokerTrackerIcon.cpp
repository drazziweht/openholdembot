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


#include "stdafx.h"
#include "CSymbolEnginePokerTrackerIcon.h"

#include "CSymbolEnginePokerTracker.h"
#include "StringFunctions.h"

CSymbolEnginePokerTrackerIcon::CSymbolEnginePokerTrackerIcon() {
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
	assert(p_symbol_engine_pokertracker_icon != NULL); 
  ClearAllStats();
}

CSymbolEnginePokerTrackerIcon::~CSymbolEnginePokerTrackerIcon() {
}

void CSymbolEnginePokerTrackerIcon::InitOnStartup() {
}

void CSymbolEnginePokerTrackerIcon::ResetOnConnection() {
}

void CSymbolEnginePokerTrackerIcon::ResetOnHandreset() {
}

void CSymbolEnginePokerTrackerIcon::ResetOnNewRound() {
}

void CSymbolEnginePokerTrackerIcon::ResetOnMyTurn() {
  // Clear computed stats once on my turn
  ClearAllStats();
}

void CSymbolEnginePokerTrackerIcon::ResetOnHeartbeat() {
}

void CSymbolEnginePokerTrackerIcon::ClearAllStats() {
  for (int i=0; i<kMaxNumberOfPlayers; ++i) {
    _icon[i] = kUndefined;
  }
}

bool CSymbolEnginePokerTrackerIcon::EvaluateSymbol(const char *name, double *result, bool log /* = false */) {
  if (memcmp(name, "pt_icon", 7) != 0 ) {
		// Symbol of a different symbol-engine
		return false;
	}
  if (strlen(name) != 8) {
		// Symbol of a different symbol-engine or ivalid pt_icon
		return false;
	}
  //!!!!!
  int chair = RightDigitCharacterToNumber(name);
  return true;
}

CString CSymbolEnginePokerTrackerIcon::SymbolsProvided() {
  CString list;
  list += RangeOfSymbols("pt_icon%i", kFirstChair, kLastChair);
  return list;
}