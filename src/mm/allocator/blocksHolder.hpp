#pragma once

#include "mm/allocator/block.hpp"
#include "mm/utils/intrusiveDeque.hpp"

namespace mm { namespace allocator
{
    ////////////////////////////////////////////////////////////////////////////////
    class BlocksHolder
    {
    public:
        BlocksHolder() = delete;
        ~BlocksHolder();

    public:
        template <std::size_t sizeClass> void *alloc();

        void *alloc(std::size_t sizeClass);

    public:
        //callbacks to block
        void blockEmptyToMiddle(Block *block);
        void blockMiddleToEmpty(Block *block);
        void blockMiddleToFull(Block *block);
        void blockFullToMiddle(Block *block);

    private:
        utils::IntrusiveDeque<Block> _empty;
        utils::IntrusiveDeque<Block> _middle;
        utils::IntrusiveDeque<Block> _full;
    };
}}
