/*
 * =====================================================================================
 *
 *       Filename:  construct.h
 *
 *    Description:  构造和析构
 *
 *        Version:  1.0
 *        Created:  2016年05月19日 10时01分02秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  qianlv (), qianlv7 at gmail dot com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef TINY_STL_CONSTRUCT_H_
#define TINY_STL_CONSTRUCT_H_

#include <new>

#include <iostream>
using std::cout;
using std::endl;

#include "type_traits.h"
#include "iterator.h"

namespace tinystl
{ 
    template <typename T1, typename T2>
    inline void construct(T1* ptr1, const T2& value)
    {
        new(ptr1) T1(value);
    }

    template <typename T>
    inline void destory(T* ptr)
    {
        ptr->~T();
    }

    template <typename Iterator>
    inline void __destory_aux(Iterator first, Iterator last, __true_type)
    {
        // cout << "__true_typ" << endl;
    }

    template <typename Iterator>
    inline void __destory_aux(Iterator first, Iterator last, __false_type)
    {
        // cout << "1__false_type" << endl;
        for (; first != last; ++first)
            destory(&*first);
    }

    template <typename Iterator, class T>
    inline void __destory(Iterator first, Iterator last, T*)
    {
        typedef typename __type_traits<T>::has_trivial_destructor is_trivial_destructor;
        __destory_aux(first, last, is_trivial_destructor());
    }

    template <typename Iterator>
    inline void destory(Iterator first, Iterator last)
    {
        __destory(first, last, value_type(first));
    }

    inline void destory(char*, char*) { }
    inline void destory(wchar_t*, wchar_t*) { }
} // end namespace tinystl

#endif // TINY_STL_CONSTRUCT_H_
