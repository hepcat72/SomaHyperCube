// Project 2: Soma Hypercube													Rob Leach
// SolutionList.h																 5/5/2000
//---------------------------------------------------------------------------------------

#ifndef _SOLUTIONLIST_H_
#define _SOLUTIONLIST_H_

#include <iostream>
#include <list>
#include "Piece.h"
#include "PieceList.h"
#include "RotationList.h"
#include "TranslationList.h"
#include "SolutionState.h"
#include "Solution.h"

class SolutionList
{
	public:
		SolutionList(PieceList& rhs);

		class iterator
		{
			public:
				friend class SolutionList;
				
				//c'tors
				iterator(){}
				iterator(iterator&);
				
				//operators
				iterator& operator=(iterator&);
				iterator& operator++();
				iterator& operator--();				//just goes backward through soln_pool
													//if soln_pool hasn't been created, create 
													//it using ++
				bool operator!=(iterator& rhs);
													
				Solution& operator*();				//declare a solution with the last list in
													//the solution_pool
				
				//Mutators
				bool AddSolution(Solution&);		//Adds solutions, (piece lists), to the
													//solution_pool, but only if the 
													//solution isn't already there in 
													//another orientation
													
			private:
				std::list<RotationList> * starter;
				std::list<Solution> solution_pool;		//The only reason we keep this is so we 
				std::list<Solution>::iterator soln_iter;	//can see if solutions are the same, only
													//rotated
		};
		
		iterator first();							//returns the first solution_pool inside
													//an iterator.
		
		iterator last();							//returns an empty solution_pool in an 
													//iterator
	private:
		SolutionList(){}
		std::list<RotationList> rot_lists;
};

#endif
