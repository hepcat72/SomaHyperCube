// Project 2: Soma Hypercube													Rob Leach
// DimensionalUnitTree.h														 5/5/2000
//---------------------------------------------------------------------------------------

#ifndef _DIMENSIONALUNITTREE_H_
#define _DIMENSIONALUNITTREE_H_

#include <list>
#include <iostream>
#include <fstream>

class DimensionalUnitTree
{
	public:
		DimensionalUnitTree(int,int,int,int=0,bool=false);//c'tor - actually, this is a 
														//special c'tor that creates a whole 
														//tree of DimensionalUnitTree's 
														//recursively.  Takes max_dims,
														//dim_num,max_units,cur_unit
		//No default copy c'tor.  This class is intended to be for the exclusive use of the 
		//Piece class which has a default copy c'tor
		~DimensionalUnitTree();

		//operators...
		DimensionalUnitTree& operator=(DimensionalUnitTree&);
		bool operator==(DimensionalUnitTree&);
		friend std :: ostream& operator<<(std :: ostream&,DimensionalUnitTree&);
		friend std :: ifstream& operator>>(std :: ifstream&,DimensionalUnitTree&);
		friend std :: istream& operator>>(std :: istream&,DimensionalUnitTree&);
		
		//Mutators.  There's 2 of most of everything because they call eachother.  The 
		//first one is like a primer which calls the next which works recursively until 
		//done
		void Normalize(int=0);
		void Normalize(std::list<std::list<DimensionalUnitTree*>*>,int=0);
		
		DimensionalUnitTree& Rotate(const int&,const int&);
		DimensionalUnitTree& Rotate(const int&,const int&,DimensionalUnitTree&);
														//The above DimensionalUnitTree is
														//a fake copy of a real one.  It
														//maintains the structure and 
														//pointers of the one it's a fake 
														//copy of.  See special copy c'tor 
														//and data under private...
		
		DimensionalUnitTree& Translate(const int&);
		
		//Accessors...
		bool Collides(DimensionalUnitTree&);
		
		int GetSize();
		
		int GetDepth(const int&);
		int GetDepth(std::list<std::list<DimensionalUnitTree*>*>,const int&);
		
		int GetShallow(const int&);
		int GetShallow(std::list<std::list<DimensionalUnitTree*>*>,const int&);
		
	private:
		DimensionalUnitTree * parent_dim;				//pointer to parent dimension
		std::list<DimensionalUnitTree *> child_dims;			//Pointers to child dimensions
		int dim_num;								    //Dimension indicator
		int unit_num;									//Unit indicator
		bool occupied;									//Unit Value	true=occupied
														//				false=not occupied
		
		void ReOccupyParents(int=1);				    //special mutator used by Rotate
		void ReOccupyParents(std::list<std::list<DimensionalUnitTree *>*>, const int&);
		
		//These are for copying purposes for use by rotate only to generate fake copies...
		DimensionalUnitTree(DimensionalUnitTree&,bool);
		std::list<DimensionalUnitTree *> copy;
};

#endif
