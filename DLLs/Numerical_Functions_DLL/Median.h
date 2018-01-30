#pragma once

/*
 * Algorithm from N. Wirth's book, implementation by N. Devillard.
 * This code in public domain.
 *
 * Changes for the OpenHoldem-projectz:
 *   DLL-export for numerical_functions.DLL
 */

#define NUMERICAL_FUNCTIONS_DLL_EXPORTS

#include "LibDef.h"

NUMERICAL_FUNCTIONS_DLL_API double kth_smallest(double a[], int n, int k);

#define median(a,n) kth_smallest(a,n,(((n)&1)?((n)/2):(((n)/2)-1)))