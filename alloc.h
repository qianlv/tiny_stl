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

namespace tinystl
{

    class MallocAlloc
    {
    public:
        static void* allocate(size_t n);
        static void* deallocate(void* p, size_t /* n */);
        static void* reallocate(void* p, size_t /* old_sz */, size_t new_sz);
        static malloc_handler set_malloc_handler(malloc_handler handler) noexcept;

    private:
        static void* oom_malloc(size_t n);
        static void* oom_realloc(void* p, size_t n);

    private:
        typedef void (*malloc_handler)();
        static malloc_handler malloc_oom_hanlder;
    };

    MallocAlloc::malloc_oom_hanlder = nullptr;

    void* MallocAlloc::allocate(size_t n)
    {
        void* result = malloc(n);
        if (result == 0)
            result = oom_malloc(n);
        return result;
    }

    void* MallocAlloc::deallocate(void* p, size_t /* n */)
    {
        free(p);
    }

    void* MallocAlloc::reallocate(void* p, size_t /* old_sz */, size_t new_sz)
    {
        void* result = realloc(p, new_sz);
        if (result == 0)
            result = oom_realloc(p, new_sz);
        return result;
    }

    /* set out of memory handling, 类似 new 的 set_new_handler, 可以参考: http://www.cplusplus.com/reference/new/set_new_handler/ 
     * 和 Effective C++ 的 Item 49
     * */
    MallocAlloc::malloc_handler MallocAlloc::set_malloc_handler(malloc_handler handler)
    {
        malloc_handler old = malloc_oom_hanlder;
        malloc_oom_hanlder = handler;
        return old;
    }

    void* MallocAlloc::oom_malloc(size_t n)
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

    void* MallocAlloc::oom_realloc(void* p, size_t n)
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

    class Alloc
    {
    public:
        static void* allocate(size_t n);
        static void* deallocate(void* p, size_t n);
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
            return (bytes + ALIGN - 1) / ALIGN
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

    const size_t Alloc::Alloc;
    const size_t Alloc::MAX_BYTES;
    const size_t Alloc::NFREELISTS;

    Alloc::obj* Alloc::free_lists = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    }

    char* Alloc::start_free = nullptr;
    char* Alloc::end_free = nullptr;
    size_t Alloc::heap_size = 0;

    void* Alloc::allocate(size_t n)
    {
        if (n > MAX_BYTES)
            return malloc(n);
        size_t index = FREELIST_INDEX(n);
        obj* free_item = free_list[index];
        if (static_cast<obj*>(0) == free_item)
        {
            void* r = refill(ROUND_UP(n));
            return r;
        }
        free_lists[index] = free_item->next;
        return free_item;
    }

    void Alloc::deallocate(void* p, size_t n)
    {
        if (n > MAX_BYTES)
        {
            free(p);
        }
        else
        {
            size_t index = FREELIST_INDEX(n);
            obj* cur = free_lists[index];
            p->next = cur;
            free_lists[index] = p;
        }
    }

    void* Alloc::reallocate(void* p, size_t old_sz, size_t new_sz)
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

    void* Alloc::refill(size_t n)
    {
        int nobjs = 20;
        char* chunk = chunk_alloc(n, nobjs);
        if (nobjs == 1)
        {
            return chunk;
        }

        free_lists[FREELIST_INDEX[n]] = static_cast<obj*>(chunk + n);
        obj* cur = free_lists[FREELIST_INDEX(n)];
        obj* next = nullptr;
        for (int i = 1; ; ++i)
        {
            if (i == nobjs)
            {
                cur->next = nullptr;
                break;
            }
            next = static_cast<obj*>((static_cast<char*>(cur) + n));
            cur->next = next;
            cur = next;
        }
        return chunk;
    }

    char* Alloc::chunk_alloc(size_t n, int& nobjs)
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
                obj* cur = static_cast<obj*>(start_free);
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
                        start_free = static_cast<char*>(p);
                        end_free = start_free + i;
                        return chunk_alloc(n, nobjs);
                    }
                }
                end_free = 0;
                // throw exceptions ?
            }
            heap_size += bytes_to_get;
            end_free = start_free + bytes_to_get;
            return chunk_alloc(n, nobjs);
        }
    }
} // end namespace tinystl

#endif // TINY_STL_ALLOC_H_
