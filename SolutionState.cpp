// Project 2: Soma Hypercube													Rob Leach
// SolutionState.cpp															 5/5/2000
//---------------------------------------------------------------------------------------

#include "SolutionState.h"

SolutionState::SolutionState(SolutionState& rhs)
{
	rotation_state=rhs.rotation_state;
	translation_state=rhs.translation_state;
}

SolutionState::SolutionState(const SolutionState& rhs)
{
	rotation_state=rhs.rotation_state;
	translation_state=rhs.translation_state;
}

SolutionState& SolutionState::operator=(SolutionState& rhs)
{
	rotation_state=rhs.rotation_state;
	translation_state=rhs.translation_state;
	return *this;
}

SolutionState& SolutionState::operator=(const SolutionState& rhs)
{
	rotation_state=rhs.rotation_state;
	translation_state=rhs.translation_state;
	return *this;
}
