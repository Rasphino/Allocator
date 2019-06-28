//
// Created by rasp on 19-6-9.
//

#ifndef ALLOCATOR_REGIONMEMORYPOOL_H
#define ALLOCATOR_REGIONMEMORYPOOL_H

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <list>

namespace MyLib {

    class RegionMemoryPool {
    public:
        RegionMemoryPool(size_t blockSize = 262144) : BLOCK_SIZE(blockSize) {}

        ~RegionMemoryPool();

        static RegionMemoryPool &getInstance();

        // no copies
        RegionMemoryPool(const RegionMemoryPool &) = delete;
        void operator=(const RegionMemoryPool &) = delete;

        // Allocate block of storage
        void *allocate(size_t size);

        // Release block of storage
        void deallocate(char *ptr, size_t n);

        void deallocateAll();

    private:
        const size_t BLOCK_SIZE;
        size_t currentBlockPos = 0, currentAllocSize = 0;
        uint8_t *currentBlock = nullptr;
        std::list<std::pair<size_t, uint8_t *>> usedBlocks, availableBlocks;
    };

    RegionMemoryPool::~RegionMemoryPool() {
        free(currentBlock);
        for (auto &block : usedBlocks)
            free(block.second);
        for (auto &block : availableBlocks)
            free(block.second);
    }

    RegionMemoryPool &RegionMemoryPool::getInstance() {
        static RegionMemoryPool instance;
        return instance;
    }

    void *RegionMemoryPool::allocate(size_t nBytes) {
        // 对齐
        const int align = alignof(std::max_align_t);
        nBytes = (nBytes + align - 1) & ~(align - 1);

        if (currentBlockPos + nBytes > currentAllocSize) {
            // 如果块存在且已满，则将其保存到usedBlocks中
            if (currentBlock) {
                usedBlocks.emplace_back(currentAllocSize, currentBlock);
                currentBlock = nullptr;
                currentAllocSize = 0;
            }

            // 从空块中寻找是否有可用的
            for (auto iter = availableBlocks.begin(); iter != availableBlocks.end(); ++iter) {
                if (iter->first >= nBytes) {
                    currentAllocSize = iter->first;
                    currentBlock = iter->second;
                    availableBlocks.erase(iter);
                    break;
                }
            }

            // 都不满足则分配新块
            if (!currentBlock) {
                currentAllocSize = std::max(nBytes, BLOCK_SIZE);
                currentBlock = reinterpret_cast<uint8_t *>(::operator new(currentAllocSize));
            }
            currentBlockPos = 0;
        }

        void *ret = currentBlock + currentBlockPos;
        currentBlockPos += nBytes;
        return ret;
    }

    void RegionMemoryPool::deallocate(char *ptr, size_t n) {}

    void RegionMemoryPool::deallocateAll() {
        currentBlockPos = 0;
        availableBlocks.splice(availableBlocks.begin(), usedBlocks);
    }

}

#endif //ALLOCATOR_REGIONMEMORYPOOL_H
