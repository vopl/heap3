#include "stable.hpp"
#include "mm.hpp"
#include "mm/system.hpp"
#include "mm/allocator.hpp"

#include "mm.ipp"
#include "mm/allocator.ipp"
#include "mm/allocator/blocksHolder.ipp"
#include "mm/allocator/block.ipp"
#include "mm/utils/intrusiveDeque.ipp"
#include "mm/utils/sized_cast.ipp"
#include "mm/allocator/bitIndex.ipp"
#include "mm/allocator/bitIndex/level.ipp"

namespace mm
{
    namespace
    {
        /////////////////////////////////////////////////////////////////////////////////
        template <std::size_t sizeClass>
        std::uintptr_t bySizeClassInstantiate()
        {
            return
                bySizeClassInstantiate<SizeLayout::evalPrevSizeClass(sizeClass-1)>() +
                reinterpret_cast<std::uintptr_t>(&details::allocBySizeClass<sizeClass>) +
                reinterpret_cast<std::uintptr_t>(&details::freeBySizeClass<sizeClass>);
        }

        /////////////////////////////////////////////////////////////////////////////////
        template <>
        std::uintptr_t bySizeClassInstantiate<SizeLayout::_minSize>()
        {
            return
                reinterpret_cast<std::uintptr_t>(&details::allocBySizeClass<SizeLayout::_minSize>) +
                reinterpret_cast<std::uintptr_t>(&details::freeBySizeClass<SizeLayout::_minSize>);
        }

        static const volatile std::uintptr_t bySizeClassInstantiateSideEffect = bySizeClassInstantiate<SizeLayout::_maxSize>();
    }

}

