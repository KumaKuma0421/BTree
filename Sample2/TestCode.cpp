// ---------------------------------------------------------------------------------------
// TestCode.cpp
//
// @sa https://github.com/solangii/b-plus-tree
// ---------------------------------------------------------------------------------------

#include <iostream>
#include "BPlusTree.h"

void print_bool (bool cond)
{
	if (cond)
	{
		std::cout << "True\n";
	}
	else
	{
		std::cout << "False\n";
	}
}

void Test1 ()
{
	BPlusTree<int> bpt (15);

	for (int i = 1; i < 136; i++)
	{
		bpt.insert (i * 13 + 87);
		std::cout << i * 13 + 87 << ' ';
		bpt.insert (i * 99 - 28);
		std::cout << i * 99 - 28 << ' ';

	}

	std::cout << std::endl;
	bpt.Traverse ();

	bpt.remove (152);
	bpt.remove (269);
	bpt.remove (256);


	std::cout << "ㄴㄴㄴ" << std::endl;
	bpt.remove (467);
	bpt.Traverse ();
	std::cout << std::endl << std::endl;
	bpt.remove (764);
	bpt.remove (863);

	std::cout << std::endl;
	bpt.Traverse ();

	bpt.remove (71);
	bpt.remove (100);
	bpt.remove (139);

	std::cout << std::endl;
	bpt.Traverse ();

	bpt.remove (368);

	std::cout << std::endl;
	bpt.Traverse ();
}
