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
// Purpose: container-class for GUI elements
//
//******************************************************************************

class CDlgFormulaScintilla;
class CDlgScraperOutput;
class CFlagsToolbar;
class COpenHoldemStatusbar;
class COpenHoldemTitle;
class CWhiteInfoBox;

class CGUI {
public:
  CGUI();
  ~CGUI();
public:
  CFlagsToolbar *FlagsToolbar();
  COpenHoldemStatusbar* OpenHoldemStatusbar();
  COpenHoldemTitle* OpenHoldemTitle();
  CDlgFormulaScintilla* DlgFormulaScintilla();
  CDlgScraperOutput *DlgScraperOutput();
  CWhiteInfoBox* WhiteInfoBox();
};

CGUI* GUI();