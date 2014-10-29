#include "stable.hpp"

#include "mm/allocator.hpp"
#include "mm/vm.hpp"
#include "mm/system.hpp"

#include "mm/allocator.ipp"
#include "mm/utils/intrusiveDeque.ipp"
#include "mm/utils/sized_cast.ipp"
#include "mm/allocator/bitIndex.ipp"
#include "mm/allocator/bitIndex/level.ipp"
#include "mm/allocator/blocksHolder.ipp"
#include "mm/allocator/block.ipp"

namespace mm
{
    namespace
    {
        bool g_vmAccessHandler(void *addr)
        {
            return g_allocator.vmAccessHandler(addr);
        }
    }

    void Allocator::init()
    {
        _vm = vm::alloc(_vmSize);

        if(!_vm)
        {
            std::abort();
        }

        _bitIndex = new(_vm) BitIndex;

        _blocks = utils::sized_cast<void *>(SizeLayout::alignUp(utils::sized_cast<std::size_t>(_vm) + _bitIndexAlignedSize, SizeLayout::_blockSize));

        if(!vm::threadInit(g_vmAccessHandler))
        {
            std::abort();
        }
    }

    void Allocator::deinit()
    {
        assert(_bitIndex);
        if(_bitIndex)
        {
            _bitIndex->~BitIndex();
            _bitIndex = nullptr;
        }
        _blocks = nullptr;

        assert(_vm);
        vm::free(_vm, _vmSize);
        _vm = nullptr;

        vm::threadDeinit(&g_vmAccessHandler);
    }

    bool Allocator::vmAccessHandler(void *addr)
    {
        if(likely(utils::sized_cast<std::size_t>(addr) - utils::sized_cast<std::size_t>(_blocks) < utils::sized_cast<std::size_t>(SizeLayout::_blocksAmount * SizeLayout::_blockSize)))
        {
#ifndef NDEBUG
            allocator::bitIndex::Address blockBitAddr = (utils::sized_cast<char *>(addr) - utils::sized_cast<char *>(_blocks)) / SizeLayout::_blockSize;
            assert(_bitIndex->isAllocated(blockBitAddr));
#endif
            void *pagePtr = utils::sized_cast<void *>(utils::sized_cast<std::size_t>(addr) / SizeLayout::_pageSize * SizeLayout::_pageSize);

            if(!vm::protect(pagePtr, SizeLayout::_pageSize, true))
            {
                std::abort();
                return false;
            }

            return true;
        }
        return false;
    }

    namespace
    {
        std::aligned_storage<sizeof(Allocator), alignof(Allocator)>::type g_allocator_area;
    }
    Allocator &g_allocator(*(Allocator *)(&g_allocator_area));
}

