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

#include "CSymbolEngineActiveDealtPlaying.h"
#include "CSymbolEnginePokerTracker.h"
#include "CTableState.h"
#include "StringFunctions.h"

// Stats based on less hands are meaningless
// and the player will be treated as "unknown".
const int kNumberOfMinimalSampleSize = 25;

typedef enum {
  pt_icon_LoosePassiveFish,
  pt_icon_ExtraLoose,
  pt_icon_ShowdownMuppet,
  pt_icon_TightUnknown,
  pt_icon_WeakTight,
  pt_icon_SemiLooseAggressiveGambler,
  pt_icon_TightPassiveRock,
  pt_icon_UltraAggressive,
  pt_icon_LooseAggressiveManiac,
  pt_icon_SemiLooseUnknown,
  pt_icon_CallingStation,
  pt_icon_SemiLoosePassive,
  pt_icon_LooseUnknown,
  pt_icon_TightAggressiveTAG,
  pt_icon_Unknown,
  // always keepe this the very last one
  kNumberOfPokerTrackerIcons,
} enum_pt_icons;

typedef struct {
  CString pt_symbol;
  double  min_value;
  double  max_value;
} t_poker_tracker_symbol_range;

const int kMaxNumberOfRelevantStatsPerIconType = 4;

typedef t_poker_tracker_symbol_range t_icon_definition_table
  [kNumberOfPokerTrackerIcons][kMaxNumberOfRelevantStatsPerIconType];

const t_icon_definition_table icon_definition_table_full_ring = {
  // 2	LP (Fishy)	9	
  {{"VP$IP", 0.35, 1}, {"Total AF", 0.00, 1.3}},
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
  {{"VP$IP", 0, 0.22}, {"Total AF", 0, 1.3}},
  // 7	Ultra Aggressor	4	
  {{"Flop AF", 2.75, 123}, {"Turn AF", 1.7, 123}, {"River AF", 1.7, 123}},
  // 10	LAG (Maniac)	7	
  {{"$VPIP", 0.35, 1}, {"Flop AF", 1.8, 123}, {"Turn AF", 0.1, 123}, {"River AF", 0.1, 123}},
  // 11	sL N/unknown	13	
  {{"VP$IP", 0.22, 0.35}},
  // 12	Calling station	2	
  {{"VP$IP", 0.22, 1}, {"WTSD", 0.30, 1}, {"Total AF", 0, 1}},
  // 13	sLP	11	
  {{"VP$IP", 0.22, 0.35}, {"Total AF", 0, 1.3}},
  // 14	L N/unknown 	14	
  {{"VP$IP", 0.35, 1}},
  // 15	TAG	8	
  {{"VP$IP", 0, 0.22}, {"Flop AF", 1.8, 123}, {"Turn AF", 0.1, 123}, {"River AF", 0.1, 123}},
  // pt_Unknown,
  {{"VP$IP", 0, 1}}
};

const t_icon_definition_table icon_definition_table_short_handed = {
  // 2	LP (Fishy)	9	
  {{"VP$IP", 0.46, 1}, {"Total AF", 0.00, 1.65}},
  // 3	X-Loose (Vp$iP 63%+)	1	
  {{"VP$IP", 0.53, 1}},
  // 4	Showdown Muppet	3	
  {{"W$SD", 0, 0.33}, {"WTSD", 0.20, 1}},
  // 5	T N/unknown	12	
  {{"VP$IP", 0, 0.33}},
  // 6	WT (Weak Tight)	5	
  {{"WTSD", 0, 0.22}},	
  // 8	sLA (Gambler)	6	
  {{"VP$IP", 0.22, 0.33}, {"Flop AF", 2, 123}, {"Turn AF", 0.1, 123}, {"River AF", 0.1, 123}},
  // 9	TP (Rock)	10	
  {{"VP$IP", 0, 0.33}, {"Total AF", 0, 1.3}},
  // 7	Ultra Aggressor	4	
  {{"Flop AF", 2.75, 123}, {"Turn AF", 2, 123}, {"River AF", 2, 123}},
  // 10	LAG (Maniac)	7	
  {{"$VPIP", 0.46, 1}, {"Flop AF", 2, 123}, {"Turn AF", 0.1, 123}, {"River AF", 0.1, 123}},
  // 11	sL N/unknown	13	
  {{"VP$IP", 0.23, 0.46}},
  // 12	Calling station	2	
  {{"VP$IP", 0.33, 1}, {"WTSD", 0.33, 1}, {"Total AF", 0, 1.65}},
  // 13	sLP	11	
  {{"VP$IP", 0.33, 0.46}, {"Total AF", 0, 1.65}},
  // 14	L N/unknown 	14	
  {{"VP$IP", 0.46, 1}},
  // 15	TAG	8	
  {{"VP$IP", 0, 0.33}, {"Flop AF", 2, 123}, {"Turn AF", 0.1, 123}, {"River AF", 0.1, 123}},
  // pt_Unknown,
  {{"VP$IP", 0, 1}}
};

CSymbolEnginePokerTrackerIcon::CSymbolEnginePokerTrackerIcon() {
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
  assert(p_symbol_engine_active_dealt_playing != NULL);
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
  const t_icon_definition_table *icon_definition_table;
  // Selecting the proper icon-definition-table
  // that covers the stats of the game-type we usually play.
  // We can't use nchairs here, because the same tablemap 
  // might be usable for FR / SH / HU.
  // We can't use nplayersdealt either, because we might usually play
  // FR and all stats of our opponent are from FR-games,
  // but now as an exception we play SH / HU.
  // Therefore we take maxnplayersdealt.
  // FR: 7..10 handed.
  // SH: 2..6 handed.
  // HU: no special icon definitins found so far
  if (p_symbol_engine_active_dealt_playing->maxnplayersdealt() >= 7) {
    icon_definition_table = &icon_definition_table_full_ring;
  } else {
    icon_definition_table = &icon_definition_table_short_handed;
  }
  CString pt_symbol_with_chair;
  for (int i=0; i<kMaxNumberOfRelevantStatsPerIconType; ++i) {
    if (icon_definition_table[icon][i]->pt_symbol == "") {
      // No mismatch found so far, but no more stats to compare
      // This is a success
      return true;
    }
    // Lookup stat
    double pt_value = kUndefined;
    pt_symbol_with_chair.Format("%s%d",
      icon_definition_table[icon][i]->pt_symbol, chair);
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
    if ((pt_value < icon_definition_table[icon][i]->min_value)) {
      return false;
    }
    if ((pt_value > icon_definition_table[icon][i]->max_value)) {
      return false;
    }
  }
  // All stats compared without mismatch
  return true;
}

void CSymbolEnginePokerTrackerIcon::ComputeIcon(int chair) {
  int hands = LookupStat("hands", chair");
  if (hands < kNumberOfMinimalSampleSize) {
    return pt_icon_Unknown;
  }
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
  if (strlen(name) > 8) {
    // Probably a player-type
    //!!!!!
		// Symbol of a different symbol-engine or invalid pt_icon
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