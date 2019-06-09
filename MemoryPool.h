//
// Created by rasp on 19-6-9.
//

#ifndef ALLOCATOR_MEMORYPOOL_H
#define ALLOCATOR_MEMORYPOOL_H

#include <cstddef>
#include <iostream>
#include <list>

namespace MyLib {

    class MemoryPool {
    public:
        MemoryPool();
        ~MemoryPool();

        static MemoryPool& getInstance();

        // no copies
        MemoryPool(const MemoryPool&) = delete;
        void operator=(const MemoryPool&) = delete;

        // Allocate block of storage
        void* allocate(size_t size);

        // Release block of storage
        void deallocate(char* ptr, size_t n, size_t size);

    private:
        static constexpr size_t BLOCK_SIZE = 262144;

        std::list<void*> freeList4Byte, freeList8Byte, usedBlock;
        std::list<std::pair<void*, int>> freeListEtcByte;
        char *currPos = nullptr, *lastPos = nullptr;

        void allocateBlock(size_t n);
    };

    MemoryPool::MemoryPool() {}

    MemoryPool::~MemoryPool() {
        for (const auto& block : usedBlock) {
            free(block);
        }
    }

    MemoryPool& MemoryPool::getInstance() {
        static MemoryPool instance;
        return instance;
    }

    void* MemoryPool::allocate(size_t size) {
        if (size == 4 && !freeList4Byte.empty()) {
            auto itr = freeList4Byte.begin();
            freeList4Byte.erase(itr);
            printf("reuse %p for 4 byte\n", itr);
            return *itr;
        } else if (size == 8 && !freeList8Byte.empty()) {
            auto itr = freeList4Byte.begin();
            freeList4Byte.erase(itr);
            printf("reuse %p for 8 byte\n", itr);
            return *itr;
        } else if (size > 8) {
            for (auto itr = freeListEtcByte.begin(); itr != freeListEtcByte.end(); ++itr) {
                if (itr->second >= size + 4) {
                    size_t res = itr->second - size;
                    if (res == 4) {
                        freeList4Byte.push_back(reinterpret_cast<char*>(itr->first) + size);
                    } else if (res == 8) {
                        freeList8Byte.push_back(reinterpret_cast<char*>(itr->first) + size);
                    } else if (res > 8) {
                        freeListEtcByte.emplace_back(reinterpret_cast<char*>(itr->first) + size, res);
                    }
                } else if (itr->second == size) {
                    freeListEtcByte.erase(itr);
                    return itr->first;
                }
            }
        }

        if (currPos + size > lastPos) {
            int n = size / BLOCK_SIZE + 1;
            allocateBlock(n);
        }

        void* p = currPos;
        currPos = currPos + size;
        return p;
    }

    void MemoryPool::deallocate(char* ptr, size_t n, size_t size) {
        size_t res = size * n;
        if (res == 4) {
            freeList4Byte.push_back(ptr);
            printf("add %p for 4 byte\n", ptr);
        } else if (res == 8) {
            freeList8Byte.push_back(ptr);
            printf("add %p for 8 byte\n", ptr);
        } else if (res > 8) {
            freeListEtcByte.emplace_back(ptr, res);
            printf("add %p for %ld byte\n", ptr, res);
        }
    }

    void MemoryPool::allocateBlock(size_t n) {
        size_t res = lastPos - currPos;
        if (res == 4) {
            freeList4Byte.push_back(currPos);
        } else if (res == 8) {
            freeList8Byte.push_back(currPos);
        } else if (res > 8) {
            freeListEtcByte.emplace_back(currPos, res);
        }
        currPos = reinterpret_cast<char*>(::operator new(n* BLOCK_SIZE));
        lastPos = currPos + n * BLOCK_SIZE;
        usedBlock.push_back(currPos);
        printf("new block %p for %ld byte\n", currPos, n * BLOCK_SIZE);
    }
}

#endif // ALLOCATOR_MEMORYPOOL_H
