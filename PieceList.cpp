// Project 2: Soma Hypercube													Rob Leach
// PieceList.cpp																 5/5/2000
//---------------------------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include "PieceList.h"

std::ifstream& operator>>(std::ifstream& ifs,PieceList& rhs)
{
	int temp,dimensions=0,depth=0,num_pieces=0,intro=1;
	char ch = 0;
	bool done = false;
	while(ifs)
	{
		do
		{
			if(!(ifs.get(ch)))
			{
				done = true;
				break;
			}
		}
		while(ch == '\n' || ch == ' ' || ch == '\t');

		if(done)
			break;

		if(intro <= 3)
		{
			switch(ch)
			{
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					ifs.putback(ch);
					ifs>>temp;
					if(temp == 0)
					{
						std::cerr<<"PieceList.cpp:ifstream& operator>>(std::ifstream& ifs,PieceList& rhs):"
							<<"Error: You cannot have zero dimensions,units per dimension, or "
							<<"pieces."<<std::endl;
						exit(1);
					}
					if(dimensions == 0)
						dimensions = temp;
					else if(depth == 0)
						depth = temp;
					else if(num_pieces == 0)
						num_pieces = temp;
					break;
				default:
				{
					std::cerr<<"PieceList.cpp:ifstream& operator>>(std::ifstream& ifs,PieceList& rhs):"
						<<"Error: Expected a number."<<std::endl;
					exit(1);
				}
			}
		}
		else
		{
			if(intro-3 <= num_pieces)
			{
				ifs.putback(ch);
				Piece next(dimensions,depth);
				ifs>>next;
				rhs.pieces.push_back(next);
			}
			else
			{
				std::cerr<<"PieceList.cpp:ifstream& operator>>(std::ifstream& ifs,PieceList& rhs):"
					<<"Error: Too much input in your input file.  You have "<<intro-3
					<<" pieces, which is more than the "
					<<num_pieces<<" pieces claimed in the header.  I found this character "
					<<"after the last piece: ["<<ch<<"]"<<std::endl;
				exit(1);
			}
		}
		intro++;
	}
	int actsize = 0,totalsize=depth;
	for(int index=1;index<dimensions;index++)
		totalsize *= depth;
	std::list<Piece>::iterator i=rhs.pieces.begin();
	while(i != rhs.pieces.end())
	{
		actsize += (*i).GetSize();
		i++;
	}
	if(actsize != totalsize)
	{
		std::cerr<<"PieceList.cpp:ifstream& operator>>(std::ifstream& ifs,PieceList& rhs):Error: You "
			<<"don't have enough or have too much piece material for a ";
		for(int x = 0;x < dimensions-1;x++)
			std::cerr<<depth<<"x";
		std::cerr<<depth<<" hypercube.  Total hypercube volume is "<<totalsize<<" and your pieces "
			<<"make up a total volume of "<<actsize<<std::endl;
		exit(1);
	}
	if(intro < num_pieces)
	{
		std::cerr<<"PieceList.cpp:ifstream& operator>>(std::ifstream& ifs,PieceList& rhs):Error: Not "
			<<"enough pieces in your infile."<<std::endl;
		exit(1);
	}
	return ifs;
}

std::istream& operator>>(std::istream& is,PieceList& rhs)
{
	int temp,dimensions=0,depth=0,num_pieces=0,intro=1;
	char ch = 0;
	bool done = false;
	while(is)
	{
		do
		{
			if(!(is.get(ch)))
			{
				done = true;
				break;
			}
		}
		while(ch == '\n' || ch == ' ' || ch == '\t');

		if(done)
			break;

		if(intro <= 3)
		{
			switch(ch)
			{
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					is.putback(ch);
					is>>temp;
					if(temp == 0)
					{
						std::cerr<<"PieceList.cpp:istream& operator>>(std::istream& is,PieceList& rhs):"
							<<"Error: You cannot have zero dimensions,units per dimension, or "
							<<"pieces."<<std::endl;
						exit(1);
					}
					if(dimensions == 0)
						dimensions = temp;
					else if(depth == 0)
						depth = temp;
					else if(num_pieces == 0)
						num_pieces = temp;
					break;
				default:
				{
					std::cerr<<"PieceList.cpp:istream& operator>>(std::istream& is,PieceList& rhs):"
						<<"Error: Expected a number."<<std::endl;
					exit(1);
				}
			}
		}
		else
		{
			if(intro-3 <= num_pieces)
			{
				is.putback(ch);
				Piece next(dimensions,depth);
				is>>next;
				rhs.pieces.push_back(next);
			}
			else
			{
				std::cerr<<"PieceList.cpp:istream& operator>>(std::istream& is,PieceList& rhs):"
					<<"Error: Too much input in your input file.  You have "<<intro-3
					<<" pieces, which is more than the "
					<<num_pieces<<" pieces claimed in the header.  I found this character "
					<<"after the last piece: ["<<ch<<"]"<<std::endl;
				exit(1);
			}
		}
		intro++;
	}
	
	int actsize = 0,totalsize = depth;
	for(int index=1;index<dimensions;index++)
		totalsize *= depth;
	std::list<Piece>::iterator i=rhs.pieces.begin();
	while(i != rhs.pieces.end())
	{
		actsize += (*i).GetSize();
		i++;
	}
	
	if(actsize != totalsize)
	{
		std::cerr<<"PieceList.cpp:istream& operator>>(std::istream& is,PieceList& rhs):Error: You "
			<<"don't have enough or have too much piece material for a ";
		for(int x = 0;x < dimensions-1;x++)
			std::cerr<<depth<<"x";
		std::cerr<<depth<<" hypercube.  Total hypercube volume is "<<totalsize<<" and your pieces "
			<<"make up a total volume of "<<actsize<<std::endl;
		exit(1);
	}

	if(intro < num_pieces)
	{
		std::cerr<<"PieceList.cpp:istream& operator>>(std::istream& is,PieceList& rhs):Error: Not "
			<<"enough pieces in your infile."<<std::endl;
		exit(1);
	}
	return is;
}
