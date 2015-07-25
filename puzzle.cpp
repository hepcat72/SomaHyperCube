// Project 2: Soma Hypercube													Rob Leach
// puzzle.cpp (main)															 5/5/2000
//---------------------------------------------------------------------------------------

#include <iostream>
#include <fstream>
std::ifstream fin;
#include <list>
#include "DimensionalUnitTree.h"
#include "Piece.h"
#include "PieceList.h"
#include "RotationList.h"
#include "TranslationList.h"
#include "SolutionState.h"
#include "Solution.h"
#include "SolutionList.h"
#include <ctype.h>

int main(int argc,char* argv[])
{
	int max = 0;
	if(argc > 2)
	{
		std::cerr<<"puzzle.cpp:int main(int argc,char* argv[]):Error: Too many arguments!: "
			<<argc-1<<" arguments detected, (";
		int x;
		for(x=1;x<argc-1;x++)
		{
			std::cerr<<argv[x]<<", ";
		}
		std::cerr<<argv[x]<<").  Only 1 is acceptable"<<std::endl;
		exit(1);
	}
	else if(argc == 2)
	{
		max = atoi(argv[1]);
	}
	else
		max = -1;

	PieceList mypieces;
	std::cin>>mypieces;
	
	SolutionList mysolutions(mypieces);
	SolutionList::iterator i=mysolutions.first();

	for(int index=0;index != max && i != mysolutions.last();index++)
	{
		std::cout<<std::endl<<"Solution "<<index+1<<" is"<<std::endl<<std::endl;
		std::cout<<*i;
		++i;
	}
	std::cout<<std::endl;

	return 0;
}
