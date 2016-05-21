/*
 * =====================================================================================
 *
 *       Filename:  test_iterator.cpp
 *
 *    Description:  test iterator
 *
 *        Version:  1.0
 *        Created:  2016年05月18日 21时04分55秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  qianlv (), qianlv7 at gmail dot com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "tinyStl/iterator.h"

#include <iostream>
#include <gtest/gtest.h>

using std::cout;
using std::endl;

struct node
{
    node(int v) : value(v), next(nullptr) { }
    int output()
    {
        return value;
    }
    int value;
    struct node* next;
};

template <typename T>
class MyIter : public tinystl::iterator<tinystl::forward_iterator_tag, T>
{

public:
    MyIter(T* p=nullptr) : ptr(p) { }
    ~MyIter() { }

    T& operator*() const { return *ptr;}

    T* operator->() { return ptr; }

    MyIter& operator++() 
    {
        ptr = ptr->next;
        return *this;
    }

    MyIter& operator++(int)
    {
        MyIter tmp = *this;
        ++*this;
        return *this;
    }

    bool operator==(const MyIter& i) const
    {
        return ptr == i.ptr;
    }

    bool operator!=(const MyIter& i) const
    {
        return ptr != i.ptr;
    }

    T* ptr;
};

TEST (MyIter, Iterator)
{
    struct node* head = new node(3);
    struct node* tmp = head;
    tmp->next = new node(5);
    tmp = tmp->next;
    tmp->next = new node(6);
    tmp = tmp->next;
    tmp->next = new node(7);
    tmp = tmp->next;

    int value[] = {3, 5, 6, 7};
    MyIter<struct node> iter(head);
    for (int i = 0; iter != MyIter<struct node>(); ++iter, ++i)
    {
        EXPECT_EQ(iter->output(), value[i]);
    }

    MyIter<struct node> iter1(head);
    EXPECT_EQ(tinystl::distance(iter1, MyIter<struct node>()), 4) << "test distance for MyIter";

    tinystl::advance(iter1, 2);
    EXPECT_EQ(iter1->output(), 6) << "test advance for MyIter";

    struct node node_array[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    // int node_array[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    EXPECT_EQ(tinystl::distance(node_array, node_array + 10), 10) << "test distance for array";
   
    tmp = node_array;
    tinystl::advance(tmp, 2);
    EXPECT_EQ(tmp->output(), 3) << "test advance for array";
}

