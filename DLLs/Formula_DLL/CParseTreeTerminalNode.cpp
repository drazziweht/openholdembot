//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: base class for numbers, identifiers ... of the parse-tree
//
//******************************************************************************

#include "CParseTreeTerminalNode.h"

CParseTreeTerminalNode::CParseTreeTerminalNode(int relative_line_number) : 
    CParseTreeNode(relative_line_number) {
}

CParseTreeTerminalNode::~CParseTreeTerminalNode() {
}