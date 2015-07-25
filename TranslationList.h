// Project 2: Soma Hypercube													Rob Leach
// TranslationList.h															 5/5/2000
//---------------------------------------------------------------------------------------

#ifndef _TRANSLATIONLIST_H_
#define _TRANSLATIONLIST_H_

#include "Piece.h"

class TranslationList
{
	public:
		TranslationList(Piece& rhs):original(rhs){}
		TranslationList(TranslationList& rhs):original(rhs.original){}
		TranslationList& operator=(TranslationList& rhs){original = rhs.original;return *this;}

		class iterator
		{
			public:
				friend class TranslationList;
				iterator(){}
				//iterator(iterator&);
				iterator(iterator const&){}
				
				Piece operator*();
				iterator& operator++();
				//iterator& operator=(iterator&);			 
				iterator& operator=(const iterator&);			 
				bool operator!=(iterator const&);

			private:
				Piece * starter;
				Piece translator;
				bool last;
		};
		
		iterator first();
		iterator last();

	private:
		Piece original;
		TranslationList(){}
};

#endif
