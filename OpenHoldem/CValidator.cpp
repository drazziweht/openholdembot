//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose:
//
//******************************************************************************

//!!!!!
#pragma warning(error:4005)   

#include "StdAfx.h"
#include "CValidator.h"

#include "CAutoPlayer.h"
#include "CEngineContainer.h"
#include "CPreferences.h"
#include "CSharedMem.h"
#include "CSymbolEngineAutoplayer.h"
#include "CSymbolEngineHandRank.h"
#include "CSymbolEngineIsTournament.h"
#include "CSymbolEngineRandom.h"
#include "CSymbolEngineReplayFrameController.h"
#include "CSymbolEngineTime.h"
#include "CSymbolEngineVersus.h"
#include "CTableState.h"
#include "FloatingPoint_Comparisions.h"
#include "OH_MessageBox.h"

CValidator *p_validator = NULL;

CValidator::CValidator() {
	_enabled_manually = false;
}

CValidator::~CValidator() {
}

void CValidator::SetEnabledManually(bool Enabled) {
	_enabled_manually = Enabled;
}

// Create a stringified list of (symbol: value)-pairs
// for output in the error-message.
CString CValidator::Symbols_And_Values(const CString symbols_possibly_affected) {
	CString Result = "";
	int Token_Pos = 0;
	while (Token_Pos < symbols_possibly_affected.GetLength()) {
		// Tokenize the string, using space or commas as delimiters.
		CString Symbol = symbols_possibly_affected.Tokenize(" ,", Token_Pos);
		double Symbol_Value = gws(Symbol);
		CString Symbol_Value_String;
		// Convert symbol value to string, 7 digits total, 2 digits precision
		Symbol_Value_String.Format("%7.2f", Symbol_Value);
		Result += "\n    ("
			+ Symbol
			+ ": "
			+ Symbol_Value_String
			+ ")";
	}
	return Result;
}

void CValidator::ValidateSingleRule() {
	// Heuristic rules and not to be tested?
	if (_heuristic && !preferences.validator_use_heuristic_rules()) {
		return;
	}
	// Test to be executed?
	if (_precondition) { 
		// Test failed?
	  if (!_postcondition ) { 
			if (_no_errors_this_heartbeat) {
				// First error: shoot replayframe, if needed
				if (preferences.validator_shoot_replayframe_on_error()) {
					p_engine_container->symbol_engine_replayframe_controller()->ShootReplayFrameIfNotYetDone();
				}
				_no_errors_this_heartbeat = false;
			}
			if (preferences.validator_stop_on_error()) { 
				p_autoplayer->EngageAutoplayer(false); 
			}
			// Create error message
			CString the_ErrorMessage = "TESTCASE ID: " 
				+ CString(_testcase_id) 
				+ "\n\n";
			if (_heuristic)	{
				the_ErrorMessage += "HEURISTIC RULE: yes\n\n";
			}
			the_ErrorMessage += "REASONING: "
				+ CString(_reasoning)
				+ "\n\n"
			  + "SYMBOLS AFFECTED: " 
				+ Symbols_And_Values(_symbols_possibly_affected)
				+ "\n\n"
				+ "(The validator is a tool to help you finding errors in your tablemap.)\n"; 
			// Show error message, if appropriate
			OH_MessageBox_Error_Warning(the_ErrorMessage, "VALIDATOR ERROR"); 
			// Log error message
			the_ErrorMessage.Replace("\n\n", ". ");
			the_ErrorMessage.Replace("\n", " ");
			write_log(k_always_log_errors, "%s%s\n", "VALIDATOR ERROR: ", the_ErrorMessage);
		} 
  } 
}

// gws function to access the symbols by name
//
double CValidator::gws(const char *the_Symbol) {
  double result = kUndefined;
  p_engine_container->EvaluateSymbol(the_Symbol, &result);
  return result;
}

void CValidator::ValidateGameState() {
  if (!p_engine_container->symbol_engine_autoplayer()->ismyturn()) {
    // Validate only if it is my turn.
    //   * because then we have stable frames
    //   * because then it matters most
    return;
  }
	if (preferences.validator_enabled()
		  // Manually enabled via toolbar?
		  || (_enabled_manually)) {
	  // Validate.
	  //
	  // Validator-rules are defined in "pseudo-code",
	  // that is easily understandable for non-programmers,
	  // but can be turned into C-code using macro-techniques.
	  //
	  // Due to the large number of rules, 
	  // we just put them in external files
	  // and include them here as is.
	  //
    _no_errors_this_heartbeat = true;
    ValidateGameState()
    ValidateVersusDBOnlyIfInstalled();
     ValidateICMOnlyIfTournament();
  }
}

void CValidator::ValidateVersusDBOnlyIfInstalled() {
	if(p_engine_container->symbol_engine_versus()->VersusBinLoaded())	{
    ValidateGamestateVersusBin();
	}
}

void CValidator::ValidateICMOnlyIfTournament() {
  if (p_engine_container->symbol_engine_istournament()->istournament()) {

  }
}