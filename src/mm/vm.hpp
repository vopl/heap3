#pragma once

namespace mm { namespace vm
{
    typedef bool (*TVmAccessHandler)(void *addr);

    bool threadInit(TVmAccessHandler accessHandler);
    bool threadDeinit(TVmAccessHandler accessHandler);

    void *alloc(std::size_t size);
    bool free(void *addr, std::size_t size);

    bool protect(void *addr, std::size_t size, bool access);
}}
