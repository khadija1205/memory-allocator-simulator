#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <vector>
#include <string>
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


struct Operation {
    string type;
    int size;
    int blockId;

    // constructor for allocate operation
    Operation(string t, int s) : type(t), size(s), blockId(-1) {};

    // constructor for free operations
    Operation(string t, int id, bool isFree) : type(t), size(-1), blockId(id) {};
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

        // store recorded operations
        vector<Operation> recordedOperations;
        bool isRecording;

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

        void startRecording();

        void stopRecording();

        vector<Operation> getRecordedOperations();

        void clearRecording();

        bool getIsRecording();

        // Destructor
        ~MemoryAllocator();
};



void replayOperations(MemoryAllocator& allocator, vector<Operation>& operations, int strategy);
void compareStrategies(vector<Operation>& operations, int memorySize);

#endif