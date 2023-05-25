//
// Sample1.cpp
//

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include "BTree.hpp"
#include <random>
#include <cassert>

int IntTest1 ()
{
	BTree<int> t (3);
	
	for (int i = 0; i < 200; i++)
	{
		t.Insert (i);
	}

	for (int i = 0; i < 200; i++)
	{
		int* pVal = t.Search (i);
		assert (*pVal == i);
	}

	for (int i = 0; i < 200; i += 2)
	{
		t.Remove (i);
	}

	t.Traverse ();

	return 0;
}

int IntTest2 ()
{
	BTree<int> t (3);

	for (int i = 200; i > 0; i--)
	{
		t.Insert (i);
	}

	for (int i = 1; i <= 200; i++)
	{
		int* pVal = t.Search (i);
		assert (*pVal == i);
	}

	for (int i = 200; i > 0; i-=2)
	{
		t.Remove (i);
	}

	t.Traverse ();

	return 0;
}

int test1 ()
{
	BTree<int> t (3);

	t.Insert (10);
	t.Insert (20);
	t.Insert (5);
	t.Insert (6);
	t.Insert (12);
	t.Insert (30);
	t.Insert (7);
	t.Insert (17);

	std::cout << "Traversal of the constructed tree is ";
	t.Traverse ();

	int k = 6;
	(t.Search (k) != nullptr) ? std::cout << "\nPresent" : std::cout << "\nNot Present";

	k = 15;
	(t.Search (k) != nullptr) ? std::cout << "\nPresent" : std::cout << "\nNot Present";

	return 0;
}

int test2 ()
{
	BTree<int> t (3);

	t.Insert (1);
	t.Insert (3);
	t.Insert (7);
	t.Insert (10);
	t.Insert (11);
	t.Insert (13);
	t.Insert (14);
	t.Insert (15);
	t.Insert (18);
	t.Insert (16);
	t.Insert (19);
	t.Insert (24);
	t.Insert (25);
	t.Insert (26);
	t.Insert (21);
	t.Insert (4);
	t.Insert (5);
	t.Insert (20);
	t.Insert (22);
	t.Insert (2);
	t.Insert (17);
	t.Insert (12);
	t.Insert (6);

	std::cout << "Traversal of tree constructed is\n";
	t.Traverse ();
	std::cout << std::endl;

	t.Remove (6);
	std::cout << "Traversal of tree after removing 6\n";
	t.Traverse ();
	std::cout << std::endl;

	t.Remove (13);
	std::cout << "Traversal of tree after removing 13\n";
	t.Traverse ();
	std::cout << std::endl;

	t.Remove (7);
	std::cout << "Traversal of tree after removing 7\n";
	t.Traverse ();
	std::cout << std::endl;

	t.Remove (4);
	std::cout << "Traversal of tree after removing 4\n";
	t.Traverse ();
	std::cout << std::endl;

	t.Remove (2);
	std::cout << "Traversal of tree after removing 2\n";
	t.Traverse ();
	std::cout << std::endl;

	t.Remove (16);
	std::cout << "Traversal of tree after removing 16\n";
	t.Traverse ();
	std::cout << std::endl;

	return 0;
}

int test3 (int degree, int min, int max, int count)
{
	BTree<int> t (degree);

	std::random_device seed_gen;
	std::default_random_engine engine (seed_gen ());
	std::uniform_int_distribution<int> dist (min, max);

	for (int i = 0; i < count; ++i)
	{
		int result = dist (engine);
		auto response = t.Search (result);
		if (response == nullptr)
		{
			t.Insert (result);
		}
		else
		{
			std::cout << "Still exist " << *response << std::endl;
		}
	}

	t.Traverse ();

	for (int i = 0; i < count; ++i)
	{
		int result = dist (engine);
		t.Remove (result);
	}

	t.Traverse ();

	return 0;
}

int test4 ()
{
	const int stringSize = 12;
	BTree<std::string> t (3);
	char buf[stringSize + 1]{ 0 };
	std::random_device seed_gen;
	std::default_random_engine engine (seed_gen ());
	std::uniform_int_distribution<int> dist ('0', 'z');

	for (int i = 0; i < 40; ++i)
	{
		for (int j = 0; j < stringSize; j++)
		{
			char charCode = static_cast<char>(dist (engine));
			if ('0' <= charCode && charCode <= '9' ||
				'a' <= charCode && charCode <= 'z' ||
				'A' <= charCode && charCode <= 'Z')
			{
				buf[j] = charCode;
			}
			else
			{
				j--;
			}
		}
		buf[stringSize] = 0x00;
		std::string buffer (buf);
		std::string* response = t.Search (buffer);
		if (response == nullptr)
		{
			t.Insert (buffer);
		}
		else
		{
			std::cout << "Still exist " << *response << std::endl;
		}
	}

	t.Insert ("ABCDEF");
	auto response = t.Search ("ABCDEF");

	t.Traverse ();

	for (int i = 0; i < 40; ++i)
	{
		for (int j = 0; j < stringSize; j++)
		{
			char charCode = static_cast<char>(dist (engine));
			if ('0' <= charCode && charCode <= '9' ||
				'a' <= charCode && charCode <= 'z' ||
				'A' <= charCode && charCode <= 'Z')
			{
				buf[j] = charCode;
			}
			else
			{
				j--;
			}
		}
		buf[stringSize] = 0x00;
		std::string buffer (buf);
		t.Remove (buffer);
	}

	t.Traverse ();


	return 0;
}

int test5 ()
{
	BTree<char> t (3); // A B-Tree with minimum degree 3

	for (char c = '0'; c < 'z'; c++)
	{
		t.Insert (c);
	}

	t.Traverse ();

	for (char c = '0'; c < 'z'; c++)
	{
		assert (c == *(t.Search (c)));
	}

	for (char c = '0'; c < 'z'; c++)
	{
		assert (t.Remove (c));
	}

	t.Traverse ();

	return 0;
}

int main ()
{
	//std::cout.setf(std::ios::hex, std::ios::basefield);

	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	IntTest1 ();
	IntTest2 ();
	test1 ();
	test2 ();
	test3 (5, -500, 500, 1000);
	test4 ();

	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtDumpMemoryLeaks();
	return 0;
}