#pragma once

#include "mm/sizeLayout.hpp"

namespace mm
{
    namespace details
    {
        ////////////////////////////////////////////////////////////////
        template <std::size_t sizeClass> void *allocBySizeClass();
        template <std::size_t sizeClass> void freeBySizeClass(void *ptr);
    }

    ////////////////////////////////////////////////////////////////
    template <std::size_t size> void *alloc()
    {
        return details::allocBySizeClass<SizeLayout::evalSizeClass(size)>();
    }

    template <std::size_t size> void free(void *ptr)
    {
        return details::freeBySizeClass<SizeLayout::evalSizeClass(size)>(ptr);
    }

    ////////////////////////////////////////////////////////////////
    void *alloc(std::size_t size);
    void free(void *ptr);
    std::size_t size(void *ptr);
}

#include "mm.ipp"
