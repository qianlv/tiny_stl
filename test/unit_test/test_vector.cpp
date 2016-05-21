/*
 * =====================================================================================
 *
 *       Filename:  test_vector.cpp
 *
 *    Description:  vector
 *
 *        Version:  1.0
 *        Created:  2016年05月21日 16时26分52秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  qianlv (), qianlv7 at gmail dot com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "tinyStl/vector.h"

#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include <initializer_list>

using std::cout;
using std::endl;

int ndtor = 0;
class A
{
public:
    A(int v = 0): value(v) {};
    ~A() 
    {
        ndtor++;
//        cout << "~A()" << endl;
    };

    bool operator==(const A& a) const
    {
        return value == a.value;
    }

    bool operator!=(const A& a) const
    {
        return !(*this == a);
    }

    int value;
    // int value1;
    // int value2;
    // int value3;
    // int value4;
    // int value5;
    // int value6;
    // int value7;
    // int value8;
    // int value9;
    // int value10;
    // int value11;
    // int value12;
    // int value13;
    // int value14;
    // int value15;
    // int value90;
    // int value91;
    // int value92;
    // int value93;
    // int value94;
    // int value95;
    // int value96;
    // int value97;
    // int value99;
    // int value98;
    // int value910;
    // int value911;
    // int value912;
    // int value913;
};

bool cmp(const std::vector<A>& l, const tinystl::vector<A>& r)
{
    if (l.size() != r.size())
        return false;
    for (size_t i = 0; i < l.size(); ++i)
    {
        if (l[i] != r[i])
            return false;
    }
    return true;
}

TEST (A, Vector)
{
    typedef std::vector<A> std_vector_A;
    typedef std::vector<int> std_vector_i;
    typedef tinystl::vector<A> tin_vector_A;
    typedef tinystl::vector<int> tin_vector_i;
    std_vector_A v1{A(1), A(2), A(3)};
    tin_vector_A v2{A(1), A(2), A(3)};
    ASSERT_TRUE(cmp(v1, v2));

    v1.insert(v1.begin(), A(4));
    v2.insert(v2.begin(), A(4));
    ASSERT_TRUE(cmp(v1, v2));

    v1.insert(v1.begin() + 2, {A(5), A(6)});
    v2.insert(v2.begin() + 2, {A(5), A(6)});
    ASSERT_TRUE(cmp(v1, v2));

    A a[4] = {A(7), A(8), A(9), A(10)};
    v1.insert(v1.end(), a, a+4);
    v2.insert(v2.end(), a, a+4);
    ASSERT_TRUE(cmp(v1, v2));

    v1.insert(v1.begin() + 3, 3, A(11));
    v2.insert(v2.begin() + 3, 3, A(11));
    ASSERT_TRUE(cmp(v1, v2));

    v1.pop_back();
    v2.pop_back();
    ASSERT_TRUE(cmp(v1, v2));
    for (size_t i = 0; i < v1.size(); ++i)
    {
        v1.pop_back();
        v2.pop_back();
    }
    ASSERT_TRUE(cmp(v1, v2));
    EXPECT_EQ(v1.capacity(), v2.capacity());

    for (size_t i = 0; i < 100; ++i)
    {
        v1.push_back(A(i));
        v2.push_back(A(i));
    }
    ASSERT_TRUE(cmp(v1, v2));
    EXPECT_EQ(v1.capacity(), v2.capacity());

    std_vector_A sw1{A(1)};
    tin_vector_A sw2{A(1)};
    v1.swap(sw1);
    v2.swap(sw2);
    ASSERT_TRUE(cmp(v1, v2));
    ASSERT_TRUE(cmp(sw1, sw2));

    v1.clear();
    v2.clear();
    ASSERT_TRUE(cmp(v1, v2));

    v1.assign(3, A(3));
    v2.assign(3, A(3));
    ASSERT_TRUE(cmp(v1, v2));

    v1.assign(2, A(4));
    v2.assign(2, A(4));
    ASSERT_TRUE(cmp(v1, v2));


    cout << v1.capacity() << "|" << v2.capacity() << endl;
    cout << v1.size() << "=" << v2.size() << endl;
    std::initializer_list<A> il{A(120), A(121)};
    v1.assign(il);
    v2.assign(il);
    ASSERT_TRUE(cmp(v1, v2));

    v1.resize(13);
    v2.resize(13);
    ASSERT_TRUE(cmp(v1, v2));

    v1.resize(2);
    v2.resize(2);
    ASSERT_TRUE(cmp(v1, v2));

    v1.shrink_to_fit();
    v2.shrink_to_fit();
    EXPECT_EQ(v1.capacity(), v2.capacity());
    EXPECT_EQ(v2.capacity(), v2.size());
    v1.resize(2000, A(10));
    v2.resize(2000, A(10));
    ASSERT_TRUE(cmp(v1, v2));
    // EXPECT_EQ(v1.capacity(), v2.capacity());

    v1.reserve(10);
    v2.reserve(10);
    ASSERT_TRUE(cmp(v1, v2));
    EXPECT_EQ(v1.capacity(), v2.capacity());
}
