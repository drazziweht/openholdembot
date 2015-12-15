//*******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*******************************************************************************
//
// Purpose: Parking zone for the mouse after autoplayer-action
//
//*******************************************************************************

#include "stdafx.h"
#include "CParkingZone.h"

CParkingZone::CParkingZone() {
  _parking_region.left   = 0;
  _parking_region.top    = 0;
  _parking_region.right  = 0;
  _parking_region.bottom = 0;
}

CParkingZone::~CParkingZone() {
}

void CParkingZone::MoveMouseToParkingZone() {
  CalculateParkingRegion();
  //!!!!!
  SetMousePosition(50, 50);
}

void CParkingZone::CalculateParkingRegion() {
  // We assume>
  //   * nearly the complete table is available as parking-zone
  //   * everything from left to right
  //   * from the very top down to nearly the action buttons 
  //   * some pixels safety-distance above the buttons,
  //     as our button/region might be slightly smaller than the real button.
  //     but parking the mouse there might lead to highlighted buttons
  //     which can cause scraping problems and must be avoided.
  // !!! To do> care about vlient-size (borders, menu, etc.)
  _parking_region.left   = 0;
  _parking_region.top    = 0;
  //!!!!!
  _parking_region.right  = 100;
  _parking_region.bottom = 100;
}
