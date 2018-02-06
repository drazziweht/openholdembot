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
// Purpose: knowing the casino, 
//   needed for PokerTracker to fetch the correct data from the DB.
//
//******************************************************************************

#include <afxwin.h>
#include <map>

class CPokerTrackerSiteID
{
public:
	// public functions
	CPokerTrackerSiteID();
	~CPokerTrackerSiteID();
	const int GetSiteId();
private:
	// private functions and variables - not available via accessors or mutators
	std::map<CString, int>	_pt4_siteid;
};
