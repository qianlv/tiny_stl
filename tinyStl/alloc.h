/*
 * =====================================================================================
 *
 *       Filename:  alloc.h
 *
 *    Description:  Alloc
 *
 *        Version:  1.0
 *        Created:  2016年05月05日 19时05分58秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  qianlv (), qianlv7 at gmail dot com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef TINY_STL_ALLOC_H_
#define TINY_STL_ALLOC_H_

#include <iostream>
#include <new>
#include <stdlib.h>
#include <string.h>

namespace tinystl
{
    
    template <int inst>
    class MallocAlloc
    {
        typedef void (*malloc_handler)();
    public:
        static void* allocate(size_t n);
        static void  deallocate(void* p, size_t /* n */);
        static void* reallocate(void* p, size_t /* old_sz */, size_t new_sz);
        static malloc_handler set_malloc_handler(malloc_handler handler);

    private:
        static void* oom_malloc(size_t n);
        static void* oom_realloc(void* p, size_t n);

    private:
        static malloc_handler malloc_oom_hanlder;
    };

    template <int inst>
    typename MallocAlloc<inst>::malloc_handler
    MallocAlloc<inst>::malloc_oom_hanlder = nullptr;

    template <int inst>
    void* MallocAlloc<inst>::allocate(size_t n)
    {
        void* result = malloc(n);
        if (result == 0)
            result = oom_malloc(n);
        return result;
    }

    template <int inst>
    void MallocAlloc<inst>::deallocate(void* p, size_t /* n */)
    {
        free(p);
    }

    template <int inst>
    void* MallocAlloc<inst>::reallocate(void* p, size_t /* old_sz */, size_t new_sz)
    {
        void* result = realloc(p, new_sz);
        if (result == 0)
            result = oom_realloc(p, new_sz);
        return result;
    }

    /* set out of memory handling, 类似 new 的 set_new_handler, 可以参考: http://www.cplusplus.com/reference/new/set_new_handler/ 
     * 和 Effective C++ 的 Item 49
     * */
    template <int inst>
    typename MallocAlloc<inst>::malloc_handler 
    MallocAlloc<inst>::set_malloc_handler(MallocAlloc::malloc_handler handler)
    {
        malloc_handler old = malloc_oom_hanlder;
        malloc_oom_hanlder = handler;
        return old;
    }

    template <int inst>
    void* MallocAlloc<inst>::oom_malloc(size_t n)
    {
        malloc_handler cur_handler = nullptr;
        void* result = nullptr;

        while (true)
        {
            cur_handler = malloc_oom_hanlder;
            if (cur_handler == nullptr)
            {
                throw std::bad_alloc();
            }

            (*cur_handler)();
            result = malloc(n);
            if (result) 
            {
                return result;
            }
        }
    }

    template <int inst>
    void* MallocAlloc<inst>::oom_realloc(void* p, size_t n)
    {
        malloc_handler cur_handler = nullptr;
        void* result = nullptr;

        while (true)
        {
            cur_handler = malloc_oom_hanlder;
            if (cur_handler == nullptr)
            {
                throw std::bad_alloc();
            }

            (*cur_handler)();
            result = realloc(p, n);
            if (result) 
            {
                return result;
            }
        }
    }

    typedef MallocAlloc<0> simple_alloc;

    template <int inst>
    class Alloc
    {
    public:
        static void* allocate(size_t n);
        static void  deallocate(void* p, size_t n);
        static void* reallocate(void* p, size_t old_sz, size_t new_sz);

    private:
        static void* refill(size_t n);
        static char* chunk_alloc(size_t size, int &nobjs);

        /*
         * bytes 调整为8的整数倍
         * 限制: ALIGN 为2^n 公式才成立
         */
        static size_t ROUND_UP(size_t bytes)
        {
            return ((bytes + ALIGN - 1) & ~(ALIGN - 1));
        }

        /*
         * bytes 大小确定用第几块free_lists
         */
        static size_t FREELIST_INDEX(size_t bytes)
        {
            return (bytes + ALIGN - 1) / ALIGN;
        }
    private:
        static const size_t ALIGN = 8;
        static const size_t MAX_BYTES = 128;
        static const size_t NFREELISTS = MAX_BYTES / ALIGN;

        union obj {
            union obj* next;
            char data[1];
        };

        /*
         * 8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 96, 104, 112, 120, 128
         */
        static obj* free_lists[NFREELISTS];

        static char* start_free;
        static char* end_free;
        static size_t heap_size;
    };

    // const size_t Alloc::Alloc;
    // const size_t Alloc::MAX_BYTES;
    // const size_t Alloc::NFREELISTS;

    template <int inst>
    typename Alloc<inst>::obj* Alloc<inst>::free_lists[Alloc::NFREELISTS] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    template <int inst> char* Alloc<inst>::start_free = nullptr;
    template <int inst> char* Alloc<inst>::end_free = nullptr;
    template <int inst> size_t Alloc<inst>::heap_size = 0;

    template <int inst>
    void* Alloc<inst>::allocate(size_t n)
    {
        if (n > MAX_BYTES)
            return simple_alloc::allocate(n);
        size_t index = FREELIST_INDEX(n);
        obj* free_item = free_lists[index];
        if (static_cast<obj*>(0) == free_item)
        {
            void* r = refill(ROUND_UP(n));
            return r;
        }
        free_lists[index] = free_item->next;
        return free_item;
    }
    
    template <int inst>
    void Alloc<inst>::deallocate(void* p, size_t n)
    {
        if (n > MAX_BYTES)
        {
            simple_alloc::deallocate(p, n);
        }
        else
        {
            obj* q = static_cast<obj*>(p);
            size_t index = FREELIST_INDEX(n);
            obj* cur = free_lists[index];
            q->next = cur;
            free_lists[index] = q;
        }
    }

    template <int inst>
    void* Alloc<inst>::reallocate(void* p, size_t old_sz, size_t new_sz)
    {
        if (old_sz > MAX_BYTES && new_sz > MAX_BYTES)
            return realloc(p, new_sz);
        if (ROUND_UP(old_sz) == ROUND_UP(new_sz))
            return p;
        void* result = allocate(new_sz);
        size_t copy_sz = new_sz > old_sz ? old_sz : new_sz;
        memcpy(result, p, copy_sz);
        deallocate(p, old_sz);
        return result;
    }

    template <int inst>
    void* Alloc<inst>::refill(size_t n)
    {
        int nobjs = 20;
        char* chunk = chunk_alloc(n, nobjs);
        if (nobjs == 1)
        {
            return chunk;
        }

        free_lists[FREELIST_INDEX(n)] = (obj*)(chunk + n);
        obj* cur = free_lists[FREELIST_INDEX(n)];
        obj* next = nullptr;
        for (int i = 1; ; ++i)
        {
            if (i == nobjs)
            {
                cur->next = nullptr;
                break;
            }
            next = (obj*)((char*)cur + n);
            cur->next = next;
            cur = next;
        }
        return chunk;
    }

    template <int inst>
    char* Alloc<inst>::chunk_alloc(size_t n, int& nobjs)
    {
        char* result;
        size_t total_bytes = n * nobjs;
        size_t left_bytes = end_free - start_free;
        if (left_bytes >= total_bytes)
        {
            result = start_free;
            start_free += total_bytes;
            return result;
        }
        else if (left_bytes >= n)
        {
            nobjs = left_bytes / n;
            result = start_free;
            start_free += n * nobjs;
            return result;
        }
        else                                    /* 从 heap 从新分配内存 */
        {
            size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
            if (left_bytes > 0)                 /* 多余内存分配给 free_lists */
            {
                obj* free_item = free_lists[FREELIST_INDEX(left_bytes)];
                obj* cur = (obj*)(start_free);
                cur->next = free_item;
                free_item = cur;
                // start_free += left_bytes;
            }

            start_free = static_cast<char*>(malloc(bytes_to_get));
            if (start_free == 0)                /* malloc 失败, 从 free_lists 中其他更大的内存块获取内存. */
            {
                for (int i = n; i <= MAX_BYTES; i += ALIGN)
                {
                    obj* free_item = free_lists[FREELIST_INDEX(i)];
                    if (free_item != static_cast<obj*>(0))
                    {
                        obj* p = free_item;
                        free_item = p->next;
                        start_free = (char*)(p);
                        end_free = start_free + i;
                        return chunk_alloc(n, nobjs);
                    }
                }
                end_free = 0;
                // MallocAlloc, 如果 malloc_handler (out of memery) 也无法获取内存, throw exception.
                start_free = static_cast<char*>(simple_alloc::allocate(bytes_to_get));
            }
            heap_size += bytes_to_get;
            end_free = start_free + bytes_to_get;
            return chunk_alloc(n, nobjs);
        }
    }

    typedef Alloc<0> alloc;

} // end namespace tinystl

#endif // TINY_STL_ALLOC_H_
