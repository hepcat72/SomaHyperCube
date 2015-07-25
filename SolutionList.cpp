// Project 2: Soma Hypercube													Rob Leach
// SolutionList.cpp																 5/5/2000
//---------------------------------------------------------------------------------------

#include <iostream>
#include "SolutionList.h"

SolutionList::SolutionList(PieceList& rhs)
{
	PieceList::iterator i=rhs.first();

	while(i != rhs.last())
	{
		Piece tmp(*i);
		RotationList temp(tmp);
		rot_lists.push_back(temp);
		++i;
	}
}

SolutionList::iterator SolutionList::first()
{
	Solution first_soln;
	struct SolutionState temp;
	
	iterator init;
	init.starter = &rot_lists;

	std::list<RotationList>::iterator piece=rot_lists.begin();
	RotationList::iterator rs=(*piece).first();
	TranslationList tl(*rs);

	TranslationList::iterator ts=tl.first();
	bool trythis = false;
	while(piece != rot_lists.end())
	{
		//Trying next RotationList of next piece...
		while(rs != (*piece).last())
		{
			//Trying a rotation of current piece...
			while(ts != tl.last())
			{
				//Trying a translation of current piece...
				//Attepmting to place current piece...
				if(first_soln.PlacePiece(rs,ts))
				{
				 	trythis = true;
				 	break;
				}
				++ts;
			}
			if(trythis)
			{
				trythis = false;
				break;
			}
			++rs;
			if(rs != (*piece).last())
			{
				TranslationList temp(*rs);
				tl = temp;
				ts = tl.first();
			}
		}
		if((rs == (*piece).last()) && (piece == rot_lists.begin()))
		{
			std::cerr<<"SolutionList.cpp:iterator& SolutionList::first():Error: There are no "
				<<"solutions to this puzzle!"<<std::endl;
			exit(1);
		}
		else if(rs == (*piece).last())	//Piece wasn't placed in any orientation. Recursing to
		{								//previous piece
			--piece;
			//Getting last piece state...
			temp = first_soln.GetLastPieceState();
			//Removing piece from solution...
			first_soln.RemovePiece();
			//Restoring rotation state...
			rs = temp.rotation_state;
			//Resetting the TranslationList...
			TranslationList tmp(*rs);
			tl = tmp;
			//Restoring the TranslationList...
			ts = temp.translation_state;
			//Advancing to the next translation...
			++ts;
		}
		else							//Piece was placed.  Advancing to next piece...
		{
			++piece;
			if(piece != rot_lists.end())
			{
				rs=(*piece).first();
				TranslationList tmp(*rs);
				tl = tmp;
				//Initializing the translationList...
				ts = tl.first();
			}
		}
		
		//If we've reached the a possible new solution, check to see if it's unique...
		if(piece == rot_lists.end())
		{
			init.AddSolution(first_soln);
		}
	}
	//first_soln has been added to the solution_pool from AddSolution above, so now we must 
	//initialize the soln_iter and return *this...
	init.soln_iter = init.solution_pool.begin();
	return init;
}

SolutionList::iterator::iterator(iterator& rhs)
{
	starter = rhs.starter;
	solution_pool = rhs.solution_pool;
	soln_iter = solution_pool.begin();
	std::list<Solution>::iterator i=rhs.solution_pool.begin();
	while(i != rhs.soln_iter)
	{
		++i;
		++soln_iter;
	}
}

SolutionList::iterator& SolutionList::iterator::operator=(iterator& rhs)
{
	starter = rhs.starter;
	solution_pool = rhs.solution_pool;
	soln_iter = solution_pool.begin();
	std::list<Solution>::iterator i=rhs.solution_pool.begin();
	while(i != rhs.soln_iter)
	{
		++i;
		++soln_iter;
	}
	return *this;
}

SolutionList::iterator& SolutionList::iterator::operator++()
{
	//Operator++ Notes:
	//iterator& operator++(iterator&)
	//use an int current to keep track of which piece in PieceList I'm on
	//declare a Piece, copy out the 'next' piece from PieceList, do the rotations and 
	//translations indicated by the 'next' SolutionState in data.  PlacePiece - if doesn't place, 
	//print an error and exit.  loop back until on last piece of PieceList at which point, we drop 
	//out of the loop and into solution search mode...  increment the piece_translation in the last
	//struct of data
	//
	//start a loop
	//declare a Piece and copy out the current piece from PieceList.  rotate and translate it 
	//according to the corresponding data element, but do it in a loop stipulating that the loop 
	//stops if at the end of the translations/rotations.  If the number of times through the loop
	//is not equal to what's in the current data element - decrement that data element if it's a 
	//translation and increment the rotation and try again.  If the one that's not equal is the 
	//rotations, pop the struct off, decrement current, increment the piece_translation in the last 
	//data element, and loop back to the beginning.  Else -  Try to PlacePiece.  If it places, 
	//increment current, push_back a new struct SolutionState with values of 1 onto data and loop 
	//back.  If it doesn't place, increment the corresponding piece_translation in data and loop 
	//back.

	++soln_iter;
	if(soln_iter == solution_pool.end())
	{
		//soln_iter is at end, Attempting to generate another solution..."<<std::endl;	
		--soln_iter;
		
		Solution next_soln(solution_pool.back());
		struct SolutionState temp;

		std::list<RotationList>::iterator piece=starter->end();
		--piece;
		//Preserving the state of the last piece in the last solution...	
		temp = next_soln.GetLastPieceState();
		//Removing the last piece from the new solution...	
		next_soln.RemovePiece();
		//Declaring a RotationList iterator and restoring the last piece's rotation state...	
		RotationList::iterator rs = temp.rotation_state;
		//Declaring a new translationList with the current rotation...	
		TranslationList tl(*rs);
		//Restoring the last translation state...	
		TranslationList::iterator ts = temp.translation_state;
		//Incrementing to try the next translation state...
		++ts;
		
		bool trythis = false;
		while(piece != starter->end())
		{
			//Trying 'next' piece...
			while(rs != (*piece).last())
			{
				//Trying 'next' rotation...
				while(ts != tl.last())
				{
					//Trying 'next' translation...
					if(next_soln.PlacePiece(rs,ts))
					{
						//Piece placed!...	
					 	trythis = true;
					 	break;
					}
					 ++ts;
				}
				if(trythis)
				{
					trythis = false;
					break;
				}
				++rs;
				if(rs != (*piece).last())
				{
					TranslationList tmp(*rs);
					tl = tmp;
					ts = tl.first();
				}
			}
			if(rs == (*piece).last() && piece == starter->begin())
			{								//Makes it equal to end because of the ++soln_iter
				//Out of pieces.  No more solutions.
				break;						//below and because I haven't added any more 
			}								//solutions.  There are none left.
			else if(rs == (*piece).last())
			{								//Piece wasn't placed in any orientation. Recursing
				--piece;					//to previous piece
				temp = next_soln.GetLastPieceState();
				next_soln.RemovePiece();
				rs = temp.rotation_state;
				TranslationList tmp(*rs);
				tl = tmp;
				ts = temp.translation_state;
				++ts;						//trying next translation
			}
			else							//Piece was placed.  Advancing to next piece...
			{
				++piece;
				if(piece != starter->end())
				{
					rs=(*piece).first();
					TranslationList tmp(*rs);
					tl = tmp;
					ts = tl.first();
				}
			}
			
			//If we've reached the a possible new solution, check to see if it's unique...
			if(piece == starter->end() && !(AddSolution(next_soln)))
			{
				--piece;
				temp = next_soln.GetLastPieceState();
				next_soln.RemovePiece();
				rs = temp.rotation_state;
				TranslationList tempo(*rs);
				tl = tempo;
				ts = temp.translation_state;
				++ts;						//trying next translation
			}
		}
		
		++soln_iter;
	}
	return *this;
}

SolutionList::iterator& SolutionList::iterator::operator--()
{
	if(solution_pool.size() > 0)
		--soln_iter;
	else
	{
		Solution first_soln;
		struct SolutionState temp;

		std::list<RotationList>::iterator piece=starter->begin();
		RotationList::iterator rs=(*piece).first();
		TranslationList tl(*rs);
		TranslationList::iterator ts=tl.first();
		while(piece != starter->end())
		{
			while(rs != (*piece).last())
			{
				while(ts != tl.last())
				{
					 if(first_soln.PlacePiece(rs,ts))
					 {
					 	break;
					 }
					 ++ts;
				}
				if(ts != tl.last())
					break;
				++rs;
			}
			if(rs == (*piece).last() && piece == starter->begin())
			{
				std::cerr<<"SolutionList.cpp:iterator& SolutionList::first():Error: There are no "
					<<"solutions to this puzzle!"<<std::endl;
				exit(1);
			}
			else if(rs == (*piece).last())	//Piece wasn't placed in any orientation. Recursing to
			{								//previous piece
				--piece;
				temp = first_soln.GetLastPieceState();
				first_soln.RemovePiece();
				rs = temp.rotation_state;
				TranslationList tmp(*rs);
				tl = tmp;
				ts = temp.translation_state;
				++ts;						//trying next translation
			}
			else							//Piece was placed.  Advancing to next piece...
			{
				++piece;
				rs=(*piece).first();
				TranslationList tmp(*rs);
				tl = tmp;
				ts = tl.first();
			}
			
			//If we've reached the a possible new solution, check to see if it's unique...
			if(piece == starter->end() && !(AddSolution(first_soln)))
			{
				--piece;
				temp = first_soln.GetLastPieceState();
				first_soln.RemovePiece();
				rs = temp.rotation_state;
				TranslationList tempo(*rs);
				tl = tempo;
				ts = temp.translation_state;
				++ts;						//trying next translation
			}
		}
		//first_soln has been added to the solution_pool from AddSolution above, so now we must 
		//initialize the soln_iter and return *this...
		soln_iter = solution_pool.begin();

		while(soln_iter != solution_pool.end())
		{
			++*this;
			++soln_iter;
		}
		--soln_iter;
	}
	return *this;
}

Solution& SolutionList::iterator::operator*()
{
	if(soln_iter == solution_pool.end())
	{
		std::cerr<<"SolutionList.cpp:Solution& SolutionList::iterator::operator*():Error: Not "
			<<"allowed to dereference the last iterator."<<std::endl;
		exit(1);
	}
	return *soln_iter;
}

bool SolutionList::iterator::AddSolution(Solution& soln)
{
	bool success = false;
	
	//This is the master iterator that determines the success of the addition.  If this 
	//iterator doesn't make it to the end, then this solution is unique
	std::list<struct SolutionState>::iterator c=(soln.GetContainer()).begin();
	
	//This is a list of list-Piece iterators.  Each one will iterate through one of the
	//Solutions in the solution_pool
	std::list<std::list<struct SolutionState>::iterator> iterators;

	//Initialize all the iterators to the beginning of all the solutions in the solution_pool
	std::list<Solution>::iterator solns=solution_pool.begin();
	while(solns != solution_pool.end())
	{
		std::list<struct SolutionState>::iterator temp=((*solns).GetContainer()).begin();

		iterators.push_back(temp);			//Here's where I push on iterators
											//Dereferencing this iterator yeilds yeilds 
											//iterators into the containers of solutions
											//They are initialized here to the first 
											//SolutionState whose translation_state member,
											//when dereferenced, yeilds a piece of the 
											//actual solution
		++solns;
	}
	
	//This results in my master iterator...
	//t is an iterator to a list of iterators
	std::list<std::list<struct SolutionState>::iterator>::iterator prev_t,t=iterators.begin();
	
	//Working list.  This is where most of the action takes place as described later.
	std::list<Piece> partial_candidate;

	//Find out how many types of rotations we're dealing with...
	std::list<int> dss;
	std::list<int> mds;
	int dimmax = (*(((soln.GetContainer()).front()).translation_state)).GetMaxDims();
	for(int ds = 2;ds <= dimmax;ds++)
		for(int md = 0;md < ds-1;md++)
		{
			dss.push_back(ds);
			mds.push_back(md);
		}
	std::list<Piece> oldlist;				//temp list used to help create rotations
	std::list<Piece>::iterator j;			//iterator used by temp list
	std::list<int>::iterator i;				//iterators for rotation type
	std::list<int>::iterator l;
	std::list<int> rot_states;
	bool erase,first_loop=true;

	//Pieces will always be in the same order, I just need to see if they're in any of the
	//same places...
	
	//look for solution matches...
	while(!iterators.empty() && c != (soln.GetContainer()).end())
	{
		//initialize partial candidate list to contain member piece_depth from soln.container
		partial_candidate.erase(partial_candidate.begin(),partial_candidate.end());
		partial_candidate.push_back(*((*c).translation_state));
		
		//So now I have 2 lists, one with one element, (partial_candidate list), and one, 
		//(iterators), with iterators to the same corresponding elements from a list 
		//of lists, (solution_pool).  I want to see if any rotation of the partial_candidate
		//list matches any from the other list.  If an element from 
		//the solutions list doesn't match any of the rotations, remove the 
		//corresponding list from the iterators list.  If everything from iterators is 
		//eventually removed, then the candidate solution is unique and may be added to the
		//solution_pool.  If, however, one or more of the iterators is still present in the
		//iterators list after a cycle of the loop, reconstruct the partial list with the 
		//next piece from the candidate's container and increment all the iterators in the
		//iterators list.
		
		//Create rotations in the partial_candidate list:
		oldlist.erase(oldlist.begin(),oldlist.end());	//list that helps create rotations
		i=dss.begin();
		l=mds.begin();
		while(i != dss.end())
		{
			oldlist.swap(partial_candidate);
			partial_candidate.erase(partial_candidate.begin(),partial_candidate.end());
			j=oldlist.begin();
			while(j != oldlist.end())
			{
				for(int rots=0;rots<4;rots++)
				{
					(*j).Rotate(*l,*i);					//no normalization!
//					partial_candidate.remove(*j);		//can't do this if I want to preserve rot_state uniformly for all pieces
					partial_candidate.push_back(*j);
				}
				j++;
			}
			i++;
			l++;
		}
		
		if(!rot_states.empty())
		{
			std::list<int>::iterator rot_state_iter=rot_states.begin();
			int counter=1;
			std::list<Piece>::iterator rot_filter=partial_candidate.begin();
			std::list<Piece>::iterator temp_rot_filter;
			while(rot_filter != partial_candidate.end())
			{
				if(rot_state_iter != rot_states.end() && counter == *rot_state_iter)
				{
					++rot_state_iter;
					++rot_filter;
				}
				else
				{
					if(rot_state_iter == rot_states.end())
						break;
					else
					{
						temp_rot_filter = rot_filter;
						++rot_filter;
						partial_candidate.erase(temp_rot_filter);
					}
				}
				++counter;
			}
			partial_candidate.erase(rot_filter,partial_candidate.end());
			//Now partial_candidate only contains those rotations corresponding to the 
			//rotations of the previous pieces that matched the corresponding piece in one
			//or more of the solutions in the solution_pool
		}
		//end create rotations in partial_candidate list
		
		//Now I need to see if any of the pieces pointed to by the iterators of the solutions
		//*don't* match
		//what's in the partial candidate list.  I don't have to worry about translations.
		//If it's a different translation, then, it's a different solution.  I'm only 
		//worried about whole solutions that are simply rotations of other solutions....

		int size = partial_candidate.size();
		
		int pc_count;
		std::list<Piece>::iterator pc_iter;
		t=iterators.begin();
		while(t != iterators.end())
		{
			erase = true;
			pc_iter = partial_candidate.begin();
			pc_count = 1;
			while(pc_iter != partial_candidate.end())
			{
				if(*pc_iter == *((*(*t)).translation_state))
				{
					if(first_loop)
						rot_states.push_back(pc_count);
					erase = false;
				}
				++pc_iter;
				++pc_count;
			}
			if(erase)
			{
				prev_t = t;
				++t;
				iterators.erase(prev_t);
			}
			else
				++t;
		}
		if(first_loop)
		{
			rot_states.sort();
			rot_states.unique();
		}
		
		//increment all the iterators to the solutions in the eliminator.  And increment the 
		//iterator into the candidate solution.
		c++;
		if(!iterators.empty())				//be careful - iterators may be empty.
		{
			t=iterators.begin();
			while(t != iterators.end())
			{
				++(*t);
				++t;
			}
		}
		first_loop=false;
	}
	if(c != (soln.GetContainer()).end())
	{
		//Solution is being added to the solution pool because it is unique...
		success = true;
		solution_pool.push_back(soln);
	}
	return success;
}
		
SolutionList::iterator SolutionList::last()
{
	iterator i;
	i.starter = &rot_lists;
	i.soln_iter = i.solution_pool.end();
	return i;
}

bool SolutionList::iterator::operator!=(SolutionList::iterator& rhs)
{
	//right now this only works for telling if the iterator hasn't reached the end
	//if I have time, I'll make this work for solutions in the middle or beginning
	if(soln_iter == solution_pool.end() && rhs.soln_iter == rhs.solution_pool.end())
		return false;
	return true;
}
