// Project 2: Soma Hypercube													Rob Leach
// Solution.cpp																	 5/5/2000
//---------------------------------------------------------------------------------------

#include "Solution.h"

Solution::Solution(Solution& rhs)
{
	container = rhs.container;
}

bool Solution::PlacePiece(RotationList::iterator& rs,TranslationList::iterator& ts)
{
	bool success = false;

	//The following call to Collides is from the piece contained in the translation iterator
	std::list<struct SolutionState>::iterator i=container.begin();

	while(i != container.end() && ((*((*i).translation_state)).Collides(*ts)) == false)
		i++;
	
	//Seeing if a collision was discovered...	
	if(i == container.end())
	{
		//No collision occurred.  Declaring a new SolutionState...	
		SolutionState temp;
		//Setting the SolutionState for the new piece...	
		temp.rotation_state = rs;
		temp.translation_state = ts;
		//Adding piece to the container...
		container.push_back(temp);
		success = true;
	}
	return success;
}

bool Solution::RemovePiece()
{
	bool success = false;
	
	if(!container.empty())
	{
		//Removing piece from the container
		container.pop_back();
		success = true;
	}
	
	return success;
}
		
std::ostream& operator<<(std::ostream& os,Solution& rhs)
{
	std::list<struct SolutionState>::iterator i=rhs.container.begin();
	
	int dimmax = (*((*i).translation_state)).GetMaxDims();
	int unitmax = (*((*i).translation_state)).GetMaxUnits();
	
	Piece printer(dimmax,unitmax,true);
	TranslationList tl(printer);
	
	TranslationList::iterator pi=tl.first();
	int counter,total_units=1;
	while(pi != tl.last())
	{
		i=rhs.container.begin();
		counter=1;
		while( i != rhs.container.end() && !( (*pi).Collides(*((*i).translation_state)) ) )
		{
			++i;
			++counter;
		}
		if(i == rhs.container.end())
		{
			std::cerr<<"Solution.cpp:void Solution::print():Error: There appears to be an empty "
				<<"spot in this solution."<<std::endl;
			exit(1);
		}
		os<<counter;
		if(total_units % unitmax == 0)
		{
			os<<std::endl;
		}
		++total_units;
		++pi;
	}
	
	
	
	return os;
}
