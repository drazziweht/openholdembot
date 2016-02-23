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

#include "stdafx.h"
#include "CSymbolEngineBlindChairs.h"

#include "CBetroundCalculator.h"
#include "CScraper.h"
#include "CSymbolEngineActiveDealtPlaying.h"
#include "CSymbolEngineChairs.h"
#include "CSymbolEngineHistory.h"
#include "CSymbolEnginePositions.h"
#include "CSymbolEngineRaisers.h"
#include "CSymbolEngineTableLimits.h"
#include "CTableState.h"
#include "..\CTablemap\CTablemap.h"

CSymbolEngineBlindChairs *p_symbol_engine_blind_chairs = NULL;

CSymbolEngineBlindChairs::CSymbolEngineBlindChairs() {
  // The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
  assert(p_symbol_engine_active_dealt_playing != NULL);
  assert(p_symbol_engine_chairs != NULL);
  assert(p_symbol_engine_history != NULL);
  assert(p_symbol_engine_positions != NULL);
  assert(p_symbol_engine_raisers != NULL);
  assert(p_symbol_engine_tablelimits != NULL);
}

CSymbolEngineBlindChairs::~CSymbolEngineBlindChairs() {
}

void CSymbolEngineBlindChairs::InitOnStartup() {
  ResetOnConnection();
}

void CSymbolEngineBlindChairs::ResetOnConnection() {
  ResetOnHandreset();
}

void CSymbolEngineBlindChairs::ResetOnHandreset() {
  _missing_small_blind = false;
  _smallblind_chair = kUndefined;
  _bigblind_chair = kUndefined;
}

void CSymbolEngineBlindChairs::ResetOnNewRound() {
}

void CSymbolEngineBlindChairs::ResetOnMyTurn() {
  // Only well-defined at my turn and requires userchair for calculation.
  // But it seems some "casinos" like test-suite can break that condition.
  // http://www.maxinmontreal.com/forums/viewtopic.php?f=110&t=17915#p124550
  //assert(p_symbol_engine_userchair->userchair_confirmed());
  CalculateMissingSmallBlind();
  CalculateSmallBlindChair();
  CalculateBigBlindChair();
}

void CSymbolEngineBlindChairs::ResetOnHeartbeat() {
}

double CSymbolEngineBlindChairs::CurrentBetByDealPosition(int dealposition) {
  int chair = p_symbol_engine_chairs->GetChairByDealposition(dealposition);
  if (chair < 0) return kUndefinedZero;
  if (chair > kLastChair) return kUndefinedZero;
  return p_table_state->Player(chair)->bet();
}

bool CSymbolEngineBlindChairs::AllOpponentsDidActPreflop() {
  // Meant for preflop, multiway and big-blind position only.
  // All opponents must have a bet
  return (p_symbol_engine_active_dealt_playing->nplayersdealt() == p_symbol_engine_raisers->nopponentsbetting());
}

void CSymbolEngineBlindChairs::CalculateMissingSmallBlind() {
  // Calculate on first action preflop only 
  if (p_betround_calculator->betround() != kBetroundPreflop) return;
  if (p_symbol_engine_history->DidActThisHand()) return;
  // Default
  _missing_small_blind = false;
  // No missing small blind headsup
  if (p_symbol_engine_active_dealt_playing->nplayersdealt() <= 2) return;
  // Small blind found, so not missing
  if (CurrentBetByDealPosition(1) == SMALL_BLIND) return;
  // If we are NOT the Second player to be dealt and see a bet 
  // of 1 big blind left to the dealer, then it is the big blind (SB missing)
  int dealposition = p_symbol_engine_positions->dealposition();
  if ((dealposition != 2) && (CurrentBetByDealPosition(1) == BIG_BLIND)) {
    _missing_small_blind = true;
    return; 
  }
  // Problematic is only the case when I am in "big blind" (second player to be dealt)
  // * if bet of DealPosition1Chair > 1 bblind then SB raised and is present
  // * if bet = 1 big blind and players "behind me" did act, then SB is present and limped
  // * if bet = 1 big blind and players behind me still to act, then SB is missing
  if ((dealposition == 2) && (CurrentBetByDealPosition(1) > BIG_BLIND)) return;
  // Case 2 and 3: precondition: bet = 1 big blind
  if ((dealposition == 2) && AllOpponentsDidActPreflop()) return;
  if ((dealposition == 2) && !AllOpponentsDidActPreflop()) {
    _missing_small_blind = true;
    return; 
  }
  // Some very special cases left, 
  // e.g. one of the blinds allin for less than a blind.
}

void CSymbolEngineBlindChairs::CalculateSmallBlindChair() {
  if (_missing_small_blind) {
    _smallblind_chair = kUndefined;
  } else if (p_symbol_engine_active_dealt_playing->nplayersdealt() > 2) {
    // 3 or more handed, normal blinds
    _smallblind_chair = p_symbol_engine_chairs->GetChairByDealposition(1);
  } else {
    // Headsup, reversed blinds
    _smallblind_chair = p_symbol_engine_chairs->GetChairByDealposition(2);
  }
}

void CSymbolEngineBlindChairs::CalculateBigBlindChair() {
  if (_missing_small_blind) {
    // Bigblind immediatelz after dealer
    _bigblind_chair = p_symbol_engine_chairs->GetChairByDealposition(1);
  } else if (p_symbol_engine_active_dealt_playing->nplayersdealt() > 2) {
    // 3 or more handed, normal blinds
    _bigblind_chair = p_symbol_engine_chairs->GetChairByDealposition(2);
  } else {
    // Headsup, reversed blinds
    _bigblind_chair = p_symbol_engine_chairs->GetChairByDealposition(1);
  }
}

bool CSymbolEngineBlindChairs::EvaluateSymbol(const char *name, double *result, bool log /* = false */) {
  if (memcmp(name, "smallblind_chair", 16)==0) {
		*result = _smallblind_chair;
		return true;
	} else if (memcmp(name, "bigblind_chair", 14)==0) {
		*result = _bigblind_chair;
		return true;
  }
  // Symbol of a different symbol-engine
	return false;
}

CString CSymbolEngineBlindChairs::SymbolsProvided() {
  return "smallblind_chair bigblind_chair ";
}
