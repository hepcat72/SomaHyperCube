// Project 2: Soma Hypercube													Rob Leach
// RotationList.cpp																 5/5/2000
//---------------------------------------------------------------------------------------

#include "RotationList.h"

RotationList::RotationList(Piece& rhs)
{
	if(rhs.GetMaxDims() == 1)
	{
		rotations.push_back(rhs);
	}
	else
	{
		std::list<int> dss;
		std::list<int> mds;
		for(int ds = 2;ds <= rhs.GetMaxDims();ds++)
			for(int md = 0;md < ds-1;md++)
			{
				dss.push_back(ds);
				mds.push_back(md);
			}
		
		std::list<Piece> oldlist;
		rotations.push_back(rhs);
		
		std::list<int>::iterator i=dss.begin();
		std::list<int>::iterator l=mds.begin();
		while(i != dss.end())
		{
			oldlist.swap(rotations);
			rotations.erase(rotations.begin(),rotations.end());
			std::list<Piece>::iterator j=oldlist.begin();
			while(j != oldlist.end())
			{
				for(int rots=0;rots<4;rots++)
				{
					(*j).Rotate(*l,*i);
					(*j).Normalize();
					rotations.remove(*j);
					rotations.push_back(*j);
				}
				j++;
			}
			i++;
			l++;
		}
	}
}
