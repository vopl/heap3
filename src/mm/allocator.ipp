#pragma once

#include "mm/allocator.hpp"
#include "mm/utils/sized_cast.hpp"
#include "mm/vm.hpp"

#include <malloc.h>

namespace mm
{
    template <std::size_t sizeClass>
    void *Allocator::alloc()
    {
        const std::size_t sizeClassIndex = SizeLayout::evalSizeClassIndex(sizeClass);
        return _blocksHolders[sizeClassIndex].alloc((sizeClassIndex+1)*SizeLayout::_sizeGranula);
    }

    template <std::size_t sizeClass> void Allocator::free(void *ptr)
    {
#ifndef NDEBUG
        allocator::bitIndex::Address blockBitAddr = (utils::sized_cast<char *>(ptr) - utils::sized_cast<char *>(_blocks)) / SizeLayout::_blockSize;
        assert(_bitIndex->isAllocated(blockBitAddr));
#endif

        void *blockPtr = utils::sized_cast<void *>(utils::sized_cast<std::size_t>(ptr) / SizeLayout::_blockSize * SizeLayout::_blockSize);
        allocator::Block *block = utils::sized_cast<allocator::Block *>(blockPtr);

        block->free<sizeClass>(ptr);
    }

    inline void *Allocator::alloc(std::size_t size)
    {
        std::size_t sizeClassIndex;
        if(unlikely(!size))
        {
            sizeClassIndex = 0;
        }
        else
        {
            sizeClassIndex = (size-1)/SizeLayout::_sizeGranula;

            if(unlikely(sizeClassIndex > SizeLayout::_maxSizeClassIndex))
            {
                return system::malloc(size);
            }
        }

        return _blocksHolders[sizeClassIndex].alloc((sizeClassIndex+1)*SizeLayout::_sizeGranula);
    }

    inline void Allocator::free(void *ptr)
    {
        if(unlikely(utils::sized_cast<std::size_t>(ptr) - utils::sized_cast<std::size_t>(_blocks) >= utils::sized_cast<std::size_t>(SizeLayout::_blocksAmount * SizeLayout::_blockSize)))
        {
            return system::free(ptr);
        }

#ifndef NDEBUG
        allocator::bitIndex::Address blockBitAddr = (utils::sized_cast<char *>(ptr) - utils::sized_cast<char *>(_blocks)) / SizeLayout::_blockSize;
        assert(_bitIndex->isAllocated(blockBitAddr));
#endif

        void *blockPtr = utils::sized_cast<void *>(utils::sized_cast<std::size_t>(ptr) / SizeLayout::_blockSize * SizeLayout::_blockSize);
        allocator::Block *block = utils::sized_cast<allocator::Block *>(blockPtr);

        block->free(ptr);
    }

    inline std::size_t Allocator::size(void *ptr)
    {
        if(unlikely(utils::sized_cast<std::size_t>(ptr) - utils::sized_cast<std::size_t>(_blocks) >= utils::sized_cast<std::size_t>(SizeLayout::_blocksAmount * SizeLayout::_blockSize)))
        {
            return ::malloc_usable_size(ptr);
        }

#ifndef NDEBUG
        allocator::bitIndex::Address blockBitAddr = (utils::sized_cast<char *>(ptr) - utils::sized_cast<char *>(_blocks)) / SizeLayout::_blockSize;
        assert(_bitIndex->isAllocated(blockBitAddr));
#endif

        void *blockPtr = utils::sized_cast<void *>(utils::sized_cast<std::size_t>(ptr) / SizeLayout::_blockSize * SizeLayout::_blockSize);
        allocator::Block *block = utils::sized_cast<allocator::Block *>(blockPtr);

        return block->getSizeClass();
    }

    inline void *Allocator::allocateBlock()
    {
        if(unlikely(!_vm))
        {
            init();
        }
        assert(_bitIndex && _blocks);

        allocator::bitIndex::Address bitAddr = _bitIndex->allocate();
        return static_cast<char *>(_blocks) + bitAddr * SizeLayout::_blockSize;
    }

    inline void Allocator::deallocateBlock(void *ptr)
    {
        assert(_bitIndex && _blocks);

        allocator::bitIndex::Address bitAddr = (static_cast<char *>(ptr) - static_cast<char *>(_blocks)) / SizeLayout::_blockSize;
        assert(_bitIndex->isAllocated(bitAddr));
        assert(ptr == static_cast<char *>(_blocks) + bitAddr * SizeLayout::_blockSize);
        _bitIndex->deallocate(bitAddr);
    }
}

