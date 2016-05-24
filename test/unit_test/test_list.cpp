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
#include "tinyStl/vector.h"
#include "tinyStl/iterator.h"

#include "test_header.h"

#include <iostream>
#include <gtest/gtest.h>
#include <list>
#include <initializer_list>
#include <algorithm>
#include <functional>

using std::cout;
using std::endl;

typedef std::list<A> std_list_A;
typedef tinystl::list<A> tin_list_A;
typedef tinystl::vector<A> tin_vector_A;

bool cmp(const std_list_A& l, const tin_list_A& r)
{
    if (l.size() != r.size())
        return false;
    return std::equal(l.begin(), l.end(), r.begin());
}

template <typename InputIterator>
void output(InputIterator first, InputIterator last)
{
    std::for_each(first, last, [](const A& a) { cout << a.value << " "; });
    cout << endl;
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
        list1.push_back(A(i+100));
        list2.push_back(A(i+100));
        EXPECT_EQ(list1.back(), list2.back());
    }
}

TEST (A, ListModifiers)
{
    std_list_A list1{A(1), A(2), A(3)};
    tin_list_A list2{A(1), A(2), A(3)};
    tin_vector_A vec1={A(4), A(5), A(6)};
    
    list1.assign(10, A(1));
    list2.assign(10, A(1));
    ASSERT_TRUE(cmp(list1, list2));

    list1.assign(vec1.begin(), vec1.end());
    list2.assign(vec1.begin(), vec1.end());
    ASSERT_TRUE(cmp(list1, list2));

    list1.insert(list1.begin(), 100, A(100));
    list2.insert(list2.begin(), 100, A(100));
    ASSERT_TRUE(cmp(list1, list2));

    list1.insert(list1.begin(), vec1.begin(), vec1.end());
    list2.insert(list2.begin(), vec1.begin(), vec1.end());
    ASSERT_TRUE(cmp(list1, list2));

    auto it1 = list1.begin();
    auto la1 = list1.begin();
    tinystl::advance(it1, 17);
    tinystl::advance(la1, 73);
    auto iter1 = list1.erase(it1, la1);
    auto it2 = list2.begin();
    auto la2 = list2.begin();
    tinystl::advance(it2, 17);
    tinystl::advance(la2, 73);
    auto iter2 = list2.erase(it2, la2);
    EXPECT_EQ(*iter1, *iter2);
    ASSERT_TRUE(cmp(list1, list2));

    std_list_A tmp1 = {A(0)};
    tin_list_A tmp2 = {A(0)};
    list1.swap(tmp1);
    list2.swap(tmp2);
    ASSERT_TRUE(cmp(list1, list2));
    list1.swap(tmp1);
    list2.swap(tmp2);
    ASSERT_TRUE(cmp(list1, list2));

    list1.clear();
    list2.clear();
    EXPECT_EQ(list1.size(), list2.size());
}

TEST (A, ListOperations)
{
    std_list_A list1{A(10), A(2), A(3), A(1023), A(-10)};
    tin_list_A list2{A(10), A(2), A(3), A(1023), A(-10)};
    std_list_A tmp1{A(10), A(2), A(3), A(1023), A(-1023)};
    tin_list_A tmp2{A(10), A(2), A(3), A(1023), A(-1023)};
    tin_vector_A vec1={A(4), A(5), A(6)};

    list1.splice(list1.begin(), tmp1);
    list2.splice(list2.begin(), tmp2);
    ASSERT_TRUE(cmp(list1, list2));
    ASSERT_TRUE(cmp(tmp1, tmp2));

    list1.sort();
    list2.sort();
    output(list1.begin(), list1.end());
    output(list2.begin(), list2.end());
    ASSERT_TRUE(cmp(list1, list2));

    list1.sort(std::greater<A>());
    list2.sort(std::greater<A>());
    output(list1.begin(), list1.end());
    output(list2.begin(), list2.end());
    ASSERT_TRUE(cmp(list1, list2));

    list1.remove(A(1023));
    list2.remove(A(1023));
    ASSERT_TRUE(cmp(list1, list2));

    list1.unique();
    list2.unique();
    ASSERT_TRUE(cmp(list1, list2));

    list1.reverse();
    list2.reverse();
    ASSERT_TRUE(cmp(list1, list2));

    auto f = [](const A& a) { return a > A(0); };
    list1.remove_if(f);
    list2.remove_if(f);
    ASSERT_TRUE(cmp(list1, list2));
    output(list1.begin(), list1.end());
}
