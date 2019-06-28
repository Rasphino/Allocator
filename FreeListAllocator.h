//
// Created by rasp on 19-6-28.
//

#ifndef ALLOCATOR_FREELISTALLOCATOR_H
#define ALLOCATOR_FREELISTALLOCATOR_H

#include <cstddef>
#include <limits>
#include <memory>

#include "FreeListMemoryPool.h"

namespace MyLib {

    template<class T>
    class FreeListAllocator {
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

        FreeListAllocator() noexcept {};

        template<class U>
        FreeListAllocator(const FreeListAllocator<U> &) noexcept {};
        template<class U>
        bool operator==(const FreeListAllocator<U> &) const noexcept;
        template<class U>
        bool operator!=(const FreeListAllocator<U> &) const noexcept;

        pointer allocate(size_type n, const_pointer hint = 0) {
            return reinterpret_cast<T *>(FreeListMemoryPool::getInstance().allocate(n * sizeof(T)));
        };

        void deallocate(pointer p, size_type n) {
            FreeListMemoryPool::getInstance().deallocate(reinterpret_cast<char *>(p), n, sizeof(T));
        };
    };

    template<class T>
    template<class U>
    bool FreeListAllocator<T>::operator==(const FreeListAllocator<U> &) const noexcept {
        return true;
    }

    template<class T>
    template<class U>
    bool FreeListAllocator<T>::operator!=(const FreeListAllocator<U> &) const noexcept {
        return false;
    }
}

#endif //ALLOCATOR_FREELISTALLOCATOR_H
