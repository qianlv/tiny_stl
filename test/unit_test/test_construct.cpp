/*
 * =====================================================================================
 *
 *       Filename:  test_construct.cpp
 *
 *    Description:  test construct and initialiezed
 *
 *        Version:  1.0
 *        Created:  2016年05月19日 11时02分32秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  qianlv (), qianlv7 at gmail dot com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "tinyStl/construct.h"
#include "tinyStl/alloc.h"
#include "tinyStl/uninitialized.h"

#include <iostream>
#include <vector>
#include <gtest/gtest.h>

using std::cout;
using std::endl;

struct A
{
    A(int v = 0) : value(v) { }
    int value;
    bool operator==(const struct A& a) const
    {
        return value == a.value;
    }
};

TEST (A, Construct)
{
    int* p = (int*)tinystl::alloc::allocate(sizeof(int) * 10);
    tinystl::uninitialized_fill(p, p + 10, 3);
    for (int i = 0; i < 10; i++)
    {
        EXPECT_EQ(p[i], 3) << "value = " << p[i];
    }

    tinystl::uninitialized_fill_n(p, 4, 10);
    for (int i = 0; i < 4; i++)
    {
        EXPECT_EQ(p[i], 10) << "value = " << p[i];
    }
    
    std::vector<int> iv = {1, 2, 3, 4};
    tinystl::uninitialized_copy(iv.begin(), iv.end(), p);
    int i = 0;
    for (auto iter = iv.begin(); iter != iv.end(); ++iter)
    {
        EXPECT_EQ(p[i], *iter) << "value = " << p[i];
        ++i;
    }

    A* alist = (A*)tinystl::alloc::allocate(sizeof(A) * 10);
    tinystl::uninitialized_fill(alist, alist + 10, A(3));
    for (int i = 0; i < 10; i++)
    {
        EXPECT_EQ(alist[i], A(3)) << "value = " << alist[i].value;
    }

    tinystl::uninitialized_fill_n(alist, 4, A(10));
    tinystl::uninitialized_fill_n(p, 4, 10);
    for (int i = 0; i < 4; i++)
    {
        EXPECT_EQ(alist[i], A(10)) << "value = " << alist[i].value;
    }

    std::vector<A> av = {A(1), A(2), A(3), A(4)};
    tinystl::uninitialized_copy(av.begin(), av.end(), alist);
    i = 0;
    for (auto iter = av.begin(); iter != av.end(); ++iter)
    {
        EXPECT_EQ(alist[i], *iter) << "value = " << alist[i].value;
        ++i;
    }
}
