// Project 2: Soma Hypercube													Rob Leach
// SolutionState.h																 5/5/2000
//---------------------------------------------------------------------------------------

#ifndef _SOLUTIONSTATE_H_
#define _SOLUTIONSTATE_H_

#include "RotationList.h"
#include "TranslationList.h"

struct SolutionState
{
	SolutionState(){}
	SolutionState(SolutionState&);
	SolutionState(const SolutionState&);
	SolutionState& operator=(SolutionState&);
	SolutionState& operator=(const SolutionState&);
	
	RotationList::iterator rotation_state;
	TranslationList::iterator translation_state;
};

#endif
