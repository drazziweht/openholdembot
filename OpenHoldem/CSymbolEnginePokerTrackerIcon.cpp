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
#include "CTableState.h"
#include "StringFunctions.h"

/*
LoosePassiveFish
ExtraLoose
ShowdownMuppet
TightUnknown
WeakTight
SemiLooseAggressiveGambler 
TightPassiveRock
UltraAggressive
LooseAggressiveManiac
SemiLooseUnknown
CallingStation
SemiLoosePassive
LooseUnknown
TightAggressiveTAG
*/

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
  ClearAllStats();
}

void CSymbolEnginePokerTrackerIcon::ResetOnHandreset() {
  // Clear computed stats once on hand-reset.
  // We want to calculate them only once per hand (if necessary).
  ClearAllStats();
}

void CSymbolEnginePokerTrackerIcon::ResetOnNewRound() {
}

void CSymbolEnginePokerTrackerIcon::ResetOnMyTurn() {
}

void CSymbolEnginePokerTrackerIcon::ResetOnHeartbeat() {
}

void CSymbolEnginePokerTrackerIcon::ClearAllStats() {
  for (int i=0; i<kMaxNumberOfPlayers; ++i) {
    _precomputed_icon[i] = kUndefined;
  }
}

void CSymbolEnginePokerTrackerIcon::ComputeIcon(int chair) {

}

int CSymbolEnginePokerTrackerIcon::PokerTrackerIcon(int chair) {
  if (chair < 0) return kUndefined;
  if (chair > kLastChair) return kUndefined;
  // Take care about stale data (game over, player no longer seated, ...)
  if (!p_table_state->Player(chair)->seated()) return kUndefined;
  // Compute icon for chair if needed
  if (_precomputed_icon[chair] == kUndefined) {
    ComputeIcon(chair);
  }
  return _precomputed_icon[chair];
}

bool CSymbolEnginePokerTrackerIcon::EvaluateSymbol(const char *name, double *result, bool log /* = false */) {
  if (memcmp(name, "pt_icon", 7) != 0 ) {
		// Symbol of a different symbol-engine
		return false;
	}
  if (strlen(name) != 8) {
		// Symbol of a different symbol-engine or ivalid pt_icon
    //!!!!! to do constants
		return false;
	}
  int chair = RightDigitCharacterToNumber(name);
  *result = PokerTrackerIcon(chair);
  return true;
}

CString CSymbolEnginePokerTrackerIcon::SymbolsProvided() {
  CString list;
  list += RangeOfSymbols("pt_icon%i", kFirstChair, kLastChair);
  return list;
}