/*
 * =====================================================================================
 *
 *       Filename:  test_alloc.cpp
 *
 *    Description:  内存配置器
 *
 *        Version:  1.0
 *        Created:  2016年05月06日 09时56分03秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  qianlv (), qianlv7 at gmail dot com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "tinyStl/alloc.h"

#include <iostream>
#include <gtest/gtest.h>
#include <stdlib.h>

using namespace tinystl;
using namespace std;

void out_of_memory()
{
    cerr << "out of memory" << endl;
    MallocAlloc::set_malloc_handler(nullptr);
}

TEST (MallocAlloc, Basic)
{
    char* p = (char*)MallocAlloc::allocate(sizeof(int) * 10);
    p = (char*)MallocAlloc::reallocate((void*)p, sizeof(p), 10);
    p = (char*)MallocAlloc::reallocate((void*)p, sizeof(p), 20);
    MallocAlloc::deallocate((void*)p, sizeof(p));
    MallocAlloc::set_malloc_handler(out_of_memory);
    try
    {
        p = (char*)MallocAlloc::allocate(200000000000);
    }
    catch (exception& e)
    {
        cout << "Standartd exception: " << e.what() << endl;
    }
}

