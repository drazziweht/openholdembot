//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
// Purpose: Validating the game-state, especially scraped data
//
// Purpose: Validating the game-state, especially scraped data
//
//******************************************************************************

#define VALIDATOR_DLL_EXPORTS

#include "..\..\Shared\MagicNumbers\MagicNumbers.h"

// Constants for the validators range-checks
#define UNDEFINED_ZERO 0
#define	UNDEFINED_NEGATIVE_ONE -1

/////////////////////////////////////////////////////////////////////////////
//
// Macros for the validator,
// turning rules in "pseudo-code"
// into executable C-code.

// BEGIN_TESTCASE
//   
// Action: Ignore this by turning it into an empty string.
//
#define BEGIN_TESTCASE

// REASONING
//
// Action: Assign the reasoning to a private string-pointer.
//
#define REASONING(R) { _reasoning = (R); }

// TESTCASE_ID
//
// Action: Assign the testcase-ID to a private variable.
//
#define TESTCASE_ID(N) { _testcase_id = (N); }

// HEURISTIC
//
// Action: Assign the heuristic-flag to a private variable
//
//
#define HEURISTIC_RULE(H) { _heuristic = (H); }

// PRECONDITION
//
// Action: Assign the precondition to a private variable.
//
#define PRECONDITION(C) { _precondition = (C); }

// POSTCONDITION
//
// Action: Assign the postcondition to a private variable.
//
#define POSTCONDITION(C) { _postcondition = (C); }

// SYMBOLS_POSSIBLY_AFFECTED
//
// Action: Defines a message about the symbols, that
//   could be affected by this rule. 
//
#define SYMBOLS_POSSIBLY_AFFECTED(S) { _symbols_possibly_affected = (S); }

// END_TESTCASE
//
// Actions: ValidateSingleRule(), i.e.: 
//   * execution of the testcase
//     (if precondition for the rule applies).
//   * message on error (postcondition not met).
//   * stop autoplayer on error (optional)
//
#define END_TESTCASE ValidateSingleRule();
	
//
// End of macro code
//
/////////////////////////////////////////////////////////////////////////////

double gws(const char *the_Symbol);

void ValidateSingleRule();

bool ValidateGamestate(
    bool use_heuristic_rules,
	  bool is_tournament,
	  bool versus_bin_loaded) {
  char *_testcase_id;
  bool _heuristic;
  char *_reasoning;
  bool _precondition;
  bool _postcondition;
  char *_symbols_possibly_affected;
#include "Validator_Rules\range_checks_general_symbols_inline.cpp_"
#include "Validator_Rules\range_checks_tablemap_symbols_inline.cpp_"
#include "Validator_Rules\range_checks_formula_file_inline.cpp_"
#include "Validator_Rules\range_checks_limits_inline.cpp_"
#include "Validator_Rules\range_checks_handrank_inline.cpp_"
#include "Validator_Rules\range_checks_chairs_inline.cpp_"
#include "Validator_Rules\range_checks_rounds_positions_inline.cpp_"
#include "Validator_Rules\range_checks_probabilities_inline.cpp_"
#include "Validator_Rules\range_checks_chip_amounts_inline.cpp_"
#include "Validator_Rules\range_checks_number_of_bets_inline.cpp_"
#include "Validator_Rules\range_checks_list_tests_inline.cpp_"
#include "Validator_Rules\range_checks_poker_values_inline.cpp_"
#include "Validator_Rules\range_checks_players_friends_opponents_inline.cpp_"
#include "Validator_Rules\range_checks_flags_inline.cpp_"
#include "Validator_Rules\range_checks_common_cards_inline.cpp_"
#include "Validator_Rules\range_checks_known_cards_inline.cpp_"
#include "Validator_Rules\range_checks_nhands_inline.cpp_"
#include "Validator_Rules\range_checks_flushes_straights_sets_inline.cpp_"
#include "Validator_Rules\range_checks_rank_bits_inline.cpp_"
#include "Validator_Rules\range_checks_rank_hi_inline.cpp_"
#include "Validator_Rules\range_checks_rank_lo_inline.cpp_"
#include "Validator_Rules\range_checks_time_inline.cpp_"
#include "Validator_Rules\range_checks_autoplayer_inline.cpp_"
#include "Validator_Rules\range_checks_action_symbols_inline.cpp_"
#include "Validator_Rules\range_checks_table_stats_inline.cpp_"
#include "Validator_Rules\range_checks_card_symbols_inline.cpp_"
#include "Validator_Rules\range_checks_NOT_TO_DO_inline.cpp_"
#include "Validator_Rules\consistency_checks_cards_inline.cpp_"
#include "Validator_Rules\consistency_checks_buttons_inline.cpp_"
#include "Validator_Rules\consistency_checks_handreset_inline.cpp_"
#include "Validator_Rules\consistency_checks_memory_symbols_inline.cpp_"
#include "Validator_Rules\consistency_checks_time_inline.cpp_"
#include "Validator_Rules\consistency_checks_table_stats_inline.cpp_"
#include "Validator_Rules\consistency_checks_general_inline.cpp_"
#include "Validator_Rules\consistency_checks_history_inline.cpp_"
#include "Validator_Rules\consistency_checks_bets_inline.cpp_"
#include "Validator_Rules\consistency_checks_autoplayer_inline.cpp_"
#include "Validator_Rules\consistency_checks_players_friends_opponents_inline.cpp_"
#include "Validator_Rules\consistency_checks_chip_amounts_inline.cpp_"
#include "Validator_Rules\consistency_checks_limits_inline.cpp_"
#include "Validator_Rules\consistency_checks_number_of_bets_inline.cpp_"
#include "Validator_Rules\consistency_checks_action_symbols_inline.cpp_"
  if  (is_tournament) {
#include "Validator_Rules\range_checks_icm_symbols_inline.cpp_"
  }
  if (versus_bin_loaded) {
#include "Validator_Rules\range_checks_versus_inline.cpp_"
  }
} 