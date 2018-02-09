//*******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*******************************************************************************
//
// Purpose: Two interfaces for OpenHoldems scraper-DLL
//.  * object-oriented interface for OpenHoldem.
//   * simple C-style interface, meant for other programs like table-hoppers
//
//*******************************************************************************
//
// The scraper.DLL depends on
//   * numerical_functions.DLL
//   * string_functions.DLL
//   * window_function.DLL
// These DLLs are expected to be in the same directory.
//
//*******************************************************************************

#define SCRAPER_DLL_EXPORTS

#include "CBasicScraper.h"
#include "CTablemap\CTablemapCompletenessChecker.h"

// Singleton for CBasicScraper
// Gets initialized via the accessor-function once needed
CBasicScraper* p_basic_scraper = NULL;

CBasicScraper* BasicScraper() {
  if (p_basic_scraper == NULL) {
    p_basic_scraper = new CBasicScraper();
  }
  assert(p_basic_scraper != NULL);
  return p_basic_scraper;
}

CBasicScraper::CBasicScraper() {
}

CBasicScraper::~CBasicScraper(){
}

bool CBasicScraper::LoadTablemap(const char* path) {
  _tablemap.LoadTablemap(path);
  // Map will be verified as long as it is not marked as popup or lobby
  CTablemapCompletenessChecker tablemap_completeness_checker;
  tablemap_completeness_checker.VerifyMap();
}

CString CBasicScraper::ScrapeRegion(const CString name) {
  //!!!!!
  return "error";
}

//*******************************************************************************
//
// Simple C-style interface, meant for other programs like table-hoppers
//
//*******************************************************************************

// Loads a tablemap (and automatically unloads the previous one)
SCRAPER_DLL_API bool LoadTablemap(const char* path) {
  return BasicScraper()->LoadTablemap(path);
}

// result-buffer has to be managed by the caller
// returned results are usually numbers and player-names,
// so any reasonable buffer should do OK, we recommend 255 ybtes.
SCRAPER_DLL_API void ScrapeRegion(const char* in_name, const int in_result_buffer_size, char* out_result) {
  if (out_result == NULL) {
    // Invalid buffer, do nothing
    return;
  } 
  if (in_result_buffer_size <= 0) {
    // Invalid buffer, do nothing
    return;
  } 
  CString result = BasicScraper()->ScrapeRegion(in_name);
  if (result.GetLength() >= in_result_buffer_size){
    // Buffer too small for result + terminal null-character
	// Clear buffer and return
    out_result = '\0';
    return;
  }
  memcpy(out_result, result.GetBuffer(), (result.GetLength() + 1));
}