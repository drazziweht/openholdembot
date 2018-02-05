#pragma once
//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: verifying naming conventions of functions and handlists,
//   either newly created or renamed
//
//******************************************************************************

#include <afxwin.h>

bool VerifyFunctionName(CString name);
bool VerifyListName(CString name);