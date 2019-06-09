//
// Created by rasp on 19-6-9.
//

#ifndef ALLOCATOR_MEMORYPOOL_H
#define ALLOCATOR_MEMORYPOOL_H

#include <cstddef>

namespace MyLib {
    template<typename T>
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
        void deallocate(char* ptr, size_t n);

    private:
        static constexpr size_t BLOCK_SIZE = (sizeof(T) > 4096) ? (sizeof(T) / 4096 + 3) * 4096 : 4096;

        struct FreeNode {
            char* p;
            size_t size;
            FreeNode* next;
        };

        struct Chunk {
            char* data;
            size_t size;
            Chunk* next;
            FreeNode* freeList;
        };

        Chunk* currentBlock;

        void allocateBlock(size_t n);
    };

    template<typename T>
    MemoryPool<T>::MemoryPool() {
        currentBlock = nullptr;
    }

    template<typename T>
    MemoryPool<T>::~MemoryPool() {}

    template<typename T>
    MemoryPool<T>& MemoryPool<T>::getInstance() {
        static MemoryPool<T> instance;
        return instance;
    }

    template<typename T>
    void* MemoryPool<T>::allocate(size_t size) {
        if (currentBlock != nullptr) {
            FreeNode* p = currentBlock->freeList;
            while (p != nullptr) {
                if (p->size >= size) {
                    void* res = reinterpret_cast<void*>(p->p);
                    p->p += size;
                    p->size -= size;
                    return res;
                }
                p = p->next;
            }
        }

        int n = size / BLOCK_SIZE + 1;
        allocateBlock(n);
        void* res = reinterpret_cast<void*>(currentBlock->freeList->p);
        currentBlock->freeList->p += size;
        currentBlock->freeList->size -= size;
        return res;
    }

    template<typename T>
    void MemoryPool<T>::deallocate(char* ptr, size_t n) {
        auto newListNode = new FreeNode;
        newListNode->next = currentBlock->freeList;
        currentBlock->freeList = newListNode;
        newListNode->p = ptr;
        newListNode->size = n * sizeof(T);
    }

    template<typename T>
    void MemoryPool<T>::allocateBlock(size_t n) {
        auto newBlock = new Chunk;
        newBlock->size = n * BLOCK_SIZE / sizeof(T) * sizeof(T);
        newBlock->data = reinterpret_cast<char*>(::operator new(newBlock->size));
        newBlock->next = currentBlock;

        auto newListNode = new FreeNode;
        if (currentBlock == nullptr)
            newListNode->next = nullptr;
        else
            newListNode->next = currentBlock->freeList;
        newListNode->p = newBlock->data;
        newListNode->size = newBlock->size;
        newBlock->freeList = newListNode;

        if (currentBlock != nullptr) currentBlock->freeList = nullptr;
        currentBlock = newBlock;
    }
}

#endif // ALLOCATOR_MEMORYPOOL_H
