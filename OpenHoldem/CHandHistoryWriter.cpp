//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: Part of the modular handhistory-generator.
//   Not really a symbol-engine but it is convenient
//   to make use of the symbol-engine-concepts,
//   Also used for the new summary in the logs.
//
// This module collects hh-data and writes it
//   at appropriate moments to disk, both for
//   real hand-histories and for the summary in the logs.
//
//******************************************************************************

#include "stdafx.h"
#include "CHandHistoryWriter.h"

#include "CHandHistoryDealPhase.h"
#include "CHandHistoryUncontested.h"

#include "CScraper.h"
#include "CTableState.h"
#include "CSessionCounter.h"
#include "..\DLLs\Preferences_DLL\Preferences.h"
#include "..\DLLs\Files_DLL\files.h"
#include "..\DLLs\Debug_DLL\debug.h"


CHandHistoryWriter *p_handhistory_writer = NULL;

FILE *hh_log_fp = NULL;
CCritSec hh_log_critsec;  // Used to ensure only one thread at a time writes to hh log file


void write_hh_log(bool debug_settings_for_this_message, const char* fmt, ...) {
	char		buff[10000];
	va_list		ap;
	if (debug_settings_for_this_message == false) {
		return;
	}
	if (hh_log_fp == NULL) {
		return;
	}
	CSLock lock(hh_log_critsec);
	va_start(ap, fmt);
	vsprintf_s(buff, 10000, fmt, ap);
	fprintf(hh_log_fp, "%s", buff);
	va_end(ap);
	fflush(hh_log_fp);
}



CHandHistoryWriter::CHandHistoryWriter() {
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
	//
	// This engine collects data from other HH-modules
  // and therefore should be executes after all the rest.
  assert(p_handhistory_deal_phase != NULL);
  assert(p_handhistory_uncontested != NULL);
  _lines_collected = 0;
}

CHandHistoryWriter::~CHandHistoryWriter() {
}

void CHandHistoryWriter::InitOnStartup() {
	
	if (!Preferences()->handhistory_generator_enable() ) {
		return;
		}
	if (hh_log_fp != NULL) {
			return;
		}
		assert(current_session_iD >= 0);
		int session_ID = p_sessioncounter->session_id();
		CSLock hh_lock(hh_log_critsec);

		// Append (or create) log
		if ((hh_log_fp = _fsopen(HandHistoryPath(session_ID).GetString(), "a", _SH_DENYWR)) != 0) {
			write_hh_log(true, "HH_InitOnStartup\n");
			fflush(hh_log_fp);
		}
}

void CHandHistoryWriter::UpdateOnConnection() {
}

void CHandHistoryWriter::UpdateOnHandreset() {
  write_hh_log(true, "######## Test UpdateOnHandreset ################\n");
  WriteHistory();
}

void CHandHistoryWriter::UpdateOnNewRound() {
}

void CHandHistoryWriter::UpdateOnMyTurn() {
  WriteHistory();
}

void CHandHistoryWriter::UpdateOnHeartbeat() {
}

void CHandHistoryWriter::AddMessage(CString message) {
  assert(_lines_collected < kMaxLines);
  _handhistory_data[_lines_collected] = message;
  ++_lines_collected;
  return;
}

void CHandHistoryWriter::PostsSmallBlind(int chair) {
}

void CHandHistoryWriter::PostsBigBlind(int chair) {
}

void CHandHistoryWriter::PostsAnte(int chair) {
}

void CHandHistoryWriter::Checks(int chair) {
}

void CHandHistoryWriter::Folds(int chair) {
}

void CHandHistoryWriter::Calls(int chair) {
}

void CHandHistoryWriter::Raises(int chair) {
}

void CHandHistoryWriter::WinsUncontested(int chair) {
}

CString CHandHistoryWriter::PlayerName(int chair) {
  assert(chair >= 0);
  assert(chair <= kLastChair);
  return p_table_state->Player(chair)->name();
}

// Should be called
// * when a hand is over
// * when it is my turn (summary in the log)
void CHandHistoryWriter::WriteHistory() {
  return; //!!
  write_hh_log(true, "Summary (might be not accurate)");
  for (int i=0; i<_lines_collected; ++i) {
    write_hh_log(true, (char*)(LPCTSTR)_handhistory_data[i]); 
  }
  write_hh_log(true, "============================================================");
  _lines_collected = 0;
}

bool CHandHistoryWriter::EvaluateSymbol(const CString name, double *result, bool log /* = false */) {
  // No symbols provided
	return false;
}

CString CHandHistoryWriter::SymbolsProvided() {
  // No symbols provided
  return "";
}
	
