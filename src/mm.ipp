#pragma once

#include "mm.hpp"
#include "mm/allocator.hpp"
#include "mm/system.hpp"

namespace mm
{
    namespace details
    {
        template <std::size_t sizeClass>
        inline void *allocBySizeClass()
        {
            return g_allocator.alloc<sizeClass>();
        }

        template <std::size_t sizeClass>
        inline void freeBySizeClass(void *ptr)
        {
            g_allocator.free<sizeClass>(ptr);
        }

        template <>
        inline void *allocBySizeClass<SizeLayout::_bigSizeClass>()
        {
            return system::malloc(SizeLayout::_bigSizeClass);
        }

        template <>
        inline void freeBySizeClass<SizeLayout::_bigSizeClass>(void *ptr)
        {
            system::free(ptr);
        }
    }

    inline void *alloc(std::size_t size)
    {
        return g_allocator.alloc(size);
    }

    inline void free(void *ptr)
    {
        return g_allocator.free(ptr);
    }

    inline std::size_t size(void *ptr)
    {
        return g_allocator.size(ptr);
    }

}

