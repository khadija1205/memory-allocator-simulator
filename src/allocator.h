#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <vector>
using namespace std;

struct MemoryBlock {
    int id;
    int start;
    int size;
    bool isFree;
};

struct FreeListNode {
    MemoryBlock *block;
    FreeListNode *next;
};



class MemoryAllocator {
    private:
        int totalsize;
        int nextId;
        vector<MemoryBlock> blocks;

        // points to the first free block
        FreeListNode *freeListHead;

        // statistics tracking
        int totalAllocations;
        int successfulAllocations;
        int failedAllocations;


        // helper function for buddy system
        int nextPowerOf2(int n);

        void addToFreeList(MemoryBlock *block);

        void removeFromFreeList(MemoryBlock *block);

        void rebuildFreeList();

    public:

        // constructor - initializing memory
        MemoryAllocator(int size);

        int allocate(int size);

        int allocateFirstFit(int size);

        int allocateBestFit(int size);

        int allocateBuddySystem(int size);

        bool free(int blockId);

        void show();

        int getUsedMemory();

        int getFreeMemory();

        int getLargestFreeBlock();

        int getFreeBlockCount();

        double getFragmentationPercent();

        void showStats();

        // Destructor to clean up free list
        ~MemoryAllocator();
};

#endif