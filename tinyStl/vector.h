/*
 * =====================================================================================
 *
 *       Filename:  vector.h
 *
 *    Description:  vector
 *
 *        Version:  1.0
 *        Created:  2016年05月20日 11时23分09秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  qianlv (), qianlv7 at gmail dot com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef TINY_STL_VECTOR_H_
#define TINY_STL_VECTOR_H_ 

#include "tinyStl/alloc.h"
#include "tinyStl/uninitialized.h"
#include "tinyStl/construct.h"

#include <stddef.h>
#include <algorithm>
#include <initializer_list>
#include <stdexcept>
#include <assert.h>

#include <iostream>
using std::cout;
using std::endl;

namespace tinystl
{
    template <typename T, class Alloc = malloc_alloc>
    class vector
    {
    public:
        typedef T           value_type;
        typedef value_type* pointer;
        typedef value_type& reference;
        typedef value_type* iterator;
        typedef size_t      size_type;
        typedef ptrdiff_t   difference_type;
        typedef const value_type* const_iterator;
        typedef const value_type& const_reference;
        typedef const value_type* const_pointer;

    public:
        /*
         * ctor, dtor, assign
         */
        vector() : start(nullptr), finish(nullptr), end_of_storage(nullptr)
        {
        };

        explicit vector(const size_type n);
        template <typename InputIterator>
        vector(InputIterator first, InputIterator last);
        vector(std::initializer_list<value_type> il);

        vector(size_type n, const T& value)
        {
            fill_initialize(n, value);
        }
        vector (int n, const T& value)
        {
            fill_initialize(n, value);
        }
        vector (long n, const T& value)
        {
            fill_initialize(n, value);
        }

        vector(const vector& v);
        vector(vector&& v) noexcept;
        vector& operator=(const vector& v);
        vector& operator=(vector&& v);

        ~vector ()
        {
            tinystl::destory(start, finish);
            deallocate();
        }

        bool operator == (const vector& v) const;
        bool operator != (const vector& v) const;

        /*
         * Iterator 
         * TODO reverse_iterator
         */
        iterator begin() { return start; }
        iterator end()   { return finish; }
        const_iterator cbegin() const { return start; }
        const_iterator cend() const { return finish; }
        
        /*
         * Capacity
         */
        size_type size() const { return finish - start; }
        size_type capacity() const { return end_of_storage - start; }
        bool empty() const { return begin() == end(); }
        void resize(size_type n);
        void resize(size_type n, const value_type& value);
        void reserve(size_type n);
        void shrink_to_fit();

        /*
         * Element access
         */
        reference operator[](const difference_type n) { return *(begin() + n); }
        const_reference operator[](const difference_type n) const { return *(cbegin() + n); }
        reference at(size_type n)
        {
            check_index(n);
            return operator[](n);
        }
        const_reference at(size_type n) const
        {
            check_index(n);
            return operator[](n);
        }
        reference front() { return *begin(); }
        const_reference front() const { return *begin(); }
        reference back() { return *(end() - 1); }
        const_reference back() const { return *(end() - 1); }
        pointer data() noexcept;
        const_pointer data() const noexcept;

        /*
         * Modifiers data
         */
        template <typename InputIterator>
        void assign(InputIterator first, InputIterator last);
        void assign(size_type n, const value_type& value);
        void assign(std::initializer_list<value_type> il);
        void push_back(const T& value)
        {
            if (finish != end_of_storage)
            {
                tinystl::construct(finish, value);
                ++finish;
            }
            else
            {
                insert_aux(end(), value);
            }
        }

        // void push_back(value_type&& val);

        void pop_back()
        {
            --finish;
            tinystl::destory(finish);
        }

        iterator insert(iterator postion, const value_type& value);
        iterator insert(iterator postion, size_type n, const value_type& value);
        template <typename InputIterator>
        iterator insert(iterator postion, InputIterator first, InputIterator last);
        // void insert(iterator postion, value_type&& value);
        iterator insert(iterator postion, std::initializer_list<value_type> il);

        iterator erase(iterator first, iterator last)
        {
            iterator i = std::copy(last, finish, first);
            tinystl::destory(i, finish);
            finish = finish - (last - first);
            return first;
        }

        iterator erase(iterator postion)
        {
            return erase(postion, postion + 1);
        }
        
        void clear()
        {
            erase(begin(), end());
        }

        void swap(vector& v);

        template <typename... Args>
        iterator emplace(const_iterator postion, Args&&... args);


    private:

        size_type get_new_capacty(size_type n)
        {
            return size() == 0 ? n : size() + std::max(size(), n);
        }

        template <typename ForwardIterator>
        void allocte_and_copy(size_type n, ForwardIterator first, ForwardIterator last)
        {
            iterator new_start = data_allocator::allocate(n);
            iterator new_finish = new_start;
            try
            {
                new_finish = tinystl::uninitialized_copy(first, last, new_start);
            } catch (...)
            {
                // destory(new_start, new_finish);
                data_allocator::deallocate(new_start, n);
                throw;
            }

            destory(start, finish);
            deallocate();
            start = new_start;
            finish = new_finish;
            end_of_storage = start + n;
        }

        void fill_initialize(size_type n, const T& value)
        {
            start = data_allocator::allocate(n);
            finish = tinystl::uninitialized_fill_n(start, n, value);
            end_of_storage = finish;
        }

        void allocate_and_fill(size_type n, const T& value)
        {
            iterator new_start = data_allocator::allocate(n);
            iterator new_finish = new_start;
            try
            {
                new_finish = tinystl::uninitialized_fill_n(new_start, n, value);
            } catch (...)
            {
                data_allocator::deallocate(new_start, n);
                throw;
            }

            destory(start, finish);
            deallocate();
            start = new_start;
            finish = new_finish;
            end_of_storage = start + n;
        }

        void deallocate()
        {
            data_allocator::deallocate(start, capacity());
        }

        void insert_aux(iterator postion, const T& value);
        template <typename InputIterator>
        iterator range_insert(iterator postion,
                              InputIterator first,
                              InputIterator last,
                              tinystl::input_iterator_tag);

        template <typename ForwardIterator>
        iterator range_insert(iterator postion,
                              ForwardIterator first,
                              ForwardIterator last,
                              tinystl::forward_iterator_tag);

        void check_index(size_type n)
        {
            if (n > size())
            {
                throw std::out_of_range("vector check_index");
            }
        }

    private:
        iterator start;
        iterator finish;
        iterator end_of_storage;

        typedef simple_alloc<value_type, Alloc> data_allocator;
    };

    template <typename T, class Alloc>
    vector<T, Alloc>::vector(const vector& v)
    {
        start = data_allocator::allocate(v.size());
        finish = tinystl::uninitialized_copy(v.begin(), v.end(), start);
        end_of_storage = finish;
    }

    template <typename T, class Alloc>
    template <typename InputIterator>
    vector<T, Alloc>::vector(InputIterator first, InputIterator last)
        : vector()
    {
        insert(end(), first, last);
    }

    template <typename T, class Alloc>
    vector<T, Alloc>::vector(std::initializer_list<value_type> il)
        : vector(il.begin(), il.end())
    {
    }

    template <typename T, class Alloc>
    vector<T, Alloc>::vector(vector&& v) noexcept:
        start(v.start), finish(v.finish), end_of_storage(v.end_of_storage)
    {
        v.start = v.finish = v.end_of_storage = nullptr;
    }

    template <typename T, class Alloc>
    vector<T, Alloc>& vector<T, Alloc>::operator=(const vector& v)
    {
        vector tmp(v);
        swap(tmp);
        return *this;
    }

    template <typename T, class Alloc>
    vector<T, Alloc>& vector<T, Alloc>::operator=(vector&& v)
    {
        vector tmp(std::move(v)); // right ?
        swap(tmp);
        return *this;
    }

    template <typename T, class Alloc>
    bool vector<T, Alloc>::operator==(const vector& v) const
    {
        if (size() != v.size())
        {
            return false;
        }
        
        for (size_type i = 0; i < size(); ++i)
        {
            if (*(start + i) != v[i])
            {
                return false;
            }
        }
        return true;
    }

    template <typename T, class Alloc>
    bool vector<T, Alloc>::operator!=(const vector& v) const
    {
        return !(*this == v);
    }

    /*
     * Capacity
     */

    template <typename T, class Alloc>
    void vector<T, Alloc>::resize(size_type n)
    {
        resize(n, value_type());
    }

    template <typename T, class Alloc>
    void vector<T, Alloc>::resize(size_type n,  const value_type& value)
    {
        if (n < size())
        {
            erase(begin() + n, end());
        }
        else
        {
            insert(end(), n - size(), value);
        }
    }

    template <typename T, class Alloc>
    void vector<T, Alloc>::reserve(size_type n)
    {
        if (n <= capacity())
            return;
        allocte_and_copy(n, begin(), end());
    }

    template <typename T, class Alloc>
    void vector<T, Alloc>::shrink_to_fit()
    {
        if (size() == capacity())
            return;
        allocte_and_copy(size(), begin(), end());
    }
    
    /*
     * Modifiers data
     */

    template <typename T, typename Alloc>
    template <typename InputIterator>
    void vector<T, Alloc>::assign(InputIterator first, InputIterator last)
    {
        size_type n = tinystl::distance(first, last);
        if (n <= capacity()) 
        {
            destory(start, finish);
            finish -= size();
            finish = tinystl::uninitialized_copy(first, last, start);
        }
        else
        {
            size_type new_size = get_new_capacty(n - size());
            allocte_and_copy(new_size, first, last);
        }
    }

    template <typename T, typename Alloc>
    void vector<T, Alloc>::assign(std::initializer_list<value_type> il)
    {
        assign(il.begin(), il.end());
    }

    template <typename T, typename Alloc>
    void vector<T, Alloc>::assign(size_type n, const value_type& value)
    {
        if (n <= capacity())
        {
            destory(start, finish);
            finish -= size();
            finish = tinystl::uninitialized_fill_n(start, n, value);
        }
        else
        {
            allocate_and_fill(n, value);
        }
    }

    template <typename T, typename Alloc>
    typename vector<T, Alloc>::iterator
    vector<T, Alloc>::insert(iterator postion, const value_type& value)
    {
        return insert(postion, 1, value);
    }

    template <typename T, class Alloc>
    typename vector<T, Alloc>::iterator
    vector<T, Alloc>::insert(iterator postion, size_type n, const T& value)
    {
        size_type dis = postion - begin();
        if (end_of_storage - finish >= n) 
        {
            T v_copy = value;
            const size_type elems_after = finish - postion;
            iterator old_finish = finish;
            if (elems_after > n)
            {
                finish = tinystl::uninitialized_copy(finish - n, finish, finish);
                std::copy_backward(postion, old_finish - n, old_finish);
                std::fill(postion, postion + n, v_copy);
            }
            else
            {
                finish = tinystl::uninitialized_fill_n(finish, n - elems_after, v_copy);
                finish = tinystl::uninitialized_copy(postion, old_finish, finish);
                std::fill(postion, postion + n, v_copy);
            }
        } 
        else 
        { 
            const size_type new_size = get_new_capacty(n);
            iterator new_start = data_allocator::allocate(new_size);
            iterator new_finish = new_start;

            try
            {
                new_finish = tinystl::uninitialized_copy(start, postion, new_start);
                new_finish = tinystl::uninitialized_fill_n(new_finish, n, value);
                new_finish = tinystl::uninitialized_copy(postion, finish, new_finish);
            }
            catch (...)
            {
                // destory(new_start, new_finish);
                data_allocator::deallocate(new_start, new_size);
                throw;
            }

            destory(start, finish);
            deallocate();

            start = new_start;
            finish = new_finish;
            end_of_storage = new_start + new_size;
        }
        return begin() + dis;
    }

    template <typename T, typename Alloc>
    template <typename InputIterator>
    typename vector<T, Alloc>::iterator
    vector<T, Alloc>::insert(iterator postion, InputIterator first, InputIterator last)
    {
        return range_insert(postion, first, last, tinystl::iterator_category(first));
    }

    template <typename T, typename Alloc>
    typename vector<T, Alloc>::iterator
    vector<T, Alloc>::insert(iterator postion, std::initializer_list<value_type> il)
    {
        return range_insert(postion, il.begin(), il.end(), tinystl::iterator_category(il.begin()));
    }

    template <typename T, typename Alloc>
    void vector<T, Alloc>::insert_aux(iterator postion, const T& value)
    {
        if (finish != end_of_storage)
        {
            tinystl::construct(finish, *(finish - 1));
            ++finish;
            T v_copy = value; // value本身可能是vector中的值, 备份避免被修改
            std::copy_backward(postion, finish - 2, finish - 1);
            *postion = value;
        }
        else
        {
            const size_type new_size = get_new_capacty(1);
            iterator new_start = data_allocator::allocate(new_size);
            iterator new_finish = new_start;
            try
            {
                new_finish = tinystl::uninitialized_copy(start, postion, new_start);
                construct(new_finish, value);
                ++new_finish;
                new_finish = tinystl::uninitialized_copy(postion, finish, new_finish);
            } catch (...)
            {
                // destory(new_start, new_finish);
                data_allocator::deallocate(new_start, new_size);
                throw;
            }

            destory(start, finish);
            deallocate();

            start = new_start;
            finish = new_finish;
            end_of_storage = start + new_size;
        }
    }

    template <typename T, typename Alloc>
    template <typename InputIterator>
    typename vector<T, Alloc>::iterator
    vector<T, Alloc>::range_insert(iterator postion,
                                   InputIterator first,
                                   InputIterator last,
                                   tinystl::input_iterator_tag)
    {
        size_type n = 0;
        for (; first != last; ++first)
        {
            postion = insert(postion, *first);
            ++postion;
            ++n;
        }
        return postion - n;
    }

    template <typename T, typename Alloc>
    template <typename ForwardIterator>
    typename vector<T, Alloc>::iterator
    vector<T, Alloc>::range_insert(iterator postion,
                                   ForwardIterator first,
                                   ForwardIterator last,
                                   tinystl::forward_iterator_tag)
    { 
        size_type dis = begin() - postion;
        size_type n = tinystl::distance(first, last);
        if (end_of_storage - finish >= n)
        {
            size_type elems_after = finish - postion;
            iterator old_finish = finish;
            if (elems_after > n)
            {
                finish = tinystl::uninitialized_copy(finish - n, finish, finish);
                std::copy_backward(postion, old_finish - n, old_finish);
                std::copy(first, last, postion);
            }
            else
            {
                ForwardIterator mid = first;
                tinystl::advance(mid, elems_after);
                finish = tinystl::uninitialized_copy(mid, last, finish);
                finish = tinystl::uninitialized_copy(postion, old_finish, finish);
                std::copy(first, mid, postion);
            }
        }
        else
        {
            const size_type new_size = get_new_capacty(n);
            iterator new_start = data_allocator::allocate(new_size);
            iterator new_finish = new_start;

            try
            {
                new_finish = tinystl::uninitialized_copy(start, postion, new_start);
                new_finish = tinystl::uninitialized_copy(first, last, new_finish);
                new_finish = tinystl::uninitialized_copy(postion, finish, new_finish);
            } catch (...)
            {
                // destory(new_start, new_finish);
                data_allocator::deallocate(new_start, new_size);
                throw;
            }
            
            destory(start, finish);
            deallocate();

            start = new_start;
            finish = new_finish;
            end_of_storage = new_start + new_size;
        }
        
        return begin() + dis;
    }

    template <typename T, typename Alloc>
    void vector<T, Alloc>::swap(vector& v)
    {
        using std::swap;
        swap(start, v.start);
        swap(finish, v.finish);
        swap(end_of_storage, v.end_of_storage);
    }

} /* tinystl */ 
#endif /* ifndef TINY_STL_VECTOR_H_  */
