// Project 2: Soma Hypercube													Rob Leach
// RotationList.h																 5/5/2000
//---------------------------------------------------------------------------------------

#ifndef _ROTATIONLIST_H_
#define _ROTATIONLIST_H_

#include <list>
#include "Piece.h"

class RotationList
{
	public:
		RotationList(Piece&);
		RotationList(RotationList& rhs){rotations=rhs.rotations;}
		RotationList(const RotationList& rhs){rotations=rhs.rotations;}
		
		class iterator
		{
			public:
				friend class RotationList;
			
				iterator(){}
				//iterator(iterator& rhs){data = rhs.data;}
				//iterator(std::list<Piece>::iterator& rhs){data = rhs;}
				iterator(iterator const& rhs) : data(rhs.data) {}
				iterator(std::list<Piece>::iterator rhs) : data(rhs) {}
						
				iterator& operator=(iterator& rhs){data=rhs.data;return *this;}
				iterator& operator=(const iterator& rhs){data=rhs.data;return *this;}
				iterator& operator=(std::list<Piece>::iterator& rhs){data = rhs;return *this;}
				bool operator==(const iterator& rhs) const{return data == rhs.data;}
				bool operator!=(iterator const& rhs){return data != rhs.data;}
				Piece& operator*(){return *data;}
				iterator& operator++(){++data;return *this;}
				iterator& operator--(){--data;return *this;}
			private:
				std::list<Piece>::iterator data;
		};

		iterator first(){iterator i;i.data=rotations.begin();return i;}
		iterator last(){iterator i;i.data=rotations.end();return i;}
		
	private:
		std::list<Piece> rotations;
		RotationList(){}
};

#endif
