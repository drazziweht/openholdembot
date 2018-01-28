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
// Purpose: class for bets, balances and pots,
//  which also handles proprocessing of special num,ber formats, ...
//
//******************************************************************************

#include "afxwin.h"
#include "Libdef.h"

class TABLESTATE_DLL_API CScrapedMoney {
 public:
  CScrapedMoney();
  ~CScrapedMoney();
 public:
  // returns true if the scraped value is a number and non-negative
  bool SetValue(CString scraped_value);
  bool SetValue(double new_value);
  void Reset();
  double GetValue() { return _value; }
 private:
  double _value;
};