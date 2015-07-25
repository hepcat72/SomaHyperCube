// Project 2: Soma Hypercube													Rob Leach
// Piece.cpp																	 5/5/2000
//---------------------------------------------------------------------------------------

#include "Piece.h"

//C'tors and D'tors
Piece::Piece(const Piece& rhs)
{
	top_dim = new DimensionalUnitTree(rhs.max_dims,rhs.max_dims,rhs.max_units);
	*top_dim = *(rhs.top_dim);
	max_dims = rhs.max_dims;
	max_units = rhs.max_units;
}

Piece::Piece(const int& dimmax,const int& unitmax,bool print_helper)
{
	top_dim = new DimensionalUnitTree(dimmax,dimmax,unitmax,0,print_helper);
	max_dims = dimmax;
	max_units = unitmax;
}

//operator overloads
Piece& Piece::operator=(const Piece& rhs)
{
	if(max_dims > 0)
		delete top_dim;
	else
	{
		max_dims = rhs.max_dims;
		max_units = rhs.max_units;
	}

	top_dim = new DimensionalUnitTree(rhs.max_dims,rhs.max_dims,rhs.max_units);
	*top_dim = *(rhs.top_dim);
	return *this;
}

bool Piece::operator==(const Piece& rhs) const
{
	return *top_dim == *(rhs.top_dim);
}

bool Piece::operator!=(const Piece& rhs) const
{
	return !(*this == rhs);
}

std::ostream& operator<<(std::ostream& os,const Piece& rhs)
{
	os<<*(rhs.top_dim);
	return os;
}

std::ifstream& operator>>(std::ifstream& ifs,Piece& rhs)
{
	//loop reads based on:max_dims & max_units.
	//Doesn't read in initial number indicators at beginning of file.
	//That's done by PieceList.
	ifs>>*(rhs.top_dim);
	(rhs.top_dim)->Normalize();
	return ifs;
}

std::istream& operator>>(std::istream& is,Piece& rhs)
{
	//loop reads based on:max_dims & max_units.
	//Doesn't read in initial number indicators at beginning of file.
	//That's done by PieceList.
	is>>*(rhs.top_dim);
	(rhs.top_dim)->Normalize();
	return is;
}

//Mutators - they are returning references so they can be chained up with other 
//functions
Piece& Piece::Rotate(const int& MD, const int& DS)
{
	top_dim->Rotate(MD,DS);
	return *this;
}

Piece& Piece::Translate(const int& rhs)
{
	top_dim->Translate(rhs);
	return *this;
}

//Member Functions / Accessors
bool Piece::Collides(const Piece& rhs) const
{
	bool collides = top_dim->Collides(*(rhs.top_dim));
	return collides;
}

int Piece::GetSize() const
{
	int size = top_dim->GetSize();
	return size;
}

Piece& Piece::Normalize(int rhs)
{
	top_dim->Normalize(rhs);
	return *this;
}

int Piece::GetDepth(const int& rhs)
{
	return top_dim->GetDepth(rhs);
}

int Piece::GetShallow(const int& rhs)
{
	return top_dim->GetShallow(rhs);
}
