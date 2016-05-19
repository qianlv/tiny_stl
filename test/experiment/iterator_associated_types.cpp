/*
 * =====================================================================================
 *
 *       Filename:  iterator_associated_types.cpp
 *
 *    Description:  迭代器对应的类型
 *
 *        Version:  1.0
 *        Created:  2016年05月18日 10时12分41秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  qianlv (), qianlv7 at gmail dot com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <iostream>
using namespace std;

template <class I, class T>
void func_impl(I iter, T t)
{
    T tmp = 10;
    cout << tmp << endl;
}

template <class I>
inline void func(I iter)
{
    func_impl(iter, *iter);
}


template <class T>
struct MyIter
{
    typedef T value_type;
    T* ptr;
    MyIter(T* p=0) : ptr(p) { }
    T& operator*() const { return *ptr; }
};

template <class I>
typename I::value_type
func2(I ite)
{
    return *ite;
}

template <typename I>
struct iterator_traits_sgi
{
    typedef typename I::value_type value_type;
};

template <typename T>
struct iterator_traits_sgi<T*>
{
    typedef T value_type;
};

template <typename T>
struct iterator_traits_sgi<const T*>
{
    typedef T value_type;
};

template <typename I>
typename iterator_traits_sgi<I>::value_type
func3(I iter)
{
    return *iter;
}

int main()
{
    int i = 2;
    func(&i);
    cout << func2(MyIter<int>(new int(10))) << endl;
    cout << func3(MyIter<int>(new int(10))) << endl;
    cout << func3(new int(10)) << endl;
    const int* j = &i;
    cout << func3(j) << endl;
}
