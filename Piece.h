// Project 2: Soma Hypercube													Rob Leach
// Piece.h																		 5/5/2000
//---------------------------------------------------------------------------------------

#ifndef _PIECE_H_
#define _PIECE_H_

#include "DimensionalUnitTree.h"
#include <iostream>
#include <fstream>

class Piece
{
	public:
		Piece(){max_dims=max_units=0;top_dim=NULL;}	//Default c'tor - this is only used by
													//TranslationList.  Put this in private 
													//to see if anything else calls this by 
													//mistake
		
		Piece(const Piece&);						//Default Copy c'tor
		
		Piece(const int&,const int&,bool=false);	//Copy c'tor - takes number of 
													//dimensions and number of units per 
													//dimension
													
		~Piece(){delete top_dim;top_dim=NULL;max_dims=0;max_units=0;}
		
		//operator overloads
		Piece& operator=(const Piece&);
		bool operator==(const Piece&) const;
		bool operator!=(const Piece&) const;
		friend std::ostream& operator<<(std::ostream&,const Piece&);
		friend std::ifstream& operator>>(std::ifstream&,Piece&);
		friend std::istream& operator>>(std::istream&,Piece&);
		
		//Mutators - they are returning references so they can be chained up with other 
		//functions
		Piece& Normalize(int=0);					//Normalize a particular dimension

		Piece& Rotate(const int&,const int&);		//1, 90 degree rotation in a plane 
													//defined by 2 dimensions: The dimension 
													//that's being moved (MD) and the 
													//dimension it's being spread accross 
													//(DS).  All other dimension values don't 
													//change because they are perpendicular to 
													//these 2 dimensional axes Rotation is 
													//accomplished by moving  pointers around 
													//so higher dimension rotations move lots 
													//of values around in one pass.
													
		Piece& Translate(const int&);				//int is the dim_num in the struct.
													//This moves a piece to the next 
													//dimensional unit which is basically 
													//exchanging pointers in the
 
													//list<struct ...> in the indicated 
													//dimension
		
		//Member Functions / Accessors
		bool Collides(const Piece&) const;			//Determines if pieces overlap
		
		int GetDepth(const int&);					//Gets unit number of last occupied unit
													//in a dimension
		int GetShallow(const int&);
		
		int GetSize() const;						//Determines total number of 1D units 
													//that are present in a piece
		int GetMaxDims(){return max_dims;}
		
		int GetMaxUnits(){return max_units;}
	private:
		DimensionalUnitTree * top_dim;				//Pointer to struct Dim... that is the 
													//Highest dimension by which the piece 
													//is defined
		//All pieces in one puzzle will have these numbers.  Making them static saves space 
		//and facilitates reading in pieces from files or standart input
		int max_dims;						//Maximum number of dimensions
		int max_units;						//Maximum number of units per dimension
};

#endif
