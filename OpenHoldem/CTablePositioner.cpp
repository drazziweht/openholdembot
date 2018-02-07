//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: positioning tables at the screen. either tiled or cascaded
//
//******************************************************************************

#include "StdAfx.h"
#include "CTablePositioner.h"
#include "CAutoConnector.h"
#include "CSessionCounter.h"
#include "CSharedMem.h"
#include "WinDef.h"
#include "Winuser.h"
#include "../CTablemap/CTableMapAccess.h"
#include "..\DLLs\WindowFunctions_DLL\window_functions.h"

CTablePositioner::CTablePositioner() {
  SystemParametersInfo(SPI_GETWORKAREA, NULL, &_desktop_rectangle, NULL);
}

CTablePositioner::~CTablePositioner() {
}

// To be called once after connection
void CTablePositioner::PositionMyWindow() {		
	// Build list of poker tables (child windows)
	// Use the shared memory (auto-connector) for that. 
	HWNDs_of_child_windows = OpenHoldem()->SharedMem()->GetDenseListOfConnectedPokerWindows();
	_number_of_tables = OpenHoldem()->SharedMem()->SizeOfDenseListOfAttachedPokerWindows();
  GetWindowSize(OpenHoldem()->AutoConnector()->attached_hwnd(),
    &_table_size_x, &_table_size_y);
  if (_number_of_tables <= 0)	{
    write_log(Preferences()->debug_table_positioner(), "[CTablePositioner] PositionMyWindow() No connected tables. going to return.\n");
		// Do nothing if there are 0 tables connected.
		// Actually an empty list of tables consists of only NULLs,
		// but if MicroSofts table-arranging functions see a NULL
		// they will arrange all windows at the desktop.
		// That's not what we want.
		return;
	}
  if (p_tablemap->islobby()) { 
    write_log(Preferences()->debug_table_positioner(), "[CTablePositioner] PositionMyWindow() Going to handle the lobby...\n");
    MoveWindowToTopLeft(OpenHoldem()->AutoConnector()->attached_hwnd());
  } else if (p_tablemap->ispopup()) { 
    write_log(Preferences()->debug_table_positioner(), "[CTablePositioner] PositionMyWindow() Ignoring connected popup...\n");
  } else if (Preferences()->table_positioner_options() == k_position_tables_tiled) {
		write_log(Preferences()->debug_table_positioner(), "[CTablePositioner] PositionMyWindow() Going to tile %d windows...\n", _number_of_tables);	
    TileSingleWindow(OpenHoldem()->AutoConnector()->attached_hwnd(), HWNDs_of_child_windows);
	}	else if (Preferences()->table_positioner_options() == k_position_tables_cascaded) {
		write_log(Preferences()->debug_table_positioner(), "[CTablePositioner] PositionMyWindow() Going to cascade %d windows...\n", _number_of_tables);
    CascadeSingleWindow(OpenHoldem()->AutoConnector()->attached_hwnd(), OpenHoldem()->SessionCounter()->session_id());
	}	else {
		// Preferences()->table_positioner_options() == k_position_tables_never
		write_log(Preferences()->debug_table_positioner(), "[CTablePositioner] PositionMyWindow() Not doing anything because of Preferences()->\n");
	}
}

// To be called once per heartbeat
void CTablePositioner::AlwaysKeepPositionIfEnabled() {
  if (!Preferences()->table_positioner_always_keep_position()
    || (OpenHoldem()->AutoConnector()->attached_hwnd() == NULL)) {
    write_log(Preferences()->debug_table_positioner(), "[CTablePositioner] AlwaysKeepPositionIfEnabled() disabled or not connected\n");
    return;
  }
  RECT current_position;
  GetWindowRect(OpenHoldem()->AutoConnector()->attached_hwnd(), &current_position);
  if ((current_position.left == _table_position.left)
    && (current_position.top == _table_position.top)) {
    write_log(Preferences()->debug_table_positioner(), "[CTablePositioner] AlwaysKeepPositionIfEnabled() position is good\n");
  }
  else {
    write_log(Preferences()->debug_table_positioner(), "[CTablePositioner] AlwaysKeepPositionIfEnabled() restoring old position\n");
    MoveWindow(OpenHoldem()->AutoConnector()->attached_hwnd(),
      _table_position.left, _table_position.top);
  }
}

void CTablePositioner::ResizeToTargetSize() {
  int width;
  int height;
  p_tablemap_access->GetClientSize("targetsize", &width, &height);
  if (width <= 0 || height <= 0) {
    write_log(Preferences()->debug_table_positioner(), "[CTablePositioner] target size <= 0\n");
    return;
  }
  ResizeToClientSize(OpenHoldem()->AutoConnector()->attached_hwnd(), width, height);
}