//
// Created by rasp on 19-6-28.
//

#ifndef ALLOCATOR_FREELISTMEMORYPOOL_H
#define ALLOCATOR_FREELISTMEMORYPOOL_H

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <list>
#include <set>

namespace MyLib {

    struct rub {
        void *p;
        size_t sz;

        rub(void *t, size_t size) : p(t), sz(size) {}

        bool operator<(const rub &rhs) const {
            if (sz != rhs.sz) return sz < rhs.sz;
            else return p < rhs.p;
        }
    };

    class FreeListMemoryPool {
    public:
        FreeListMemoryPool();
        ~FreeListMemoryPool();

        static FreeListMemoryPool &getInstance();

        // no copies
        FreeListMemoryPool(const FreeListMemoryPool &) = delete;
        void operator=(const FreeListMemoryPool &) = delete;

        // Allocate block of storage
        void *allocate(size_t size);

        // Release block of storage
        void deallocate(char *ptr, size_t n, size_t size);

    private:
        static constexpr size_t BLOCK_SIZE = 262144;
        std::set<rub> myset;

        std::list<void *> usedBlock;
        char *currPos = nullptr, *lastPos = nullptr;

        void allocateBlock(size_t n);
    };

    FreeListMemoryPool::FreeListMemoryPool() {}

    FreeListMemoryPool::~FreeListMemoryPool() {
        for (const auto &block : usedBlock) {
            free(block);
        }
    }

    FreeListMemoryPool &FreeListMemoryPool::getInstance() {
        static FreeListMemoryPool instance;
        return instance;
    }

    void *FreeListMemoryPool::allocate(size_t size) {
        std::set<rub>::iterator pos;
        pos = myset.lower_bound(rub((void *) 0, size));//get the best pos in the set
        if (pos == myset.end()) {//the largest rubbish still doesn't work
            if (currPos + size > lastPos) {
                int n = size / BLOCK_SIZE + 1;
                allocateBlock(n);
            }
            currPos += size;
            return currPos - size;
        } else {
            rub tmp = *pos;
            myset.erase(pos);
            if (tmp.sz > size)
                myset.insert({reinterpret_cast<char *>(tmp.p) + size, tmp.sz - size});
            return tmp.p;
        }
    }

    void FreeListMemoryPool::deallocate(char *ptr, size_t n, size_t size) {
        size_t res = size * n;
        myset.insert({ptr, res});
    }

    void FreeListMemoryPool::allocateBlock(size_t n) {
        size_t res = lastPos - currPos;
        currPos = reinterpret_cast<char *>(::operator new(n * BLOCK_SIZE));
        lastPos = currPos + n * BLOCK_SIZE;
        usedBlock.push_back(currPos);
    }
}

#endif //ALLOCATOR_FREELISTMEMORYPOOL_H
