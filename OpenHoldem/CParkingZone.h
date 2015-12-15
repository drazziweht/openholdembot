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

#ifndef INC_CPARKINGZONE_H
#define INC_CPARKINGZONE_H

class CParkingZone {
 public:
  CParkingZone();
  ~CParkingZone();
 public:
  void MoveMouseToParkingZone();
 private:
  void CalculateParkingRegion();
 private:
  RECT _parking_region;
};

#endif INC_CPARKINGZONE_H
