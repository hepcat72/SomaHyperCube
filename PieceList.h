// Project 2: Soma Hypercube							Rob Leach
// PieceList.h									5/5/2000
//---------------------------------------------------------------------------------------

#ifndef _PIECELIST_H_
#define _PIECELIST_H_

#include <list>
#include "Piece.h"

class PieceList
{
	public:
		PieceList(){}
		PieceList(PieceList& rhs){pieces = rhs.pieces;}
		PieceList(std::list<Piece>& rhs){pieces = rhs;}
		
		friend std::ifstream& operator>>(std::ifstream&,PieceList&);
		friend std::istream& operator>>(std::istream&,PieceList&);
		
		class iterator
		{
			public:
				friend class PieceList;
				
				iterator(){}
				//iterator(iterator& rhs){data = rhs.data;}
				//iterator(std::list<Piece>::iterator& rhs){data = rhs;}
				iterator(iterator const& rhs) : data(rhs.data) {}
				iterator(std::list<Piece>::iterator rhs) : data(rhs) {}
				
				Piece operator*(){return *data;}
				iterator& operator=(iterator& rhs){data=rhs.data;return *this;}
				iterator& operator=(std::list<Piece>::iterator& rhs){data = rhs;return *this;}
				bool operator==(iterator& rhs){return data==rhs.data;}
				bool operator==(const iterator& rhs) const{return data==rhs.data;}
				bool operator!=(iterator rhs){return data != rhs.data;}
				iterator& operator++(){++data;return *this;}
				iterator& operator--(){--data;return *this;}
			private:
				std::list<Piece>::iterator data;
		};
		
		iterator first(){iterator i;i.data=pieces.begin();return i;}
		iterator last(){iterator i;i.data=pieces.end();return i;}
		
	private:
		std::list<Piece> pieces;
};

#endif
