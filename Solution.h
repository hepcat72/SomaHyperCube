// Project 2: Soma Hypercube													Rob Leach
// Solution.h																	 5/5/2000
//---------------------------------------------------------------------------------------

#ifndef _SOLUTION_H_
#define _SOLUTION_H_

#include <iostream>
#include <list>
#include "Piece.h"
#include "RotationList.h"
#include "TranslationList.h"
#include "SolutionState.h"

class Solution
{
	public:
		//C'tors
		Solution(){}
		Solution(Solution&);
		Solution(const Solution& rhs){container=rhs.container;}
		
		//Mutators
		bool PlacePiece(RotationList::iterator&,TranslationList::iterator&);
		bool RemovePiece();								//these return success status
		
		//Accessors
		struct SolutionState& GetLastPieceState(){return container.back();}
		std::list<struct SolutionState>& GetContainer(){return container;}

		//Operators
		friend std::ostream& operator<<(std::ostream&,Solution&);	
	private:
		std::list<struct SolutionState> container;
};

#endif
