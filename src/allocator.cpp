#include "allocator.h"
#include <iostream>
#include <iomanip>
using namespace std;


MemoryAllocator::MemoryAllocator(int size){
    
    totalsize = size;
    nextId = 1;

    // Initialize statistics to zero
    totalAllocations = 0;
    successfulAllocations = 0;
    failedAllocations = 0;

    freeListHead = nullptr;

    blocks.clear();

  

    MemoryBlock initialBlock;
    initialBlock.id = 0;
    initialBlock.start = 0;
    initialBlock.size = size;
    initialBlock.isFree = true;

    blocks.push_back(initialBlock);

    

   

    rebuildFreeList();

   
}

MemoryAllocator::~MemoryAllocator(){
    FreeListNode *current = freeListHead;
    while(current != nullptr){
        FreeListNode *temp = current;
        current = current->next;
        delete temp;
    }
}



// helper function - calculating next power of 2
int MemoryAllocator::nextPowerOf2(int n){
    if (n <= 0)
        return 1;


    if((n& (n-1)) == 0){
        return n;
    }

    int power = 1;
    while(power < n){
        power *= 2;
    }

    return power;
}



void MemoryAllocator::rebuildFreeList(){

    

    FreeListNode *current = freeListHead;
    while(current != nullptr){
        FreeListNode *temp = current;
        current = current->next;
        delete temp;
    }



    freeListHead = nullptr;

    

    FreeListNode *tail = nullptr;
    




    for (int i = 0; i < blocks.size(); i++){
        if(blocks[i].isFree){

 
            FreeListNode *newNode = new FreeListNode;
            newNode->block = &blocks[i];
            newNode->next = nullptr;


            if(freeListHead == nullptr){
                freeListHead = newNode;
                tail = newNode;
            } else {
                tail->next = newNode;
                tail = newNode;
            }
        }
    }

}



void MemoryAllocator::addToFreeList(MemoryBlock* block){
    FreeListNode *newNode = new FreeListNode;
    newNode->block = block;
    newNode->next = nullptr;

    newNode->next = freeListHead;
    freeListHead = newNode;
}



void MemoryAllocator::removeFromFreeList(MemoryBlock* block){
    if(freeListHead == nullptr){
        return;
    }


    // the block is at the head
    if(freeListHead->block == block){
        FreeListNode *temp = freeListHead;
        freeListHead = freeListHead->next;
        delete temp;
        return;
    }



    // the block is somewhere in the middle or end
    FreeListNode *current = freeListHead;
    while(current->next != nullptr){
        if(current->next->block == block){
            FreeListNode *temp = current->next;
            current->next = current->next->next;
            delete temp;
            return;
        }
        current = current->next;
    }
}





// Allocate function 
int MemoryAllocator::allocate(int size) {

    return allocateFirstFit(size);
    
}




int MemoryAllocator::allocateFirstFit(int size) {

    totalAllocations++;

    FreeListNode* current = freeListHead;

    while(current != nullptr){
        MemoryBlock *block = current->block;

        if(block->size >= size){
            int blockId = nextId++;


            if(block->size > size){
                int blockIndex = -1;
                for (int i = 0; i < blocks.size();i++){
                    if(&blocks[i] == block){
                        blockIndex = i;
                        break;
                    }
                }


                // SAVE ORIGINAL SIZE BEFORE MODIFYING!
                int originalSize = blocks[blockIndex].size;
                int originalStart = blocks[blockIndex].start;

                blocks[blockIndex].isFree = false;
                blocks[blockIndex].id = blockId;
                blocks[blockIndex].size = size;


                int remainingSize = originalSize - size;
                int remainingStart = originalStart + size;

                MemoryBlock leftover;
                leftover.id = 0;
                leftover.start = remainingStart;
                leftover.size = remainingSize;
                leftover.isFree = true;

                // insert leftover into vector
                blocks.insert(blocks.begin() + blockIndex + 1, leftover);

                rebuildFreeList();
            } else{
                removeFromFreeList(block); 
                block->isFree = false;
                block->id = blockId; 
            }

           

            successfulAllocations++;
             cout << "Allocated Block #" << blockId << " (" << size << " KB)" << endl;
            return blockId;
        }

        // Move to next free block
        current = current->next;
    }
        
    // No suitable block found
    failedAllocations++;
    cout << "Cannot allocate " << size << " KB!" << endl;
    return -1;


    
}



int MemoryAllocator :: allocateBestFit(int size){
    totalAllocations++;

    FreeListNode *current = freeListHead;
    FreeListNode *bestNode = nullptr;
    int smallestSize = -1;

    while(current != nullptr){
        MemoryBlock *block = current->block;

        if(block->size >= size){
            if(bestNode == nullptr || block->size < smallestSize){
                bestNode = current;
                smallestSize = block->size;
            }
        }

        current = current->next;
    }


    // no suitable blocks found
    if(bestNode == nullptr){
        failedAllocations++;
        cout << "Cannot Allocate" << size << "KB!" << endl;
        return -1;
    }

    MemoryBlock *block = bestNode->block;
    int blockId = nextId++;


    if(block->size > size){
        int blockIndex = -1;
        for (int i = 0; i < blocks.size();i++){
            if(&blocks[i] == block){
                blockIndex = i;
                break;
            }
        }


        // Save original values
        int originalSize = blocks[blockIndex].size;
        int originalStart = blocks[blockIndex].start;

        // Mark as allocated
        blocks[blockIndex].isFree = false;
        blocks[blockIndex].id = blockId;
        blocks[blockIndex].size = size;


        // Create leftover block
        int remainingSize = originalSize - size;
        int remainingStart = originalStart + size;

        MemoryBlock leftover;
        leftover.id = 0;
        leftover.start = remainingStart;
        leftover.size = remainingSize;
        leftover.isFree = true;


         // Insert leftover into vector
        blocks.insert(blocks.begin() + blockIndex + 1, leftover);

        // Rebuild free list
        rebuildFreeList();

    } else{
        removeFromFreeList(block);
        block->isFree = false;
        block->id = blockId;
    }

    successfulAllocations++;
    cout << "Allocated Block #" << blockId << " (" << size << " KB) [BEST-FIT]" << endl;
    return blockId;
}




int MemoryAllocator::allocateBuddySystem(int size){
    totalAllocations++;

    int requiredSize = nextPowerOf2(size);

    FreeListNode *current = freeListHead;
    FreeListNode *bestNode = nullptr;

    int smallestSize = -1;

    while(current != nullptr){
        MemoryBlock *block = current->block;

        if(block->size >= requiredSize && (block->size &(block->size -1)) == 0){
            if(bestNode == nullptr || block->size < smallestSize){
                bestNode = current;
                smallestSize = block->size;
            }
        }

        current = current->next;
    }

    // No suitable block found
    if (bestNode == nullptr) {
        failedAllocations++;
        cout << "Cannot allocate " << size << " KB with Buddy System" << endl;
        return -1;
    }


     
    
   // Use the best block
    MemoryBlock* block = bestNode->block;

    // Find block index in vector
    int blockIndex = -1;
    for (int i = 0; i < blocks.size(); i++) {
        if (&blocks[i] == block) {
            blockIndex = i;
            break;
        }
    }

    // Split block until we get exactly requiredSize
    while (blocks[blockIndex].size > requiredSize) {
        int halfSize = blocks[blockIndex].size / 2;
        
        // Save values before modifying
        int currentStart = blocks[blockIndex].start;
        
        // Update left half
        blocks[blockIndex].size = halfSize;
        
        // Create right buddy
        MemoryBlock rightBuddy;
        rightBuddy.id = 0;
        rightBuddy.start = currentStart + halfSize;
        rightBuddy.size = halfSize;
        rightBuddy.isFree = true;
        
        // Insert right buddy
        blocks.insert(blocks.begin() + blockIndex + 1, rightBuddy);
        
        
    }


    // Allocate the block
    int blockId = nextId++;
    blocks[blockIndex].isFree = false;
    blocks[blockIndex].id = blockId;
    
    // Rebuild free list after all splits
    rebuildFreeList();

    
    successfulAllocations++;
    cout << "Allocated Block #" << blockId << " (" << requiredSize 
         << " KB for " << size << " KB request) [BUDDY]" << endl;
    
    return blockId;
}



// Free function 
bool MemoryAllocator::free(int blockId) {
    for (int i = 0; i < blocks.size(); i++) {
        if (!blocks[i].isFree && blocks[i].id == blockId) {
            blocks[i].isFree = true;
            blocks[i].id = 0;
            
            cout << "Freed Block #" << blockId << endl;
            
            // Merge adjacent free blocks
            for (int j = 0; j < blocks.size() - 1; j++) {
                if (blocks[j].isFree && blocks[j+1].isFree) {
                    blocks[j].size += blocks[j+1].size;
                    blocks.erase(blocks.begin() + j + 1);
                    j--;
                }
            }

            rebuildFreeList();

            return true;
        }
    }
    
    cout << "Block #" << blockId << " not found!" << endl;
    return false;
}


// Show memory state
void MemoryAllocator::show() {
    cout << "\nMemory Layout:" << endl;
    
    // Show each block with details
    for (int i = 0; i < blocks.size(); i++) {
        if (blocks[i].isFree) {
            cout << "[FREE " << blocks[i].size << "KB]";
        } else {
            cout << "[BLOCK#" << blocks[i].id << " " << blocks[i].size << "KB]";
        }
    }
    
    cout << "\n\nBlock Details:" << endl;
    int usedMemory = 0;
    int freeMemory = 0;
    
    for (int i = 0; i < blocks.size(); i++) {
        if (blocks[i].isFree) {
            cout << "  Position " << blocks[i].start << ": FREE " 
                 << blocks[i].size << " KB" << endl;
            freeMemory += blocks[i].size;
        } else {
            cout << "  Position " << blocks[i].start << ": BLOCK #" 
                 << blocks[i].id << " (" << blocks[i].size << " KB)" << endl;
            usedMemory += blocks[i].size;
        }
    }
    
    cout << "\nTotal: " << totalsize << " KB (Used: " << usedMemory 
         << " KB, Free: " << freeMemory << " KB)" << endl;
}



int MemoryAllocator::getUsedMemory(){
    int used = 0;
    for (int i = 0; i < blocks.size(); i++){
        if(!blocks[i].isFree){
            used += blocks[i].size;
        }
    }

    return used;
}


int MemoryAllocator::getFreeMemory(){
    return totalsize - getUsedMemory();
}


int MemoryAllocator::getLargestFreeBlock(){
    int largest = 0;
    for (int i = 0; i < blocks.size(); i++){
        if (blocks[i].isFree && blocks[i].size > largest){
            largest = blocks[i].size;
        }
    }

    return largest;
}



int MemoryAllocator::getFreeBlockCount(){
    int count = 0;
    for (int i = 0; i < blocks.size();i++){
        if(blocks[i].isFree){
            count++;
        }
    }

    return count;
}


double MemoryAllocator::getFragmentationPercent() {
    int totalFree = getFreeMemory();
    
    // No free memory = no fragmentation
    if (totalFree == 0) return 0.0;
    
    int largestFree = getLargestFreeBlock();
    
    //How much of free memory is "wasted"
    double fragmentation = (1.0 - (double)largestFree / totalFree) * 100.0;
    
    return fragmentation;
}


void MemoryAllocator::showStats() {
    cout << "\n==========================" << endl;
    cout << "      Memory STATISTICS " << endl;
    cout << "\n==========================" << endl;

    int used = getUsedMemory();
    int free = getFreeMemory();
    double usedPercent = (used * 100.0) / totalsize;
    double freePercent = (free * 100.0) / totalsize;


    cout << "Total Memory:        " << totalsize << " KB" << endl;
    cout << "Used Memory:         " << used << " KB (" << usedPercent << "%)" << endl;
    cout << "Free Memory:         " << free << " KB (" << freePercent << "%)" << endl;

    int freeBlocks = getFreeBlockCount();
    double fragPercent = getFragmentationPercent();
    cout << "Free Blocks:         " << freeBlocks;
    cout << "Fragmentation:       " << fixed << setprecision(1) << fragPercent << "%";

    if (fragPercent > 50.0) {
    cout << " [HIGH]";
} else if (fragPercent > 20.0) {
    cout << " [MEDIUM]";
} else {
    cout << " [LOW]";
}
cout << endl;
}