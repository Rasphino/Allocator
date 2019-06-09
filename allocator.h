//
// Created by rasp on 19-5-30.
//

#ifndef ALLOCATOR_ALLOCATOR_H
#define ALLOCATOR_ALLOCATOR_H

#include <cstddef>
#include <limits>
#include <memory>

#include "MemoryPool.h"

namespace MyLib {

    template<class T>
    class allocator {
    public:
        // Member types
        typedef T value_type;
        typedef T* pointer;
        typedef T& reference;
        typedef const T* const_pointer;
        typedef const T& const_reference;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef unsigned char data_type;
        typedef unsigned char* data_pointer;

        allocator() noexcept;

        template<class U>
        allocator(const allocator<U>&) noexcept;

        template<class U>
        bool operator==(const allocator<U>&) const noexcept;

        template<class U>
        bool operator!=(const allocator<U>&) const noexcept;

        pointer allocate(size_type n, const_pointer hint = 0);

        void deallocate(pointer p, size_type n);
    };

    template<class T>
    allocator<T>::allocator() noexcept {}

    template<class T>
    template<class U>
    allocator<T>::allocator(const allocator<U>&) noexcept {}

    template<class T>
    template<class U>
    bool allocator<T>::operator==(const allocator<U>&) const noexcept {
        return true;
    }

    template<class T>
    template<class U>
    bool allocator<T>::operator!=(const allocator<U>&) const noexcept {
        return false;
    }

    template<class T>
    typename allocator<T>::pointer allocator<T>::allocate(allocator::size_type n, allocator::const_pointer hint) {
        return reinterpret_cast<T*>(MemoryPool::getInstance().allocate(n * sizeof(T)));
    }

    template<class T>
    void allocator<T>::deallocate(T* p, size_t n) {
        MemoryPool::getInstance().deallocate(reinterpret_cast<char*>(p), n);
    }

}

#endif // ALLOCATOR_ALLOCATOR_H
