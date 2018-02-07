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

#include "stdafx.h"
#include "Singletons.h"

#include "CCasinoInterface.h"
#include "CAutoConnector.h"
#include "CAutoplayer.h"
#include "CAutoplayerFunctions.h"
#include "CAutoplayerTrace.h"
#include "CConfigurationCheck.h"
#include "CDebugTab.h"
#include "..\DLLs\Symbols_DLL\CEngineContainer.h"
#include "CFileSystemMonitor.h"
#include "CFormulaParser.h"
#include "CHandresetDetector.h"
#include "CHeartbeatThread.h"
#include "CIteratorThread.h"
#include "..\DLLs\Scraper_DLL\CLazyScraper.h"
#include "CMemoryPool.h"
#include "COcclusioncheck.h"
#include "COpenHoldemTitle.h"
#include "CParserSymbolTable.h"
#include "CPokerTrackerThread.h"
#include "CPopupHandler.h"

#include "CRebuyManagement.h"
#include "..\DLLs\Scraper_DLL\CScraper.h"
#include "CSessionCounter.h"
#include "CSharedMem.h"
#include "CStableFramesCounter.h"
#include "CStringMatch.h"
#include "..\DLLs\Symbols_DLL\CSymbolEngineTableLimits.h"
#include "CTableMaploader.h"
#include "CTablePositioner.h"
#include "CTitleEvaluator.h"
#include "CValidator.h"
#include "CVersionInfo.h"
#include "CWatchdog.h"
#include "CWhiteInfoBox.h"

void InstantiateAllSingletons() {
  // Instantiation of all singletons, except session-counter.
  // session-counter has to be done do earlier, as it is needed 
  // to create the log-file, which might be needed before this point.
  // This function gets executed exactly once at startup.
  // So the global class-pointers have to be NULL.
  write_log(Preferences()->debug_singletons(), "[Singletons] Going to create m emory pools\n");
  CreateMemoryPools();
  write_log(Preferences()->debug_singletons(), "[Singletons] Going to create CWatchdog\n");
  assert(!p_watchdog);
  p_watchdog = new CWatchdog;
  write_log(Preferences()->debug_singletons(), "[Singletons] Going to create CStringMatch\n");
  assert(!p_string_match);
  p_string_match = new CStringMatch;
  write_log(Preferences()->debug_singletons(), "[Singletons] Going to create CAutoplayerTrace\n");
  assert(!p_autoplayer_trace);
  p_autoplayer_trace = new CAutoplayerTrace;
  write_log(Preferences()->debug_singletons(), "[Singletons] Going to create CHandresetDetector\n");
  assert(!p_handreset_detector);
  p_handreset_detector = new CHandresetDetector;
  write_log(Preferences()->debug_singletons(), "[Singletons] Going to create CConfigurationCheck\n");
  assert(!p_configurationcheck);
  p_configurationcheck = new CConfigurationCheck;
  write_log(Preferences()->debug_singletons(), "[Singletons] Going to create CAutoplayerFunctions\n");
  assert(!p_autoplayer_functions);
  p_autoplayer_functions = new CAutoplayerFunctions;
  write_log(Preferences()->debug_singletons(), "[Singletons] Going to create CStableFramesCounter\n");
  assert(!p_stableframescounter);
  p_stableframescounter = new CStableFramesCounter;                                                                                                                                                         
  write_log(Preferences()->debug_singletons(), "[Singletons] Going to create CTitleEvaluator\n");
  assert(!p_title_evaluator);
  p_title_evaluator = new CTitleEvaluator;
  write_log(Preferences()->debug_singletons(), "[Singletons] Going to create CScraper\n");
  assert(!p_scraper); 
  p_scraper = new CScraper;
  write_log(Preferences()->debug_singletons(), "[Singletons] Going to create CLazyScraper\n");
  assert(!p_lazyscraper);
  p_lazyscraper = new CLazyScraper;
  write_log(Preferences()->debug_singletons(), "[Singletons] Going to create CTablemap\n");
  assert(!p_tablemap);
  p_tablemap = new CTablemap;
  write_log(Preferences()->debug_singletons(), "[Singletons] Going to create CTablemapAccess\n");
  assert(!p_tablemap_access);
  p_tablemap_access = new CTablemapAccess;
  write_log(Preferences()->debug_singletons(), "[Singletons] Going to create CParserSymbolTable\n");
  assert(!p_parser_symbol_table);
  p_parser_symbol_table = new CParserSymbolTable;
  write_log(Preferences()->debug_singletons(), "[Singletons] Going to create CFormulaParser\n");
  assert(!p_formula_parser);
  p_formula_parser = new CFormulaParser;
  write_log(Preferences()->debug_singletons(), "[Singletons] Going to create CDebugTab\n");
  assert(p_debug_tab == NULL);
  p_debug_tab = new CDebugTab;
  write_log(Preferences()->debug_singletons(), "[Singletons] Going to create CAutoplayer\n");
  assert(!p_autoplayer);
  p_autoplayer = new CAutoplayer();
  write_log(Preferences()->debug_singletons(), "[Singletons] Going to create CPokerTrackerThread\n");
  assert(!p_pokertracker_thread);
  p_pokertracker_thread = new CPokerTrackerThread;
  write_log(Preferences()->debug_singletons(), "[Singletons] Going to create CValidator\n");
  assert(!p_validator);
  p_validator = new CValidator;
  write_log(Preferences()->debug_singletons(), "[Singletons] Going to create CTablePositioner\n");
  assert(!p_table_positioner);
  p_table_positioner = new CTablePositioner;
  write_log(Preferences()->debug_singletons(), "[Singletons] Going to create CTableMapLoader\n");
  assert(!p_tablemap_loader);
  p_tablemap_loader = new CTableMapLoader;
  write_log(Preferences()->debug_singletons(), "[Singletons] Going to create CVersionInfo\n");
  assert(!p_version_info);
  p_version_info = new CVersionInfo;
  write_log(Preferences()->debug_singletons(), "[Singletons] Going to create CPopupHandler\n");
  assert(!p_popup_handler);
  p_popup_handler = new CPopupHandler;
  write_log(Preferences()->debug_singletons(), "[Singletons] Going to create CAutoConnector\n");
  assert(!p_autoconnector);
  p_autoconnector = new CAutoConnector;
  write_log(Preferences()->debug_singletons(), "[Singletons] Going to create CWhiteInfoBox\n");
  assert(!p_white_info_box);
  p_white_info_box = new CWhiteInfoBox;
  EngineContainer()->CreateSymbolEngines();
  write_log(Preferences()->debug_singletons(), "[Singletons] Going to create CCasinoInterface\n");
  assert(!p_casino_interface);
  p_casino_interface = new CCasinoInterface;
  write_log(Preferences()->debug_singletons(), "[Singletons] All singletons created.\n");
}

bool all_threads_stopped = false;

// To be executed first,
// as these threads might access some variables.
void StopThreads() {
	write_log(Preferences()->debug_singletons(), "[Singletons] StopThreads()\n");
	if (p_heartbeat_thread)	{
		write_log(Preferences()->debug_singletons(), "[Singletons] Deleting heartbeat-thread\n");
		delete p_heartbeat_thread;
		p_heartbeat_thread = NULL;
	}
	if (p_iterator_thread) {
		write_log(Preferences()->debug_singletons(), "[Singletons] Deleting iterator-thread\n");
		delete p_iterator_thread;
		p_iterator_thread = NULL;
	}
	if (p_pokertracker_thread) { 
		write_log(Preferences()->debug_singletons(), "[Singletons] Deleting PokerTracker-thread\n");
		p_pokertracker_thread->~CPokerTrackerThread(); 
		p_pokertracker_thread = NULL; 
	}
	all_threads_stopped = true;
}
 
  

CreateMemoryPools();
EngineContainer()->CreateSymbolEngines();

Formula
=======
CParserSymbolTable
CFormulaParser
CDebugTab

Main
====
CWatchdog
CSharedMem
CAutoplayer
CTablePositioner
CVersionInfo
CPopupHandler
CAutoConnector

Symbols
=======
CAutoplayerTrace -> EvaluatorTrace
CPokerTrackerThread

Refactoring
===========
CStringMatch

Scraper
=======
CTitleEvaluator
CScraper

Unclear
=======
CHandresetDetector (main or symbols)
CConfigurationCheck
CAutoplayerFunctions
CStableFramesCounter (main or symbols)
CLazyScraper (main or scraper, simplescraper / region_evaluator?)
CTablemap
CTablemapAccess
CValidator
CFileSystemMonitor
CTableMapLoader
CWhiteInfoBox (only used for log$, not for the rest, to be refactored)
CCasinoInterface