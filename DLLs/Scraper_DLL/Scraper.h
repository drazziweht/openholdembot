#pragma once
//*******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*******************************************************************************
//
// Purpose: simple interface for OpenHoldems scraper-DLL, C-style.
//   Meant to be used by external programs like table-hoppers
//
//*******************************************************************************
//
// The scraüer.DLL depends on
//   * numerical_functions.DLL
//   * string_functions.DLL
//   * window_function.DLL
// These DLLs are expected to be in the same directory.
//
//*******************************************************************************

#include "LibDef.h"

SCRAPER_DLL_API bool LoadTablemap(const char* path);
SCRAPER_DLL_API void UnloadTablemap();

// result-buffer has to be managed by the caller
// returned results are usually numbers and player-names,
// so any reasonable buffer should do OK, we recommend 255 ybtes.
SCRAPER_DLL_API void ScrapeRegion(const char* in_name, const int in_result_buffer_size, char* out_result);