/*
 * =====================================================================================
 *
 *       Filename:  list.h
 *
 *    Description:  List
 *
 *        Version:  1.0
 *        Created:  2016年05月22日 11时30分39秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  qianlv (), qianlv7 at gmail dot com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "tinyStl/iterator.h"
#include "tinyStl/alloc.h"
#include "tinyStl/construct.h"

#include <iostream>
#include <initializer_list>
#include <algorithm>
#include <functional>
#include <stddef.h>

#ifndef TINY_STL_LIST_H_
#define TINY_STL_LIST_H_
namespace tinystl
{
    template <typename T> 
    struct __list_node
        {
        typedef void* void_pointer;
        void_pointer prev;
        void_pointer next;
        T data;
    };

    template <typename T, typename Ref, typename Ptr>
    struct __list_iterator
    {
        typedef __list_iterator<T, T&, T*>      iterator;
        typedef __list_iterator<T, Ref, Ptr>    self;
        
        typedef bidirectional_iterator_tag      iterator_category;
        typedef T                               value_type;
        typedef Ptr                             pointer;
        typedef Ref                             reference;
        typedef size_t                          size_tyep;
        typedef ptrdiff_t                       difference_type;
        typedef __list_node<T>*                 link_type;

        link_type node;

        __list_iterator(link_type x) : node(x) { }
        __list_iterator() { }
        __list_iterator(const iterator& x) : node(x.node) { }

        bool operator==(const self& x) const { return node == x.node; }
        bool operator!=(const self& x) const { return node != x.node; }

        reference operator*() const { return (*node).data; }
        pointer operator->() const { return &(operator*()); }
        
        self& operator++() 
        {
            node = (link_type)((*node).next);
            return *this;
        }

        self operator++(int)
        {
            self tmp = *this;
            ++*this;
            return tmp;
        }

        self& operator--()
        {
            node = (link_type)((*node).prev);
            return *this;
        }

        self operator--(int)
        {
            self tmp = *this;
            --*this;
            return tmp;
        }
    };

    template <typename T, typename Alloc = malloc_alloc>
    class list
    {
        typedef void*                   void_pointer;
        typedef __list_node<T>          list_node;
        typedef simple_alloc<list_node, Alloc>  list_node_allocator;
    public:
        typedef __list_iterator<T, T&, T*>              iterator;
        typedef __list_iterator<T, const T&, const T*>  const_iterator;
        typedef T                                       value_type;
        typedef value_type*                             pointer; 
        typedef const value_type&                       const_pointer;
        typedef value_type&                             reference;
        typedef const value_type&                       const_reference;
        typedef size_t                                  size_type;
        typedef ptrdiff_t                               difference_type;
        typedef list_node*                              link_type;

    public:
        /*
         * ctor, dtor, assginment
         */
        list ()
        {
            empty_initialize();
        }

        explicit list(size_type n, const value_type& value = value_type())
        {
            fill_initialize(n, value);
        }

        list (const list& li) { range_initialize(li.begin(), li.end()); }

        list (list&& li) : node(li.node)
        {
            li.node = nullptr;
        }

        list (std::initializer_list<value_type> il)
        {
            range_initialize(il.begin(), il.end());
        }

        list& operator= (const list& li)
        {
            cout << "list = (const list& li)" << endl;
            if (this != &li)
            {
                range_init(li.begin(), li.end());
            }
            return *this;
        }

        list& operator= (std::initializer_list<value_type> il)
        {
            cout << "list& = (std::initializer_list)" << endl;
            range_init(il.begin(), il.end());
            return *this;
        }

        list& operator= (list&& li)
        {
            cout << "list& = (list&& li)" << endl;
            range_init(li.begin(), li.end());
            return *this;
        }

        ~list ()
        {
            clear();
            put_node(node);
        }

    public:
        /*
         * Iterators, TODO reverse_iterator
         */
        iterator begin() { return iterator((link_type)(node->next));}
        const_iterator begin() const { return const_iterator((link_type)(node->next));}
        iterator end() { return iterator(node); }
        const_iterator end() const { return const_iterator(node); }
        const_iterator cbegin() const { return const_iterator((link_type)(node->next)); }
        const_iterator cend() const { return const_iterator(node); }

        /*
         * Capacity
         */
        bool empty() const { return (*node).next == node; }
        size_type size() const noexcept
        {
            return tinystl::distance(begin(), end());
        }

        /*
         * Element access
         */
        reference front()
        {
            return *begin();
        }
        const_reference front() const
        {
            return *begin();
        }

        reference back()
        {
            return *(--end());
        }
        const_reference back() const
        {
            return *(--end());
        }

        /*
         * Modifiers data
         */
        void push_front(const value_type& value)
        {
            insert(begin(), value); 
        }
        void push_back(const value_type& value)
        {
            insert(end(), value);
        }

        void pop_front()
        {
            erase(begin());
        }
        void pop_back()
        {
            iterator tmp = end();
            erase(--tmp);
        }

        template <typename InputIterator>
        void assign(InputIterator first, InputIterator last);
        void assign(size_type n, const value_type& value);
        void assign(std::initializer_list<value_type> il);

        iterator insert(iterator postion, const value_type& value);
        iterator insert(iterator postion, size_type n, const value_type& value);
        template <typename InputIterator>
        iterator insert(iterator postion, InputIterator first, InputIterator last);

        iterator erase(iterator postion);
        iterator erase(iterator first, iterator last);
        void swap(list& li)
        {
            using std::swap;
            swap(node, li.node);
        }
        void resize(size_type n);
        void resize(size_type n, const value_type& value);
        void clear();

        /*
         * Operations
         */
        void remove(const value_type& value);
        template <typename Predicate>
        void remove_if(Predicate pred);
        void unique();
        template <typename BinaryPredicate>
        void unique(BinaryPredicate binary_pred);

        void splice(iterator postion, list& x);
        void splice(iterator postion, list&& x);
        void splice(iterator postion, list& x, iterator i);
        void splice(iterator postion, list&& x, iterator i);
        void splice(iterator postion, list& x, iterator first, iterator last);
        void splice(iterator postion, list&& x, iterator first, iterator last);

        void merge(list& x);
        void merge(list&& x);
        template <typename Compare>
        void merge(list& x, Compare cmp);
        template <typename Compare>
        void merge(list&& x, Compare cmp);

        void reverse() noexcept;
        void sort();
        template <typename Compare>
        void sort(Compare cmp);

    private:
        link_type get_node()
        {
            return list_node_allocator::allocate();
        }
        link_type put_node(link_type p)
        {
            list_node_allocator::deallocate(p);
        }

        link_type create_node(const value_type& value)
        {
            link_type p = get_node();
            try
            {
                tinystl::construct(&p->data, value);
            } catch (...)
            {
                put_node(p);
                throw;
            }
            return p;
        }

        void destory_node(link_type p)
        {
            destory(&p->data);
            put_node(p);
        }

        void empty_initialize()
        {
            node = get_node();
            node->next = node;
            node->prev = node;
        }

        void fill_initialize(size_type n, const value_type& value)
        {
            empty_initialize();
            try
            {
                insert(begin(), n, value);
            } catch (...)
            {
                clear(); put_node(node);
                throw;
            }
        }

        template <typename InputIterator>
        void range_initialize(InputIterator first, InputIterator last)
        {
            empty_initialize();
            try
            {
                insert(begin(), first, last);
            } catch (...)
            {
                clear(); put_node(node);
                throw;
            }
        }

        template <typename InputIterator>
        void range_init(InputIterator first, InputIterator last)
        {
            iterator first1 = begin();
            iterator last1 = end();
            while (first1 != last1 && first != last)
            {
                *first1++ = *first++;
            }
            if (first1 != last1)
                erase(first1, last1);
            else
                insert(last1, first, last);
        }

        void transfer(iterator postion, iterator first, iterator last)
        {
            if (postion != last)
            {
                ((link_type)(last.node->prev))->next = postion.node;
                ((link_type)(first.node->prev))->next = last.node;
                ((link_type)(postion.node->prev))->next = first.node;
                link_type tmp = link_type(postion.node->prev);
                postion.node->prev = last.node->prev;
                last.node->prev = first.node->prev;
                first.node->prev = tmp;
            }
        }

    private:
        link_type node;
    };

    template <typename T, typename Alloc>
    template <typename InputIterator>
    void list<T, Alloc>::assign(InputIterator first, InputIterator last)
    {
        range_init(first, last);
    }

    template <typename T, typename Alloc>
    void list<T, Alloc>::assign(std::initializer_list<value_type> il)
    {
        range_init(il.begin(), il.end());
    }

    template <typename T, typename Alloc>
    void list<T, Alloc>::assign(size_type n, const value_type& value)
    {
        iterator first = begin();
        iterator last = end();
        while (first != last && (n--) > 0)
        {
            *first++ = value;
        }
        if (n == 0)
            erase(first, last);
        else
            insert(last, n, value);
    }

    template <typename T, typename Alloc>
    typename list<T, Alloc>::iterator 
    list<T, Alloc>::insert(iterator postion, const value_type& value)
    {
        link_type p = create_node(value);
        link_type prev_node = (link_type)(postion.node->prev);
        p->next = postion.node;
        p->prev = prev_node;
        prev_node->next = p;
        postion.node->prev = p;
        return p;
    }

    template <typename T, typename Alloc>
    typename list<T, Alloc>::iterator
    list<T, Alloc>::insert(iterator postion, size_type n, const value_type& value)
    {
        iterator tmp = postion;
        for (size_type i = 0; i < n; ++i)
        {
            if (i == 0)
                tmp = insert(postion, value);
            else
                insert(postion, value);
        }
        return tmp;
    }

    template <typename T, typename Alloc>
    template <typename InputIterator>
    typename list<T, Alloc>::iterator
    list<T, Alloc>::insert(iterator postion, InputIterator first, InputIterator last)
    {
        iterator tmp = postion;
        for (size_type i = 0; first != last; ++first, ++i)
        {
            if (i == 0)
                tmp = insert(postion, *first);
            else
                insert(postion, *first);
        }
        return tmp;
    }

    template <typename T, typename Alloc>
    typename list<T, Alloc>::iterator
    list<T, Alloc>::erase(iterator postion)
    {
        link_type prev_node = (link_type)(postion.node->prev);
        link_type next_node = (link_type)(postion.node->next);
        prev_node->next = next_node;
        next_node->prev = prev_node;
        destory_node(postion.node);
        return iterator(next_node);
    }

    template <typename T, typename Alloc>
    typename list<T, Alloc>::iterator
    list<T, Alloc>::erase(iterator first, iterator last)
    {
        while (first != last)
        {
            first = erase(first);
        }
        return last;
    }

    template <typename T, typename Alloc>
    void list<T, Alloc>::clear()
    {
        link_type cur = (link_type)(node->next);
        while (cur != node) 
        {
            link_type tmp = cur;
            cur = (link_type)cur->next;
            destory_node(tmp);
        }
        node->next = node;
        node->prev = node;
    }

    template <typename T, typename Alloc>
    void list<T, Alloc>::resize(size_type n, const value_type& value)
    {
        if (n < size())
        {
            iterator pos = begin();
            tinystl::advance(pos, n);
            erase(pos, end());
        }
        else
        {
            insert(end(), n - size(), value);
        }
    }
    
    template <typename T, typename Alloc>
    void list<T, Alloc>::resize(size_type n)
    {
        resize(n, value_type());
    }

    template <typename T, typename Alloc>
    void list<T, Alloc>::remove(const value_type& value)
    {
        iterator first = begin();
        iterator last = end();
        while (first != last)
        {
            if (*first == value)
            {
                first = erase(first);
                continue;
            }
            ++ first;
        }
    }

    template <typename T, typename Alloc>
    template <typename Predicate>
    void list<T, Alloc>::remove_if(Predicate pred)
    {
        iterator first = begin();
        iterator last = end();
        while (first != last)
        {
            if (pred(*first))
            {
                first = erase(first);
                continue;
            }
            ++ first;
        }
    }

    template <typename T, typename Alloc>
    void list<T, Alloc>::unique()
    {
        unique(std::equal_to<T>());
    }

    template <typename T, typename Alloc>
    template <typename BinaryPredicate>
    void list<T, Alloc>::unique(BinaryPredicate binary_pred)
    {
        iterator first = begin();
        iterator last = end();
        if (first == last) return;
        iterator next = ++first;
        while (next != last)
        {
            if (binary_pred(*next, *first))
                next = erase(next);
            else
            {
                ++first; ++next;
            }
        }
    }

    template <typename T, typename Alloc>
    void list<T, Alloc>::splice(iterator postion, list& x)
    {
        if (!x.empty())
            transfer(postion, x.begin(), x.end());
    }

    template <typename T, typename Alloc>
    void list<T, Alloc>::splice(iterator postion, list&& x)
    {
        if (!x.empty())
            transfer(postion, x.begin(), x.end());
    }

    template <typename T, typename Alloc>
    void list<T, Alloc>::splice(iterator postion, list& , iterator i)
    {
        iterator j = i;
        ++j;
        if (postion == i || postion == j) return;
        transfer(postion, i, j);
    }

    template <typename T, typename Alloc>
    void list<T, Alloc>::splice(iterator postion, list&& x, iterator i)
    {
        splice(postion, x, i);
    }

    template <typename T, typename Alloc>
    void list<T, Alloc>::splice(iterator postion, list& /* x */, iterator first, iterator last)
    {
        if (first != last)
            transfer(postion, first, last);
    }

    template <typename T, typename Alloc>
    void list<T, Alloc>::splice(iterator postion, list&& /* x */, iterator first, iterator last)
    {
        if (first != last)
            transfer(postion, first, last);
    }

    template <typename T, typename Alloc>
    void list<T, Alloc>::merge(list& x)
    {
        merge(x, std::less<T>());
    }

    template <typename T, typename Alloc>
    void list<T, Alloc>::merge(list&& x)
    {
        merge(x, std::less<T>());
    }

    template <typename T, typename Alloc>
    template <typename Compare>
    void list<T, Alloc>::merge(list& x, Compare cmp)
    {
        if (&x == this) return;

        iterator first1 = begin();
        iterator last1 = end();
        iterator first2 = x.begin();
        iterator last2 = x.end();
        while (first1 != last1 && first2 != last2)
        {
            if (!cmp(*first1, *first2))
            {
                iterator next = first2;
                transfer(first1, first2, ++next);
                first2 = next;
            }
            else
            {
                ++first1;
            }
        }
        if (first2 != last2)
            transfer(first1, first2, last2);
    }

    template <typename T, typename Alloc>
    template <typename Compare>
    void list<T, Alloc>::merge(list&& x, Compare cmp)
    {
        merge(x, cmp);
    }

    template <typename T, typename Alloc>
    void list<T, Alloc>::reverse() noexcept
    {
        if (node->next == node || (link_type(node->next))->next == node)
            return;
        iterator first = begin();
        ++first;
        while (first != end())
        {
            iterator old = first;
            ++first;
            transfer(begin(), old, first);
        }
    }

    template <typename T, typename Alloc>
    void list<T, Alloc>::sort()
    {
        sort(std::less<T>());
    }

    template <typename T, typename Alloc>
    template <typename Compare>
    void list<T, Alloc>::sort(Compare cmp)
    {
        if (node->next == node || (link_type(node->next))->next == node)
            return;
        
        int fill = 0;
        list<T, Alloc> carry;
        list<T, Alloc> counter[64];

        while (!empty())
        {
            int i = 0;
            carry.splice(carry.begin(), *this, begin());
            // cout << "size=" << carry.size() << endl;
            // cout << "=" <<  begin().node << endl;
            while (i < fill && !counter[i].empty())
            {
                counter[i].merge(carry, cmp);
                carry.swap(counter[i++]);
            }

            carry.swap(counter[i]);
            if (i == fill) ++fill;
        }

        for (int i = 1; i < fill; ++i)
            counter[i].merge(counter[i-1], cmp);
        swap(counter[fill - 1]);
    }


    template <typename T, typename Alloc>
    inline void swap(list<T, Alloc>& x, list<T, Alloc>& y)
    {
        x.swap(y);
    }

} /* tinystl */ 
#endif /* ifndef TINY_STL_LIST_H_ */
