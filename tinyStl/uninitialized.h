/*
 * =====================================================================================
 *
 *       Filename:  uninitialized.h
 *
 *    Description:  uninitialized
 *
 *        Version:  1.0
 *        Created:  2016年05月19日 11时04分06秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  qianlv (), qianlv7 at gmail dot com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef TINY_STL_UNINITIALIZED_H_
#define TINY_STL_UNINITIALIZED_H_

#include "type_traits.h"
#include "iterator.h"

#include <algorithm>
#include <string.h>
#include <iostream>
using std::cout;
using std::endl;

namespace tinystl
{
    /* 
     * uninitialized_fill
     * */
    template <typename Iterator, typename T>
    inline void __uninitialized_fill_aux(Iterator first, Iterator last, const T& v, __true_type)
    {
        cout << "fill" << endl;
        std::fill(first, last, v);
    }

    template <typename Iterator, typename T>
    inline void __uninitialized_fill_aux(Iterator first, Iterator last, const T& v, __false_type)
    {
        Iterator cur = first;
        try 
        {
            for (; cur != last; ++cur)
                construct(&*cur, v);
        } catch (...)
        {
            destory(first, cur);
        }
    }

    template <typename Iterator, typename T, typename T1>
    inline void __uninitialized_fill(Iterator first, Iterator last, const T& v, T1*)
    {
        typedef typename __type_traits<T1>::is_POD_type is_POD_type;
        __uninitialized_fill_aux(first, last, v, is_POD_type());
    }

    template <typename Iterator, typename T>
    inline void uninitialized_fill(Iterator first, Iterator last, const T& v)
    {
        __uninitialized_fill(first, last, v, value_type(first));
    }

    /* 
     * uninitialized_fill
     * */
    template <typename Iterator, typename Size, typename T>
    inline Iterator __uninitialized_fill_n_aux(Iterator first, Size n, const T& v, __false_type)
    {
        Iterator cur = first;
        try
        {
            for (; n > 0; --n, ++cur)
            {
                construct(&*cur, v);
            }
            return cur;
        } catch (...)
        {
            destory(first, cur);
        }
    }

    template <typename Iterator, typename Size, typename T>
    inline Iterator __uninitialized_fill_n_aux(Iterator first, Size n, const T& v, __true_type)
    {
        cout << "fill_n" << endl;
        return std::fill_n(first, n, v);
    }

    template <typename Iterator, typename Size, typename T, typename T1>
    inline Iterator __uninitialized_fill_n(Iterator first, Size n, const T& v, T1*)
    {
        typedef typename __type_traits<T1>::is_POD_type is_POD_type;
        return __uninitialized_fill_n_aux(first, n, v, is_POD_type());
    }

    template <typename Iterator, typename Size, typename T>
    inline Iterator uninitialized_fill_n(Iterator first, Size n, const T& v)
    {
        return __uninitialized_fill_n(first, n, v, value_type(first));
    }

    /* 
     * uninitialized_copy
     * */
    template <typename InputIterator, typename ForwardIterator>
    inline ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __false_type)
    {
        ForwardIterator cur = result;
        try
        {
            for(; first != last; ++first, ++cur)
            {
                construct(&*cur, *first);
            }
            return cur;
        } catch(...)
        {
            destory(result, cur);
        }
    }

    template <typename InputIterator, typename ForwardIterator>
    inline ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __true_type)
    {
        cout << "copy" << endl;
        return std::copy(first, last, result);
    }

    template <typename InputIterator, typename ForwardIterator, typename T>
    inline ForwardIterator __uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result, T*)
    {
        typedef typename __type_traits<T>::is_POD_type is_POD_type;
        return __uninitialized_copy_aux(first, last, result, is_POD_type());
    }

    template <typename InputIterator, typename ForwardIterator>
    inline ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result)
    {
        return __uninitialized_copy(first, last, result, value_type(result));
    }

    inline char* uninitialized_copy(char* first, char* last, char* result)
    {
        memmove(result, first, last - first);
        return result + (last - first);
    }

    inline wchar_t* uninitialized_copy(wchar_t* first, wchar_t* last, wchar_t* result)
    {
        memmove(result, first, sizeof(wchar_t) * (last - first));
        return result + (last - first);
    }

} // end namespace tinystl

#endif // TINY_STL_UNINITIALIZED_H_
