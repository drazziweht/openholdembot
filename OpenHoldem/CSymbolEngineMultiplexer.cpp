//*******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*******************************************************************************
//
// Purpose: multiplexer functionality to support postfixes
//   like "dealer" or "last_caller_chair" instead of chair numbers.
//   This should be one of the very first symbol-engines to be called
//   to allow proper replacement of the postfix and a new query 
//   before other symbol engines try to handle this query.
//
//*******************************************************************************

#include "stdafx.h"
#include "CSymbolEngineMultiplexer.h"

#include "CFunctionCollection.h"
#include "CPreferences.h"
#include "StringFunctions.h"

CSymbolEngineMultiplexer *p_symbol_engine_multiplexer = NULL;

const int kNumberOfSupportedPostfixes = 11;
// If you extend the list below please don't forget to update FastExitOnLastCharacter()
const char* kSupportedPostFixes[kNumberOfSupportedPostfixes] = {
  "user",
  "headsup",
  "smallblind",
  "bigblind",
  "dealer",
  "cutoff",
  "firstcaller",
  "lastcaller",
  "firstraiser",
  "lastraiser",
  // lastraiser and raischair are synonyms
  "raischair"    
};

CSymbolEngineMultiplexer::CSymbolEngineMultiplexer() {
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
	//
	// This engine does not use any other engines.
}

CSymbolEngineMultiplexer::~CSymbolEngineMultiplexer() {
}

void CSymbolEngineMultiplexer::InitOnStartup() {
	ResetOnConnection();
}

void CSymbolEngineMultiplexer::ResetOnConnection() {
}

void CSymbolEngineMultiplexer::ResetOnHandreset() {
}

void CSymbolEngineMultiplexer::ResetOnNewRound() {
}

void CSymbolEngineMultiplexer::ResetOnMyTurn() {
}

void CSymbolEngineMultiplexer::ResetOnHeartbeat() {
}

inline bool CSymbolEngineMultiplexer::FastExitOnLastCharacter(int last_character_of_symbol) {
  switch (last_character_of_symbol) {
    // Last characters of multiplexed postfixes
    case 'd':
    case 'f':
    case 'p':
    case 'r': return false;
    default: return true;
  }
}

void CSymbolEngineMultiplexer::Multiplex(const char* complete_name, int multiplex_index, double* result) {
  // Multiplexing symbols like "currentbet_dealer"
  // that consist of a general symbol prefix and a chair postfix,
  // finallz evaluationg someting like "currentbetX", 
  // where X is the chair-number.
  write_log(true, "[CSymbolEngineMultiplexer] Multiplex(%s)\n", complete_name);
  assert(strlen(complete_name) >= (strlen(kSupportedPostFixes[multiplex_index] + 2)));
  assert(StringAIsPostfixOfStringB(kSupportedPostFixes[multiplex_index], complete_name));
  assert(RightCharacter(complete_name, strlen(kSupportedPostFixes[multiplex_index])) == '_');
  CString complete_symbol = complete_name;
  int complete_length = strlen(complete_name);
  int multiplexer_length = strlen(kSupportedPostFixes[multiplex_index]);
  int symbol_prefix_length = complete_length - multiplexer_length - 1;
  assert(complete_length > 0);
  assert(multiplexer_length > 0);
  assert(symbol_prefix_length > 0);
  CString symbol_prefix = complete_symbol.Left(symbol_prefix_length);
  CString multiplexer_postfix = complete_symbol.Right(multiplexer_length);
  assert(symbol_prefix != "");
  assert(multiplexer_postfix != "");
  int chair = p_function_collection->Evaluate(multiplexer_postfix, true);
  write_log(true, "[CSymbolEngineMultiplexer] Chair(%s( = %i\n",
    multiplexer_postfix, chair);
  if ((chair < 0) || (chair > kLastChair)) {
    write_log(k_always_log_errors, "[CSymbolEngineMultiplexer] WARNING! Chair out of range\n");
    *result = kUndefined;
    return;
  }
  CString multiplexed_symbol_with_chair_number;
  multiplexed_symbol_with_chair_number.Format("%s%i",
    symbol_prefix, chair);
  *result = p_function_collection->Evaluate(multiplexed_symbol_with_chair_number);
  write_log(true, "[CSymbolEngineMultiplexer] Evaluated multiplexed symbol %s -> %.3f\n",
    multiplexed_symbol_with_chair_number, *result);
}

bool CSymbolEngineMultiplexer::EvaluateSymbol(const char *name, double *result, bool log /* = false */) {
  // This function gets called a lot, 
  // very early and therefore for nearly every symbol.
  // Therefore we implement several fast exits.
  // But no FAST_EXIT_ON_OPENPPL_SYMBOLS(name),
  // because we also want to multiplex OpenPPL-symbols.
  //
  // Fast exit on f$-functions.
  // They should already be handled, but just in case.
	if (memcmp(name, "f$", 2) == 0)	{
		return false;
	} 
  // Fast exit on last character
  char last_character = RightCharacter(name);
  if (FastExitOnLastCharacter(last_character)) {
    return false;
  }
  int length_of_symbol = strlen(name);
  // Checking all postfixes...
  for (int i=0; i<kNumberOfSupportedPostfixes; ++i) {
    int length_of_postfix = strlen(kSupportedPostFixes[i]);
    // Skip postfixes that are too long for the symbol
    // We need at least 2 dditional characters>
    // one for the basic symbol and one for the underscore inbetween.
    if (length_of_symbol < (length_of_postfix + 2)) continue;
    // If there is no underscore at the expected position
    // (before postfix) then continue
    if (RightCharacter(name, length_of_postfix) != '_') continue;
    if (StringAIsPostfixOfStringB(kSupportedPostFixes[i], name)) {
      Multiplex(name, i, result);
      return true;
    }
  }
  // Symbol of a different symbol-engine
	return false;
}

CString CSymbolEngineMultiplexer::SymbolsProvided() {
  // Way too much to name them all.
  // List depends heavily on other symbol-egines,
  // so we leave it empty at the cost of
  // losing syntax-highlighting for multiplexed symbols.
  return "";
}

/*
if (s.Right(10) == "_raischair") {
		chair = p_symbol_engine_raisers->raischair();
	}
	// PokerTracker symbols for the opponent headsup chair
	else if (s.Right(8) == "_headsup") {
    chair = p_symbol_engine_chairs->opponent_headsup_chair();
	}
  // PokerTracker symbols for the smallblind chair
	else if (s.Right(11) == "_smallblind") {
    chair = p_symbol_engine_chairs->smallblind_chair();
	}
  // PokerTracker symbols for the bigblind chair
	else if (s.Right(9) == "_bigblind") {
    chair = p_symbol_engine_chairs->bigblind_chair();
	}
  // PokerTracker symbols for the cutoff chair
	else if (s.Right(7) == "_cutoff") {
    chair = p_symbol_engine_chairs->cutoff_chair();
	}
  // PokerTracker symbols for the firstcaller chair
	else if (s.Right(12) == "_firstcaller") {
    chair = p_symbol_engine_callers->firstcaller_chair();
	}
  // PokerTracker symbols for the lastcaller chair
	else if (s.Right(11) == "_lastcaller") {
    chair = p_symbol_engine_callers->lastcaller_chair();
	}
  // PokerTracker symbols for the firstraiser chair
	else if (s.Right(12) == "_firstraiser") {
		chair = p_symbol_engine_raisers->firstraiser_chair();
	}
  // PokerTracker symbols for the dealerchair chair
	else if (s.Right(7) == "_dealer") {
    chair = p_symbol_engine_dealerchair->dealerchair();
	}
  // PokerTracker symbols for the  chair
	else if (s.Right(5) == "_user") {
    chair = p_symbol_engine_userchair->userchair();
	}*/