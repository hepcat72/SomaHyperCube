// Project 2: Soma Hypercube													Rob Leach
// DimensionalUnitTree.cpp														 5/5/2000
//---------------------------------------------------------------------------------------

#include "DimensionalUnitTree.h"

DimensionalUnitTree::DimensionalUnitTree(int dimmax,int dim,int unitmax,int unit,bool print_helper)
{
	if(dimmax == dim)
	{
		parent_dim = NULL;
		if(print_helper)
			occupied = true;
		else
			occupied = false;
	}
	else
		occupied = false;
	dim_num = dim;
	unit_num = unit;
	
	if(print_helper && unit_num == 1 && parent_dim != NULL)
	{
		occupied = true;
	}
	else if(parent_dim != NULL)
		print_helper = false;

	int cur_unit;
	if(dim > 0)							     //last one's child_dims will be empty
		for(cur_unit = 1;cur_unit <= unitmax;cur_unit++)
		{
			child_dims.push_back(new DimensionalUnitTree(dimmax,dim-1,unitmax,cur_unit,print_helper));
			(child_dims.back())->parent_dim = this;
		}
}

DimensionalUnitTree::~DimensionalUnitTree()
{
	//Destruction must be initiated from the top_dim in the Piece class
	if(!child_dims.empty())
	{
		std::list<DimensionalUnitTree *>::iterator i = child_dims.begin();
		while(i != child_dims.end())		  //no new's were called in the last unit
		{		
			delete *i;
			++i;
		}
	}
}

DimensionalUnitTree& DimensionalUnitTree::operator=(DimensionalUnitTree& rhs)
{
	if(dim_num != rhs.dim_num || unit_num != rhs.unit_num)
	{
		std::cerr<<"Piece.cpp:DimensionalUnitTree& DimensionalUnitTree::operator=(const "
			<<"DimensionalUnitTree& rhs): DimensionalUnitTree conflict.  Only corresponding "
			<<"dimensions and units may be set equal to one another.  The left-hand object "
			<<"is in dimension "<<dim_num<<" and unit "<<unit_num<<" where-as the right-hand "
			<<"object is in dimension "<<rhs.dim_num<<" and unit "<<rhs.unit_num<<std::endl;
		exit(1);
	}
	occupied = rhs.occupied;
	
	//Take care of rest of tree structure
	if(rhs.dim_num > 0)					     //last one's child_dims will be empty
	{
		std::list<DimensionalUnitTree *>::iterator i = rhs.child_dims.begin();
		std::list<DimensionalUnitTree *>::iterator j = child_dims.begin();
		while(i != rhs.child_dims.end() && j != child_dims.end())
		{
			*(*j) = *(*i);				  //recursively calling 'this' operator=
			i++;j++;					    //both already have their own pointers
		}								       //parent_dim's are already set too
		if(i != rhs.child_dims.end() || j != child_dims.end())
		{
			std::cerr<<"Piece.cpp:DimensionalUnitTree& DimensionalUnitTree::operator=(const "
				<<"DimensionalUnitTree& rhs): DimensionalUnitTree conflict.  Only "
				<<"corresponding units may be set equal to one another.  These objects "
				<<"don't have the same number of units."<<std::endl;
			exit(1);
		}
	}
	return *this;
}

bool DimensionalUnitTree::operator==(DimensionalUnitTree& rhs)
{
	bool answer = true;

	if(occupied != rhs.occupied)
		answer = false;
	else
	{
		//Take care of rest of tree structure
		if(rhs.dim_num > 0)
		{
			std::list<DimensionalUnitTree *>::iterator i = rhs.child_dims.begin();
			std::list<DimensionalUnitTree *>::iterator j = child_dims.begin();
			while(i != rhs.child_dims.end() && j != child_dims.end())
			{
				if(!(answer = *(*j) == *(*i)))
					break;
				++i;
				++j;
			}
			if((i == rhs.child_dims.end() && j != child_dims.end()) || (i != rhs.child_dims.end() && j == child_dims.end()))
			{
				std::cerr<<"DImensionalUnitTree.cpp:bool DimensionalUnitTree::operator==("
					<<"DimensionalUnitTree& rhs) const:Error:Either your units or your dimensions "
					<<"aren't corresponding.  You can only compare the same dimensions and units.  "
					<<std::endl<<"Here are your units: "<<unit_num<<" and "<<rhs.unit_num<<std::endl<<"And "
					<<"here are your dimensions: "<<dim_num<<" and "<<rhs.dim_num<<std::endl;
				exit(1);
			}
		}
	}
	return answer;
}

std::ostream& operator<<(std::ostream& os,DimensionalUnitTree& rhs)
{
	std::list<DimensionalUnitTree *>::iterator i = rhs.child_dims.begin();
	if(rhs.dim_num > 0)				//last one's occupied's are the ones we want to print
	{
		if(rhs.occupied)
		{
			while(i != rhs.child_dims.end())
			{
				os<<*(*i);			//this will traverse down to dimension 1,1,1,.... first
				++i;
			}
			if(rhs.dim_num == 1)
				os<<std::endl;
		}
		else
		{
			//save time by printing blocks of oh's without traversing the tree
			int ohsperline = rhs.child_dims.size(),linesofohs = 1;
			for(int liner=rhs.dim_num;liner > 1;liner--)
				linesofohs *= ohsperline;
			int prntohs;
			for(int prntlns=1;prntlns<=linesofohs;prntlns++)
			{
				for(prntohs=1;prntohs<=ohsperline;prntohs++)
					os<<"0";
				os<<std::endl;
			}
		}
	}
	else							//then print the first, first dimension and on from there
	{
		if(rhs.occupied)
			os<<"1";
		else
			os<<"0";
	}
	return os;
}

std::ifstream& operator>>(std::ifstream& ifs,DimensionalUnitTree& rhs)
{
	//Construction is forced to use the 4-int route, just be careful I don't use any other
	//constructor that doesn't demand initialization of dim_num
	//This reads in from the dimension that's sent to it.
	//Check that # of units are right and that # of '\n''s is right

	char ch;
	//get the tree in the beginning position...
	std::list<DimensionalUnitTree *>::iterator i = rhs.child_dims.begin();
	if(rhs.dim_num > 0)
	{
		//Go down a dimension...
		while(i != rhs.child_dims.end())
		{
			ifs>>*(*i);				//this will traverse down to dimension 1,1,1,.... first
			++i;
		}
		if(rhs.dim_num == 1)
		{
			ifs.get(ch);
			if(ch != '\n')
			{
				std::cerr<<"Piece.cpp:ifstream& operator>>(std::ifstream& ifs,DimensionalUnitTree& rhs):"
					<<"Error: End-of-line expected after unit "<<rhs.unit_num<<" of dimension "
					<<rhs.dim_num<<std::endl;
				exit(1);
			}
		}
	}
	else
	{
		ifs.get(ch);
		if(ch == '1')
		{
			rhs.occupied = true;
			DimensionalUnitTree * climb = rhs.parent_dim;
			while(climb != NULL && !(climb->occupied))
			{
				climb->occupied = true;
				climb = climb->parent_dim;
			}
		}
		else if(ch == '0')
			rhs.occupied = false;
		else
		{
				std::cerr<<"Piece.cpp:ifstream& operator>>(std::ifstream& ifs,DimensionalUnitTree& "
					<<"rhs):Error: '0' or '1' for unit "<<rhs.unit_num<<" of dimension "
					<<rhs.dim_num<<std::endl;
				exit(1);
		}
	}
	return ifs;
}

std::istream& operator>>(std::istream& is,DimensionalUnitTree& rhs)
{
	//Construction is forced to use the 4-int route, just be careful I don't use any other
	//constructor that doesn't demand initialization of dim_num
	//This reads in from the dimension that's sent to it.
	//Check that # of units are right and that # of '\n''s is right

	char ch;
	//get the tree in the beginning position...
	std::list<DimensionalUnitTree *>::iterator i = rhs.child_dims.begin();
	if(rhs.dim_num > 0)
	{
		//Go down a dimension...
		while(i != rhs.child_dims.end())
		{
			is>>*(*i);					//this will traverse down to dimension 1,1,1,.... first
			++i;
		}
		if(rhs.dim_num == 1)
		{
			is.get(ch);
			if(ch != '\n')
			{
				std::cerr<<"DimensionalUnitTree.cpp:istream& operator>>(std::istream& is,"
					<<"DimensionalUnitTree& rhs):"
					<<"Error: End-of-line expected after unit "<<rhs.unit_num<<" of dimension "
					<<rhs.dim_num<<std::endl;
				exit(1);
			}
		}
	}
	else
	{
		is.get(ch);
		if(ch == '1')
		{
			rhs.occupied = true;
			DimensionalUnitTree * climb = rhs.parent_dim;
			while(climb != NULL && !(climb->occupied))
			{
				climb->occupied = true;
				climb = climb->parent_dim;
			}
		}
		else if(ch == '0')
			rhs.occupied = false;
		else
		{
				std::cerr<<"DimensionalUnitTree.cpp:istream& operator>>(std::istream& is,"
					<<"DimensionalUnitTree& "
					<<"rhs):Error: Only '0' or '1' is acceptable piece information for "
					<<"unit "<<rhs.unit_num<<" of dimension "<<rhs.dim_num<<".  "
					<<"You entered: ["<<ch<<"]"<<std::endl;
				exit(1);
		}
	}
	return is;
}

//When supplied a dimension, Normalize will only normalize that dimension.  Otherwise, 
//it normalize everything.
void DimensionalUnitTree::Normalize(int rhs)
{
	int max_size = child_dims.size(),times=0;
	std::list<std::list<DimensionalUnitTree *>*> starter;
	DimensionalUnitTree * temp;
	if(dim_num == rhs || rhs == 0)					//when rhs is 0, everything gets normalized
	{												//but you have to be careful and make sure
													//you send it the top_dim or else only part 
		while(!((child_dims.front())->occupied) && times < max_size)
		{											//pop empty child_dims off the front of
			temp = child_dims.front();				//the list and move them to the back.
			child_dims.pop_front();
			child_dims.push_back(temp);
			times++;
		}
		std::list<DimensionalUnitTree *>::iterator unit_fixer=child_dims.begin();
		times = 1;
		while(unit_fixer != child_dims.end())
		{
			(*unit_fixer)->unit_num = times;
			++times;
			++unit_fixer;
		}
	}
	if(dim_num > rhs)
	{
		//This spans the breadth of all existing sub dimensions...
		std::list<DimensionalUnitTree *>::iterator i = child_dims.begin();
		while(i != child_dims.end())
		{
			if((*i)->occupied)						//only concerned with occupied dimensions
				starter.push_back(&((*i)->child_dims)); 
			++i;
		}
		if(!starter.empty())
			Normalize(starter,rhs);
	}
}

void DimensionalUnitTree::Normalize(std::list<std::list<DimensionalUnitTree*>*> listolists,int dim)
{
	DimensionalUnitTree * temp;
	bool shift = false;
	int times=0,max_size=0;
	std::list<DimensionalUnitTree *>::iterator unit_fixer;
	std::list<std::list<DimensionalUnitTree*>*>::iterator i=listolists.begin();
													//dereferencing the iterator
													//yields pointers child_dims 
													//throughout the original tree
													//structure at one dimensional
													//level
	
	//Go through each of the pointed-to lists and find where each has its first occupied
	//only store the one that's closest to the beginning, because that's what determines how
	//much each one will move
	if(( (*i)->front() )->parent_dim->dim_num == dim || dim == 0)
	{
		//can't depend on child_dims size of current object, but can on parent's child_dims size
		max_size=( (*i)->front() )->parent_dim->child_dims.size();
		do
		{
			if(shift)
			{
				i=listolists.begin();
				while(i != listolists.end())
				{
					temp = (*i)->front();
					(*i)->pop_front();
					(*i)->push_back(temp);
					++i;
				}
				shift = false;
			}
			i=listolists.begin();
			while(i != listolists.end() && !((*i)->front())->occupied)
			{
				++i;
			}
			if(i == listolists.end())
				shift = true;
		}
		while(shift && times++ < max_size);
		
		//fix broken unit_num's...
		i=listolists.begin();
		while(i != listolists.end())
		{
			times = 1;
			unit_fixer = (*i)->begin();
			while(unit_fixer != (*i)->end())
			{
				(*unit_fixer)->unit_num = times;
				++times;
				++unit_fixer;
			}
			++i;
		}
		i=listolists.begin();
		if(( (*i)->front() )->dim_num == dim)
			return;
	}
	
	//Advance to the next dimension...
	if(( (*i)->front() )->parent_dim->dim_num > dim)
	{
		std::list<std::list<DimensionalUnitTree*>*> next;
		i=listolists.begin();
		while(i != listolists.end())
		{
			std::list<DimensionalUnitTree*>::iterator k=(*i)->begin();
			while(k != (*i)->end())
			{
				if((*k)->occupied)	    //push on pointers to the next occupied lists
					next.push_back( &( (*k)->child_dims) );
				++k;
			}
			++i;
		}
		if(!next.empty())
			Normalize(next,dim);
	}
}

DimensionalUnitTree& DimensionalUnitTree::Rotate(const int& move_dim,const int& dim_spread)
{
	DimensionalUnitTree temp(*this,true);//This copies the whole tree structure from the
										//top down no matter where *this is in the overall 
										//tree, into a fake version that has copied pointers
										//in addition to its own
	//Since I'm rotating each dimension individually, I can't be altering the calling 
	//object as I go.  I have to copy the object first and then change the original
	//based on it
	Rotate(move_dim,dim_spread,temp);

	ReOccupyParents(move_dim+1);		//This also takes care of fixing broken unit_num's

	return *this;						//due to rotation
}

DimensionalUnitTree& 
DimensionalUnitTree::
Rotate(const int& move_dim,const int& dim_spread,DimensionalUnitTree& temp)
{
	//This is the beginning of a recursive rotation.  In the first conditional, it will 
	//recursively go down the dimensions along one side until it gets to the dimension above 
	//the one that's being moved.  It will then manipulate the pointers that are to be moved
	//When it's done with that dimension, it will return to the previous dimension and move to
	//its next child and do the same thing all over again until we've made it all the way back
	//to the top dimension where it will just fall out of the conditional and return.
	
	std::list<DimensionalUnitTree *>::iterator i = child_dims.begin();
	
	if(dim_num > move_dim+1)		//since move_dim is actually the unit level of the 
	{								//dimension whose pointers we want to move, we want to 
									//deal with its parent_dim's child_dims list
									
		//This will call Rotate for all sub-dimensions that need to be rotated in parallel
		while(i != child_dims.end())
		{
			(*i)->Rotate(move_dim,dim_spread,temp);
			++i;
		}
	}
	else							//This is where rotation actually takes place
	{
		int maxunits = temp.child_dims.size();
		//Find where the rotation is taking place in the temp object by determining where we
		//are in the calling object...
		//Comming into this else means that dim_num is the dimension just above move_dim
		DimensionalUnitTree * placematch = &temp;
		DimensionalUnitTree * whereami;
		while(placematch->dim_num != dim_num)
		{
			//This can be made significantly shorter by pushing unit_num's onto a stack.
			//Come back and do that if I have time...
		
			//The next 2 while loops find out which child_dim placematch needs to advance to.
			//Have 'this' come up to meet placematch.  Stop just under where it is so we can
			//identify it by its unit_num
			whereami = this;
			while(whereami->parent_dim->dim_num != placematch->dim_num)
				whereami = whereami->parent_dim;
			std::list<DimensionalUnitTree *>::iterator advanceto = placematch->child_dims.begin();
			while((*advanceto)->unit_num != whereami->unit_num && advanceto != placematch->child_dims.end())
				advanceto++;
			placematch = *advanceto;
			//Now we need to go back and see if we're there yet.
		}
		//OK, now that placematch points to the same relative location that 'this' does, we
		//need to copy its fake pointers to children to the appropriate places for this 
		//rotation in 'this'
		
		DimensionalUnitTree * pm_restart = placematch;
		
		//Since we are at the moving dimension level, we first need to
		//determine what the new unit location is going to be and save it in a special variable.
		//This will be based on which unit we are moving.  If we are moving unit 1, we will be 
		//starting down unit/dimension one from the DS level.
		//Then we traverse up the placematch tree pushing each unit_num onto mirrorclimber as we 
		//go.  Next we need to start down the 'this' tree from the DS dimension, popping off the
		//first thing from mirrorclimber, saving it in another special variable, and using the 
		//special variable value we saved before - traverse down that unit.  Then we continue
		//popping off stuff from mirrorclimber in the same order (FILO), to determine 
		//which way to traverse down 'this'  We will also need a pointer to do these traversals.
		//On the last step, we break out that other special variable, and that's the object 
		//pointed to in child_dims that gets set equal to *newkid
		//There is a danger of a memory leak here, but if everything's done right, all lose 
		//memory will eventually still get reassigned elsewhere through this function
		
		int movetoslice,movetounit;
		std::list<int> mirrorclimber;
		//We need to go up 'this' to dim_spread to navigate down to where *newkid is going to 
		//go for each moving slice below
		DimensionalUnitTree *firstnav,*navigator = this,*new_parent;
		//Traversing 'this' up to dim_spread...
		while(navigator->dim_num != dim_spread)
			navigator = navigator->parent_dim;
		firstnav = navigator;
		std::list<DimensionalUnitTree *>::iterator newplace;
		std::list<DimensionalUnitTree *>::iterator newkid = pm_restart->copy.begin();
		for(int slice = 1;slice <= pm_restart->copy.size() && 
			newkid != pm_restart->copy.end();slice++)
		{
			//Moving a unit...
			//*newkid is the pointer I'm moving
			movetoslice = slice;    //movetoslice is the slice I start down from dim_spread
			//Bringing fake copy up to dim_spread and loading mirror climber with unit path...
			while(placematch->dim_num != dim_spread)
			{
				//Mirror Climber path push
				mirrorclimber.push_back(placematch->unit_num);
				placematch = placematch->parent_dim;
			}
			//movetounit is the position in 'this''s child_dims list where *newkid will be going
			movetounit = maxunits + 1 - mirrorclimber.back();
			//Changing the last mirrorclimber unit pathb value to movetoslice			
			mirrorclimber.pop_back();
			mirrorclimber.push_back(movetoslice);
			navigator = firstnav;
			//Traversing down this from dim_spread following the mirrorclimber path backwards...
			while(!mirrorclimber.empty())
			{
				//Moving to unit mirrorclimber.back()
				newplace = navigator->child_dims.begin();
				while((*newplace)->unit_num != mirrorclimber.back() && newplace != navigator->child_dims.end())
				{
					++newplace;
					//Probing for unit mirrorclimber.back() - on unit (*newplace)->unit_num
				}
				navigator = *newplace;
				mirrorclimber.pop_back();
			}
			//navigator is now a dimension above the one whose values are being changed.  We 
			//still need to get the appropriate location
			newplace = navigator->child_dims.begin();
			new_parent = navigator;

			int unit_count=1;
			while(newplace != navigator->child_dims.end() && unit_count != movetounit)
			{
				++newplace;
				++unit_count;
			}
			*newplace = *newkid;

			//Upgrading its unit_num to reflect the new location...			
			//fix the unit_num and parent_dim in the thing in 'this' that we're moving...
			(*newkid)->unit_num = movetounit;
			(*newkid)->parent_dim = new_parent;
			
			newkid++;
			placematch = pm_restart;
		}
	}

	return *this;
}

DimensionalUnitTree::DimensionalUnitTree(DimensionalUnitTree& rhs,bool first)
{
	//This copy c'tor is only to be used by Rotate just to be safe
	//It creates its own tree structure in child_dims, but saves the pointers of the tree
	//it's copying in the copy list of DimensionalUnitTree pointers

	//Copy the values of this dimension...
	if(first)								//top_dim gets a NULL parent pointer
		parent_dim = NULL;
	dim_num = rhs.dim_num;
	unit_num = rhs.unit_num;
	occupied = rhs.occupied;
	//Create the Tree structure and copy the pointers of the object sent in...
	if(rhs.dim_num > 0)
	{
		std::list<DimensionalUnitTree *>::iterator i=rhs.child_dims.begin();
		while(i != rhs.child_dims.end())
		{
			child_dims.push_back(new DimensionalUnitTree(**i,false));
			(child_dims.back())->parent_dim = this;
			copy.push_back(*i);
			i++;
		}
	}
}

DimensionalUnitTree& DimensionalUnitTree::Translate(const int& rhs)
{
	if(dim_num > rhs)			//rhs is the dimension I'm translating in, so I will be
	{							//changing the pointers in its child_dim as opposed to the
								//way in Rotate, I changed the pointers in the child_dim
								//move_dim was in.  This is confusing however and if I have 
								//time, I will go into rotate and change it so move_dim's
								//child_dims is the one that's manipulated.
		std::list<DimensionalUnitTree *>::iterator i=child_dims.begin();
		while(i != child_dims.end())
		{
			(*i)->Translate(rhs);
			i++;
		}
	}
	else
	{
		if((child_dims.back())->occupied)
		{
			std::cerr<<"DimensionalUnitTree.cpp:DimensionalUnitTree& DimensionalUnitTree::"
				<<"DimensionalUnitTree& Translate(const int& rhs):Error: Attempt to Translate "
				<<"past the bounds of the cube"<<std::endl;
			exit(1);
		}
		else
		{
			DimensionalUnitTree * temp = child_dims.back();
			child_dims.pop_back();
			child_dims.push_front(temp);
			
			//Fix the broken unit_num's...
			int times = 1;
			std::list<DimensionalUnitTree *>::iterator unit_fixer = child_dims.begin();
			while(unit_fixer != child_dims.end())
			{
				(*unit_fixer)->unit_num = times;
				++times;
				++unit_fixer;
			}
		}
	}
	return *this;
}

bool DimensionalUnitTree::Collides(DimensionalUnitTree& rhs)
{
	bool collision = false;
	std::list<DimensionalUnitTree *>::iterator i=child_dims.begin();
	std::list<DimensionalUnitTree *>::iterator j=rhs.child_dims.begin();
	if(dim_num > 1)  //I want to go through the child_dims in dimension 1 below
	{
		while(i != child_dims.end() && j != rhs.child_dims.end())
		{
			if((*i)->occupied && (*j)->occupied)
			{
				collision = (*i)->Collides(**j);
				if(collision)
					break;
			}
			i++;
			j++;
		}
	}
	else
	{
		while(i != child_dims.end() && j != rhs.child_dims.end())
		{
			if((*i)->occupied && (*j)->occupied)
				collision = true;
			i++;
			j++;
		}
	}
	return collision;
}

int DimensionalUnitTree::GetSize()
{
	int size = 0;
	std::list<DimensionalUnitTree *>::iterator i=child_dims.begin();
	if(dim_num > 1)  //I want to go through the child_dims in dimension 1 below
	{
		while(i != child_dims.end())
		{
			if((*i)->occupied)
				size += (*i)->GetSize();
			i++;
		}
	}
	else
	{
		while(i != child_dims.end())
		{
			if((*i)->occupied)
				size++;
			i++;
		}
	}
	return size;
}

int DimensionalUnitTree::GetDepth(const int& rhs)
{
	int depth;
	if(rhs < 1 || rhs > dim_num)
	{
		std::cerr<<"DimensionalUnitTree.cpp:int DimensionalUnitTree::GetDepth(const int& rhs):"
			<<"Error: "<<rhs<<" is not a dimension from whom a depth can be obtained."<<std::endl;
		exit(1);
	}
	if(dim_num == rhs)
	{
		std::list<DimensionalUnitTree *>::iterator backwards=child_dims.end();
		backwards--;
		while(!((*backwards)->occupied))
		{
			if(backwards == child_dims.begin())
				break;
			else
				backwards--;
		}
		if((*backwards)->occupied)
		{
			depth = (*backwards)->unit_num;
		}
		else
		{
			std::cerr<<"DimensionalUnitTree.cpp:int DimensionalUnitTree::GetDepth(const int& rhs):"
				<<"Error: Either your piece is completely empty or you called this function "
				<<"from something other than the top dimension.  You sent dimension "<<dim_num
				<<"."<<std::endl;
			exit(1);
		}
	}
	else
	{
		//This spans the breadth of all existing sub dimensions...
		std::list<std::list<DimensionalUnitTree *>*> starter;
		std::list<DimensionalUnitTree *>::iterator i = child_dims.begin();
		for(int cur_unit = 1;cur_unit <= child_dims.size();cur_unit++,i++)
		{
			if((*i)->occupied)      //only concerned with occupied dimensions
				starter.push_back(&((*i)->child_dims));
		}
		if(!starter.empty())
		{
			depth = GetDepth(starter,rhs);
		}
		else
		{
			std::cerr<<"DimensionalUnitTree.cpp:int DimensionalUnitTree::GetDepth(const int& rhs):"
				<<"Error: Either your piece is completely empty or you called this function "
				<<"from something other than the top dimension.  You sent dimension "<<dim_num
				<<"."<<std::endl;
			exit(1);
		}
	}
	return depth;
}

int DimensionalUnitTree::GetDepth(std::list<std::list<DimensionalUnitTree*>*> listolists,const int& dim)
{
	int depth=0;
	std::list<std::list<DimensionalUnitTree*>*>::iterator i=listolists.begin();

	//Go through each of the pointed-to lists and find where each has its last occupied 
	//position.  Only store the one that's closest to the end, because that's what 
	//determines how deep this piece goes into that dimension
	if(( (*i)->front() )->parent_dim->dim_num == dim)
	{
		i=listolists.begin();
		while(i != listolists.end() && depth != (*i)->size())
		{
			//Going through the listolists...
			std::list<DimensionalUnitTree *>::iterator j=(*i)->end();
			--j;
			while(!((*j)->occupied) && j != (*i)->begin())
			{
				--j;
			}

			if((*j)->occupied)
			{
				if(depth < (*j)->unit_num)
					depth = (*j)->unit_num;
			}
			else
			{
				std::cerr<<"DimensionalUnitTree.cpp:int DimensionalUnitTree::GetDepth"
					<<"(list<list<DimensionalUnitTree*>*> listolists,const int& dim):"
					<<"Error: Expected dimension to be occupied, but it doesn't "
					<<"appear to be that way.  I'll bet either Normalize, Rotate, "
					<<"Translate, or some other mutator didn't deal with parent_dims'"
					<<"occupied values properly."<<std::endl;
				exit(1);
			}
			i++;
		}
	}
	else
	{
		std::list<std::list<DimensionalUnitTree*>*> next;
		i=listolists.begin();
		while(i != listolists.end())
		{
			std::list<DimensionalUnitTree*>::iterator k=(*i)->begin();
			while(k != (*i)->end())
			{
				if((*k)->occupied)	    //push on pointers to the next occupied lists
					next.push_back( &( (*k)->child_dims) );
				++k;
			}
			++i;
		}
		depth = GetDepth(next,dim);
	}
	return depth;
}

int DimensionalUnitTree::GetShallow(const int& rhs)
{
	int shallow;
	if(rhs < 1 || rhs > dim_num)
	{
		std::cerr<<"DimensionalUnitTree.cpp:int DimensionalUnitTree::GetShallow(const int& rhs):"
			<<"Error: "<<rhs<<" is not a dimension from whom a shallow can be obtained."<<std::endl;
		exit(1);
	}
	if(dim_num == rhs)
	{
		std::list<DimensionalUnitTree *>::iterator forwards=child_dims.begin();
		while(!((*forwards)->occupied))
		{
			if(forwards == child_dims.end())
				break;
			else
				forwards++;
		}

		if((*forwards)->occupied)
		{
			shallow = (*forwards)->unit_num;
		}
		else
		{
			std::cerr<<"DimensionalUnitTree.cpp:int DimensionalUnitTree::GetShallow(const int& rhs):"
				<<"Error: Either your piece is completely empty or you called this function "
				<<"from something other than the top dimension.  You sent dimension "<<dim_num
				<<"."<<std::endl;
			exit(1);
		}
	}
	else
	{
		//This spans the breadth of all existing sub dimensions...
		std::list<std::list<DimensionalUnitTree *>*> starter;
		std::list<DimensionalUnitTree *>::iterator i = child_dims.begin();
		for(int cur_unit = 1;cur_unit <= child_dims.size();cur_unit++,i++)
		{
			if((*i)->occupied)      //only concerned with occupied dimensions
				starter.push_back(&((*i)->child_dims));
		}
		if(!starter.empty())
		{
			shallow = GetShallow(starter,rhs);
		}
		else
		{
			std::cerr<<"DimensionalUnitTree.cpp:int DimensionalUnitTree::GetShallow(const int& rhs):"
				<<"Error: Either your piece is completely empty or you called this function "
				<<"from something other than the top dimension.  You sent dimension "<<dim_num
				<<"."<<std::endl;
			exit(1);
		}
	}
	return shallow;
}

int DimensionalUnitTree::GetShallow(std::list<std::list<DimensionalUnitTree*>*> listolists,const int& dim)
{
	int shallow=0;
	std::list<std::list<DimensionalUnitTree*>*>::iterator i=listolists.begin();

	//Go through each of the pointed-to lists and find where each has its first occupied 
	//position.  Only store the one that's closest to the end, because that's what 
	//determines how deep this piece goes into that dimension
	if(( (*i)->front() )->parent_dim->dim_num == dim)
	{
		i=listolists.begin();
		while(i != listolists.end() && shallow != 1)
		{
			std::list<DimensionalUnitTree *>::iterator j=(*i)->begin();
			while(!((*j)->occupied) && j != (*i)->end())
				j++;
			if((*j)->occupied)
			{
				if(shallow > (*j)->unit_num)
					shallow = (*j)->unit_num;
			}
			else
			{
				std::cerr<<"DimensionalUnitTree.cpp:int DimensionalUnitTree::GetDepth"
					<<"(list<list<DimensionalUnitTree*>*> listolists,const int& dim):"
					<<"Error: Expected dimension to be occupied, but it doesn't "
					<<"appear to be that way.  I'll bet either Normalize, Rotate, "
					<<"Translate, or some other mutator didn't deal with parent_dims'"
					<<"occupied values properly."<<std::endl;
				exit(1);
			}
			i++;
		}
	}
	else
	{
		std::list<std::list<DimensionalUnitTree*>*> next;
		i=listolists.begin();
		while(i != listolists.end())
		{
			std::list<DimensionalUnitTree*>::iterator k=(*i)->begin();
			while(k != (*i)->end())
				if((*k)->occupied)	    //push on pointers to the next occupied lists
					next.push_back( &( (*k)->child_dims) );
			++i;
		}
		shallow = GetShallow(next,dim);
	}
	return shallow;
}

void DimensionalUnitTree::ReOccupyParents(int dim)
{
	DimensionalUnitTree * ptr = this;
	while(ptr->parent_dim != NULL)
		ptr = ptr->parent_dim;
	std::list<DimensionalUnitTree *>::iterator subdim=ptr->child_dims.begin();
	if(ptr->dim_num == dim)	  //base case - fix the occupied value of the top dimension
	{
		//Fixing only the top_dim...
		while(subdim != ptr->child_dims.end() && !((*subdim)->occupied))
			subdim++;
		if(subdim != ptr->child_dims.end())
			ptr->occupied = true;
		else
		{
			std::cerr<<"DimensionalUnitTree.cpp:void DimensionalUnitTree::ReOccupyParents(const "
				<<"int& dim):Unknown Error: Somehow, you ended up with an empty piece."<<std::endl;
			exit(1);
		}
	}
	else						//move down to next dimension by calling helper
	{
		std::list<std::list<DimensionalUnitTree *>*> starter;
		//Building list of child_dims lists...
		while(subdim != ptr->child_dims.end())
		{
			starter.push_back(&((*subdim)->child_dims));
			++subdim;
		}
		ReOccupyParents(starter,dim);
	}
}

void DimensionalUnitTree::ReOccupyParents(std::list<std::list<DimensionalUnitTree *>*> listolists, const int& dim)
{
	bool reoccupier = false;
	std::list<std::list<DimensionalUnitTree*>*>::iterator i=listolists.begin();
	if(( (*i)->front() )->parent_dim->dim_num == dim)	//fix this dimension's occupied
	{													//values if we've hit the dimension 
		i=listolists.begin();							//the user wants to fix occupied 
		while(i != listolists.end())					//values from and move up to the 
		{												//next dimension to fix those
			std::list<DimensionalUnitTree *>::iterator j=(*i)->begin();
			while(j != (*i)->end() && !((*j)->occupied))
				++j;
			if(j != (*i)->end())
				reoccupier = true;
			else
				reoccupier = false;
			(*((*i)->begin()))->parent_dim->occupied = reoccupier;
			
			i++;
		}
		if(( (*(listolists.begin()))->front() )->parent_dim->parent_dim != NULL)
			ReOccupyParents(dim + 1);
	}
	else												//We haven't reached the dimension 
	{													//yet where changes in occupied 
		std::list<std::list<DimensionalUnitTree*>*> next;			//values need to change, so move 
		i=listolists.begin();							//down to the next dimension by
		while(i != listolists.end())					//recursion
		{
			//Getting next group of child_dims lists
			std::list<DimensionalUnitTree*>::iterator k=(*i)->begin();
			while(k != (*i)->end())
			{
				next.push_back( &( (*k)->child_dims) );
				++k;
			}
			++i;
		}
		ReOccupyParents(next,dim);
	}
}
