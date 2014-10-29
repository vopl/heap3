#pragma once

#include "mm/allocator/blocksHolder.hpp"
#include "mm/allocator/bitIndex.hpp"
#include "mm/sizeLayout.hpp"

namespace mm
{

    /////////////////////////////////////////////////////////////////////////////////////////
    class Allocator
    {
    public:
        Allocator() = delete;
        ~Allocator() = delete;

        void init();
        void deinit();

        ////////////////////////////////////////////////////////////////
        template <std::size_t sizeClass> void *alloc();
        template <std::size_t sizeClass> void free(void *ptr);

        void *alloc(std::size_t size);
        void free(void *ptr);
        size_t size(void *ptr);

        ////////////////////////////////////////////////////////////////
        void *allocateBlock();
        void deallocateBlock(void *ptr);

        bool vmAccessHandler(void *addr);

    private:
        using BitIndex = mm::allocator::BitIndex<SizeLayout::_blocksAmount>;

        static constexpr std::size_t _bitIndexAlignedSize = SizeLayout::alignUp(sizeof(BitIndex), SizeLayout::_pageSize);
        static constexpr std::size_t _vmSize =
                _bitIndexAlignedSize +
                SizeLayout::_blockSize +
                SizeLayout::_blocksAmount * SizeLayout::_blockSize;



        void *_vm;

        BitIndex *_bitIndex;
        void *_blocks;

    private:
        allocator::BlocksHolder _blocksHolders[SizeLayout::_maxSizeClassIndex+1];
    };

    extern Allocator &g_allocator;
}
