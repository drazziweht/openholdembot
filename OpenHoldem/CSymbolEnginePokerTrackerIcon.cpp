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

typedef enum {
  pt_LoosePassiveFish,
  pt_ExtraLoose,
  pt_ShowdownMuppet,
  pt_TightUnknown,
  pt_WeakTight,
  pt_SemiLooseAggressiveGambler,
  pt_TightPassiveRock,
  pt_UltraAggressive,
  pt_LooseAggressiveManiac,
  pt_SemiLooseUnknown,
  pt_CallingStation,
  pt_SemiLoosePassive,
  pt_LooseUnknown,
  pt_TightAggressiveTAG,
  pt_Unknown,
  // always keepe this the very last one
  kNumberOfPokerTrackerIcons,
} enum_pt_icons;

typedef struct {
  CString pt_symbol;
  double  min_value;
  double  max_value;
} t_poker_tracker_symbol_range;

const int kMaxNumberOfRelevantStatsPerIconType = 4;

const t_poker_tracker_symbol_range
  poker_tracker_symbol_range_full_ring[kNumberOfPokerTrackerIcons][kMaxNumberOfRelevantStatsPerIconType] = {
  // 2	LP (Fishy)	9	
  {{"VP$IP", 0.35, 1}, {"Total AF", 0.01, 1.3}},
  // 3	X-Loose (Vp$iP 55%+)	1	
  {{"VP$IP", 0.55, 1}},
  // 4	Showdown Muppet	3	
  {{"W$SD", 0, 0.29}, {"WTSD", 0.15, 1}},
  // 5	T N/unknown	12	
  {{"VP$IP", 0, 0.22}},
  // 6	WT (Weak Tight)	5	
  {{"WTSD", 0, 0.13}},
  // 8	sLA (Gambler)	6	
  {{"VP$IP", 0.22, 0.35}, {"Flop AF", 1.8, 123}, {"Turn AF", 0.1, 123}, {"River AF", 0.1, 123}},
  // 9	TP (Rock)	10	
  {{"VP$IP", 0, 0.22}, {"Total AF", 0.01, 1.3}},
  // 7	Ultra Aggressor	4	
  {{"Flop AF", 2.75, 123}, {"Turn AF", 1.7, 123}, {"River AF", 1.7, 123}},
  // 10	LAG (Maniac)	7	
  {{"$VPIP", 0.35, 1}, {"Flop AF", 1.8, 123}, {"Turn AF", 0.1, 123}, {"River AF", 0.1, 123}},
  // 11	sL N/unknown	13	
  {{"VP$IP", 0.22, 0.35}},
  // 12	Calling station	2	
  {{"VP$IP", 0.22, 1}, {"WTSD", 0.30, 1}, {"Total AF", 0, 1}},
  // 13	sLP	11	
  {{"VP$IP", 0.22, 0.35}, {"Total AF", 0.01, 1.3}},
  // 14	L N/unknown 	14	
  {{"VP$IP", 0.35, 1}},
  // 15	TAG	8	
  {{"VP$IP", 0, 0.22}, {"Flop AF", 1.8, 123}, {"Turn AF", 0.1, 123}, {"River AF", 0.1, 123}},
  // pt_Unknown,
  {{"VP$IP", 0, 1}}
};

/* short handed
2	LP (Fishy)	9	"VP$IP","between","46","99"|"Total AF","between",".01","1.65"|
3	X-Loose (Vp$iP 63%+)	1	"VP$IP","between","63","99"|
4	Showdown Muppet	3	"W$SD","between","1","33"|"WTSD",">","20",""|
5	T N/unknown	12	"VP$IP","<","33",""|
6	WT (Weak Tight)	5	"WTSD","between","1","14"|
8	sLA (Gambler)	6	"VP$IP","between","33","46"|"Flop AF",">=","2",""|"Turn AF",">",".1",""|"River AF",">",".1",""|
9	TP (Rock)	10	"VP$IP","<","33",""|"Total AF","between",".01","1.3"|
7	Ultra Aggressor	4	"Flop AF",">=","2.75",""|"Turn AF",">=","2",""|"River AF",">=","2",""|
10	LAG (Maniac)	7	"VP$IP","between","46","99"|"Flop AF",">=","2",""|"Turn AF",">",".1",""|"River AF",">",".1",""|
11	sL N/unknown	13	"VP$IP","between","33","46"|
12	Calling station	2	"VP$IP",">","33",""|"WTSD","between","33","99"|"Total AF","between","0.01","1.65"|
13	sLP	11	"VP$IP","between","33","46"|"Total AF","between",".01","1.65"|
14	L N/unknown 	14	"VP$IP","between","46","99"|
15	TAG	8	"VP$IP","<","33",""|"Flop AF",">=","2",""|"Turn AF",">",".1",""|"River AF",">",".1",""|
*/

CSymbolEnginePokerTrackerIcon::CSymbolEnginePokerTrackerIcon() {
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
	assert(p_symbol_engine_pokertracker != NULL); 
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

bool CSymbolEnginePokerTrackerIcon::PlayerMatchesIconDefinition(int chair, int icon) {
  CString pt_symbol_with_chair;
  for (int i=0; i<kMaxNumberOfRelevantStatsPerIconType; ++i) {
    if (poker_tracker_symbol_range_full_ring[icon][i].pt_symbol == "") {
      // No mismatch found so far, but no more stats to compare
      // This is a success
      return true;
    }
    // Lookup stat
    double pt_value = kUndefined;
    pt_symbol_with_chair.Format("%s%d",
      poker_tracker_symbol_range_full_ring[icon][i].pt_symbol, chair);
    bool success = p_symbol_engine_pokertracker->EvaluateSymbol(
      pt_symbol_with_chair, &pt_value, true); //!!!!!
    if (!success) {
      // This can only if the symbol lookup fails,
      // most probably due to a typo in the symbols name.
      write_log(k_always_log_errors, 
        "CSymbolEnginePokerTrackerIcon] ERROR: coud not lookup PT-symbol %s\n", 
        pt_symbol_with_chair);
      return false;
    }
    // Check range
    if ((pt_value < poker_tracker_symbol_range_full_ring[icon][i].min_value)) {
      return false;
    }
    if ((pt_value > poker_tracker_symbol_range_full_ring[icon][i].max_value)) {
      return false;
    }
  }
  // All stats compared without mismatch
  return true;
}

void CSymbolEnginePokerTrackerIcon::ComputeIcon(int chair) {
  for (int icon=0; icon<kNumberOfPokerTrackerIcons; ++icon) {
    if (PlayerMatchesIconDefinition(chair, icon)) {
      _precomputed_icon[chair] = icon;
      break;
    }
  }
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