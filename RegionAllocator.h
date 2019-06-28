//
// Created by rasp on 19-5-30.
//

#ifndef ALLOCATOR_REGIONALLOCATOR_H
#define ALLOCATOR_REGIONALLOCATOR_H

#include <cstddef>
#include <limits>
#include <memory>

#include "RegionMemoryPool.h"

namespace MyLib {

    template<class T>
    class RegionAllocator {
    public:
        // Member types
        typedef T value_type;
        typedef T *pointer;
        typedef T &reference;
        typedef const T *const_pointer;
        typedef const T &const_reference;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef unsigned char data_type;
        typedef unsigned char *data_pointer;

        RegionAllocator() noexcept {};

        template<class U>
        RegionAllocator(const RegionAllocator<U> &) noexcept {};
        template<class U>
        bool operator==(const RegionAllocator<U> &) const noexcept;
        template<class U>
        bool operator!=(const RegionAllocator<U> &) const noexcept;

        pointer allocate(size_type n, const_pointer hint = 0) {
            return reinterpret_cast<T *>(RegionMemoryPool::getInstance().allocate(n * sizeof(T)));
        };

        void deallocate(pointer p, size_type n) {
            RegionMemoryPool::getInstance().deallocate(reinterpret_cast<char *>(p), n);
        };
    };

    template<class T>
    template<class U>
    bool RegionAllocator<T>::operator==(const RegionAllocator<U> &) const noexcept {
        return true;
    }

    template<class T>
    template<class U>
    bool RegionAllocator<T>::operator!=(const RegionAllocator<U> &) const noexcept {
        return false;
    }
}

#endif
