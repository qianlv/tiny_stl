/*
 * =====================================================================================
 *
 *       Filename:  iterator.h
 *
 *    Description:  iterator
 *
 *        Version:  1.0
 *        Created:  2016年05月18日 15时40分20秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  qianlv (), qianlv7 at gmail dot com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef TINY_STL_ITERATOR_H_
#define TINY_STL_ITERATOR_H_

#include <stddef.h>
// #include <iostream>

// using namespace std;

namespace tinystl
{
    /*
     * 迭代器类型
     */
    struct input_iterator_tag {};
    struct output_iterator_tag {};
    struct forward_iterator_tag : public input_iterator_tag {};
    struct bidirectional_iterator_tag : public forward_iterator_tag {};
    struct random_access_iterator_tag : public bidirectional_iterator_tag {};

    /*
     * 自行开发迭代器时继承这个类，避免遗漏
     * */
    template <typename Category,
              typename T,
              typename Distance = ptrdiff_t,
              typename Pointer = T*,
              typename Reference = T&>
    struct iterator
    {
        typedef Category    iterator_category;
        typedef T           value_type;
        typedef Distance    difference_type;
        typedef Pointer     pointer;
        typedef Reference   reference;
    };

    /*
     * 迭代器萃取(traits).
     * */
    template <typename Iterator>
    struct iterator_traits
    {
        typedef typename Iterator::iterator_category iterator_category;
        typedef typename Iterator::value_type        value_type;
        typedef typename Iterator::difference_type   difference_type;
        typedef typename Iterator::pointer           pointer;
        typedef typename Iterator::reference         reference;
    };

    template <typename T>
    struct iterator_traits<T*>
    {
        typedef random_access_iterator_tag iterator_category;
        typedef T                          value_type;
        typedef ptrdiff_t                  difference_type;
        typedef T*                         pointer;
        typedef T&                         reference;
    };

    template <typename T>
    struct iterator_traits<const T*>
    {
        typedef random_access_iterator_tag iterator_category;
        typedef T                          value_type;
        typedef ptrdiff_t                  difference_type;
        typedef const T*                   pointer;
        typedef const T&                   reference;
    };

    template <typename Iterator>
    inline typename iterator_traits<Iterator>::iterator_category
    iterator_category(const Iterator&)
    {
        typedef typename iterator_traits<Iterator>::iterator_category category;
        return category();
    }

    template <typename Iterator>
    inline typename iterator_traits<Iterator>::difference_type*
    distance_type(const Iterator&)
    {
        return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
    }

    template <typename Iterator>
    inline typename iterator_traits<Iterator>::value_type*
    value_type(const Iterator&)
    {
        return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
    }

    template <typename InputIterator>
    inline typename iterator_traits<InputIterator>::difference_type
    __distance(InputIterator first, InputIterator last,
               input_iterator_tag)
    {
        typename iterator_traits<InputIterator>::difference_type n = 0;
        while (first != last)
        {
            ++first; ++n;
        }
        return n;
    }

    template <typename RandomAccessIterator>
    inline typename iterator_traits<RandomAccessIterator>::difference_type
    __distance(RandomAccessIterator first, RandomAccessIterator last,
               random_access_iterator_tag)
    {
        return last - first;
    }

    template <typename Iterator>
    inline typename iterator_traits<Iterator>::difference_type
    distance(Iterator first, Iterator last)
    {
        return __distance(first, last, iterator_category(first));
    }

    template <typename InputIterator, typename Distance>
    inline void __advance(InputIterator& iter, Distance n,
                   input_iterator_tag)
    {
        while (n--)
            ++iter;
    }

    template <typename BidirectionalIterator, typename Distance>
    inline void __advance(BidirectionalIterator& iter, Distance n,
                   bidirectional_iterator_tag)
    {
        if (n >= 0)
            while (n--) ++iter;
        else
            while (n++) --iter;
    }

    template <typename RandomAccessIterator, typename Distance>
    inline void __advance(RandomAccessIterator& iter, Distance n,
                   random_access_iterator_tag)
    {
        iter += n;
    }

    template <typename Iterator, typename Distance>
    inline void advance(Iterator& iter, Distance n)
    {
        __advance(iter, n, iterator_category(iter));
    }

} // end namespace tinystl

#endif // TINY_STL_ITERATOR_H_
