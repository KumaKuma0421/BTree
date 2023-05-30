// ---------------------------------------------------------------------------------------
// main.cpp
//
// @sa https://github.com/solangii/b-plus-tree
// ---------------------------------------------------------------------------------------

#include <iostream>
#include "BPlusTree.h"

void Test1 ();

int main ()
{
    //std::cout.setf(std::ios::hex, std::ios::basefield);

    _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

    //Test1 ();

    {
        BPlusTree<int> tree (3);
        for (int i = 1; i <= 12; i++)
        {
            tree.insert (i);
            tree.Traverse ();
            std::cout << "-------- -------- -------- --------" << std::endl;
        }
    
        for (int i = 13; i > 0; i--)
        {
            tree.remove (i);
            tree.Traverse ();
            std::cout << "-------- -------- -------- --------" << std::endl;
        }

    }

    //for (int i = 0; i < 5; i++)
    //{
    //    tree.remove (i);
    //}

    //tree.Traverse ();

    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    _CrtDumpMemoryLeaks();
    return 0;
}