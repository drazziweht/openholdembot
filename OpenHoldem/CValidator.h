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

#ifndef INC_VALIDATOR_H
#define INC_VALIDATOR_H

// !!! ATTENTION
// !!! CString not properly declared/included.
// !!! atlstr / afxstr don't work.
// !!! Any other includes cause lots of erros
// !!! (escpecially CSpaceOptimiedObject.h).

class CValidator
{
public:
	// public functions
	CValidator();
	~CValidator();
	void ValidateGameState();
	void SetEnabledManually(bool Enabled);
private:
	// private functions
	CString Symbols_And_Values(const CString symbols_possibly_affected);
private:
	// private data members, not (directly) accessible
private:
	bool _enabled_manually;
};

extern CValidator *p_validator; 

#endif

																																																						