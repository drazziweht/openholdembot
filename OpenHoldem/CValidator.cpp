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