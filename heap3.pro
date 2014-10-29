#-------------------------------------------------
#
# Project created by QtCreator 2014-09-01T22:08:37
#
#-------------------------------------------------

QT       -= core
QT       -= gui

CONFIG += c++11

TARGET = heap3

TEMPLATE = lib
CONFIG -= staticlib
CONFIG -= static
CONFIG += shared


SOURCES += \
    main.cpp\
    src/mm.cpp\
    src/mm/allocator.cpp\
    src/mm/vm.cpp\
    src/mm/system.cpp\

#lockless_allocator/ll_alloc.c\

HEADERS += \
    include/mm.hpp\
    include/mm/sizeLayout.hpp\
    src/mm.ipp\
    src/mm/utils/intrusiveDeque.hpp\
    src/mm/utils/intrusiveDeque.ipp\
    src/mm/utils/sized_cast.hpp\
    src/mm/utils/sized_cast.ipp\
    src/mm/allocator.hpp\
    src/mm/allocator.ipp\
    src/mm/allocator/bitIndex.hpp\
    src/mm/allocator/bitIndex.ipp\
    src/mm/allocator/bitIndex/address.hpp\
    src/mm/allocator/bitIndex/level.hpp\
    src/mm/allocator/bitIndex/level.ipp\
    src/mm/allocator/bitIndex/orderEvaluator.hpp\
    src/mm/allocator/blocksHolder.hpp\
    src/mm/allocator/blocksHolder.ipp\
    src/mm/allocator/block.hpp\
    src/mm/allocator/block.ipp\
    src/mm/vm.hpp\
    src/mm/system.hpp\

INCLUDEPATH += include src

QMAKE_CXXFLAGS += -g
QMAKE_CFLAGS += -g
QMAKE_LFLAGS += -g

QMAKE_CXXFLAGS_RELEASE += -flto -O3 -DNDEBUG
QMAKE_CFLAGS_RELEASE += -flto -O3 -DNDEBUG
QMAKE_LFLAGS_RELEASE += -flto -O3


QMAKE_LFLAGS += \
    -Wl,--no-undefined \


PRECOMPILED_HEADER = stable.hpp

#release {
#    DEFINES += NDEBUG
#}
