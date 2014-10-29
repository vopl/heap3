#include "stable.hpp"
#include "mm.hpp"
#include "mm/system.hpp"

#include "mm.ipp"
#include "mm/allocator.ipp"
#include "mm/allocator/blocksHolder.ipp"
#include "mm/allocator/block.ipp"
#include "mm/utils/intrusiveDeque.ipp"
#include "mm/utils/sized_cast.ipp"
#include "mm/allocator/bitIndex.ipp"
#include "mm/allocator/bitIndex/level.ipp"

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    free(malloc(220));
    return 0;
}


namespace
{
    std::atomic_flag mtx_954129 = ATOMIC_FLAG_INIT;


    struct Locker
    {
        Locker()
        {
            while(mtx_954129.test_and_set(std::memory_order_acquire))
            {
                std::this_thread::yield();
            }
        }

        ~Locker()
        {
            mtx_954129.clear(std::memory_order_release);
        }
    };
}

extern "C"
{
    void *mm_malloc(size_t size)
    {
        Locker l;
        return mm::alloc(size);
    }

    void *mm_calloc(size_t n, size_t size)
    {
        Locker l;

        void *res = mm::alloc(size*n);
        ::memset(res, 0, size*n);

        return res;
    }

    void *mm_realloc(void *ptr, size_t size)
    {
        if(likely(ptr))
        {
            Locker l;

            size_t oldSize = mm::size(ptr);
            if(unlikely(oldSize >= size))
            {
                return ptr;
            }
            else
            {
                void *mem = mm::alloc(size);
                memcpy(mem, ptr, oldSize);
                mm::free(ptr);

                return mem;
            }
        }

        Locker l;
        return mm::alloc(size);
    }

    void mm_free(void *mem)
    {
        Locker l;
        return mm::free(mem);
    }





#pragma GCC visibility push(default)
    void *malloc(size_t)                        __attribute__ ((alias ("mm_malloc")));
    void *calloc(size_t, size_t)                __attribute__ ((alias ("mm_calloc")));
    void *realloc(void *, size_t)               __attribute__ ((alias ("mm_realloc")));
    void free(void *)                           __attribute__ ((alias ("mm_free")));

#pragma GCC visibility pop

}
