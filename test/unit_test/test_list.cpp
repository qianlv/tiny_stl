/*
 * =====================================================================================
 *
 *       Filename:  test_list.cpp
 *
 *    Description:  List
 *
 *        Version:  1.0
 *        Created:  2016年05月23日 21时27分09秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  qianlv (), qianlv7 at gmail dot com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "tinyStl/list.h"
#include "test_header.h"

#include <iostream>
#include <gtest/gtest.h>
#include <list>
#include <initializer_list>
#include <algorithm>

using std::cout;
using std::endl;

typedef std::list<A> std_list_A;
typedef tinystl::list<A> tin_list_A;

bool cmp(const std_list_A& l, const tin_list_A& r)
{
    if (l.size() != r.size())
        return false;
    return std::equal(l.begin(), l.end(), r.begin());
}

TEST (A, ListConstruct)
{
    std_list_A list1{A(1), A(2), A(3)};
    tin_list_A list2{A(1), A(2), A(3)};
    tin_list_A list3(tin_list_A({A(1), A(2), A(3)}));
    tin_list_A list4; list4 = list2;
    tin_list_A list5; list5 = tin_list_A({A(1), A(2), A(3)});
    tin_list_A list6; list6 = {A(1), A(2), A(3)};
    ASSERT_TRUE(cmp(list1, list2));
    ASSERT_TRUE(cmp(list1, list3));
    ASSERT_TRUE(cmp(list1, list4));
    ASSERT_TRUE(cmp(list1, list5));
    ASSERT_TRUE(cmp(list1, list6));
}

TEST (A, ListIteratorsAndCapacity)
{
    tin_list_A list2{A(1), A(1), A(1)};
    for (tin_list_A::iterator iter = list2.begin();
            iter != list2.end(); ++iter)
    {
        EXPECT_EQ(*iter, A(1));
    }

    const tin_list_A& clist2 = list2;
    for (tin_list_A::const_iterator iter = clist2.begin();
            iter != clist2.end(); ++iter)
    {
        EXPECT_EQ(*iter, A(1));
    }

    EXPECT_EQ(list2.size(), 3);
    ASSERT_FALSE(list2.empty());
}

TEST (A, ListElementAcess)
{
    std_list_A list1;
    tin_list_A list2;

    for (int i = 0; i < 100; ++i)
    {
        list1.push_front(A(i));
        list2.push_front(A(i));
        // cout << list1.front().value << endl;
        EXPECT_EQ(list1.front(), list2.front());
    }
}


