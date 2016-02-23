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

#ifndef INC_CSYMBOLENGINECHAIRS_H
#define INC_CSYMBOLENGINECHAIRS_H

#include "CVirtualSymbolEngine.h"

class CSymbolEngineChairs: public CVirtualSymbolEngine {
  friend class CSymbolEngineBlindChairs;
 public:
	CSymbolEngineChairs();
	~CSymbolEngineChairs();
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
 protected:
  int GetChairByDealposition(int dealposition);
 private:
  // Dealer, user, bigblind and smallblind
  // get handled by different symbol engines
  int headsup_chair(); 
  int cutoff_chair();
  int mp3_chair();
  int mp2_chair();
  int mp1_chair();
  int ep3_chair();
  int ep2_chair();
  int ep1_chair();
  int utg_chair();
  // !!!!! to do: raischair, firstcaller. firstraiser, lastcaller
 private:
  void CalculateOpponentHeadsupChair();
  void CalculateSmallBlindChair();
  void CalculateBigBlindChair();
  void CalculateCutOffChair();
 private:
  int GetChairByOffsetFromDealer(int counter_clockwise_offset);
 private:
  int _opponent_headsup_chair;
  int _smallblind_chair;
  int _bigblind_chair;
  int _cutoff_chair;
 private:
  int _nchairs;
};

extern CSymbolEngineChairs *p_symbol_engine_chairs;

#endif INC_CSYMBOLENGINECHAIRS_H
