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

using tinystl::alloc;
using std::cout;
using std::endl;
using std::cerr;

void out_of_memory()
{
    cerr << "out of memory" << endl;
    // alloc::set_malloc_handler(nullptr);
}

TEST (MallocAlloc, Basic)
{
    char* p = (char*)alloc::allocate(sizeof(int) * 10);
    p = (char*)alloc::reallocate((void*)p, sizeof(p), 10);
    p = (char*)alloc::reallocate((void*)p, sizeof(p), 20);
    alloc::deallocate((void*)p, sizeof(p));
    // alloc::set_malloc_handler(out_of_memory);
    try
    {
        p = (char*)alloc::allocate(200000000000);
    }
    catch (std::exception& e)
    {
        cout << "Standartd exception: " << e.what() << endl;
    }
}

