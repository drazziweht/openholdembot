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

#ifndef INC_STRUCTS_DEFINES_H
#define INC_STRUCTS_DEFINES_H

#include "..\pokereval\include\poker_defs.h"
#include "..\Shared\MagicNumbers\MagicNumbers.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum FileType { FTnone=1, FTlist, FTfunc };

struct STableList {
	HWND		hwnd;
  int     tablemap_index;
#ifndef   OPENHOLDEM_PROGRAM
  RECT    attached_rect;
  RECT    crect;
  CString title;
#endif
};

//prwin 1326 chair structure    Matrix 2008-04-29
struct sprw1326_chair {
	int			level;			// indicates significance level for 'always consider'
	int			limit;			// max index into significance array - used for computational efficiency
	int			ignore;			// if non-zero no weighting will be applied to this chair
	int			rankhi[k_number_of_pocketcard_combinations_without_order];	// hi card number in pocket cards
	int			ranklo[k_number_of_pocketcard_combinations_without_order];	// lo card number in pocket cards
	int			weight[k_number_of_pocketcard_combinations_without_order];	// the significance value for this hand
	double		scratch;		// for future reference
};

//prwin 1326 structure			Matrix 2008-04-29
struct sprw1326
{
	int			useme;				// unless set to 1326 the normal OH prwin will be used
	int			preflop;			// unless set to 1326 the normal OH prwin will be used pre-flop
	int			usecallback;		// unless set to 1326 the callback function will not be invoked
	double	(*prw_callback)(); // if enabled will be invoked before the prwin calculation pass
	double	scratch;			// for future reference
	sprw1326_chair	vanilla_chair;	// will be precalculated by OH at startup - convenience values
	sprw1326_chair  chair[kMaxNumberOfPlayers];  // structures for each chair
};

#endif /* INC_STRUCTS_DEFINES_H */
