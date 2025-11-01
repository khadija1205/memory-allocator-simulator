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

    MemoryBlock initialBlock;
    initialBlock.id = 0;
    initialBlock.start = 0;
    initialBlock.size = size;
    initialBlock.isFree = true;

    blocks.push_back(initialBlock);
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




// Allocate function 
int MemoryAllocator::allocate(int size) {

    return allocateFirstFit(size);
    
}




int MemoryAllocator::allocateFirstFit(int size) {

    totalAllocations++;

    // Find first free block that fits
    for (int i = 0; i < blocks.size(); i++) {
        if (blocks[i].isFree && blocks[i].size >= size) {
            
            int blockId = nextId++;
            
            // spliting
            if (blocks[i].size > size) {
                int remainingSize = blocks[i].size - size;
                int remainingStart = blocks[i].start + size;
                
                MemoryBlock leftover;
                leftover.id = 0;
                leftover.start = remainingStart;
                leftover.size = remainingSize;
                leftover.isFree = true;
                
                blocks.insert(blocks.begin() + i + 1, leftover);
                blocks[i].size = size;

                
            }
            
            blocks[i].isFree = false;
            blocks[i].id = blockId;

            successfulAllocations++;
            
            cout << "Allocated Block #" << blockId << " (" << size << " KB)" << endl;
            return blockId;
        }
    }
    
    failedAllocations++;
    cout << " Cannot allocate " << size << " KB!" << endl;
    return -1;


    
}



int MemoryAllocator :: allocateBestFit(int size){
    totalAllocations++;

    // Finding smallest block that fits
    int bestIndex = -1;
    int smallestSize = -1;


    // checking all the blocks
    for (int i = 0; i < blocks.size();i++){
        if (blocks[i].isFree && blocks[i].size >= size){
            // first fit found
            if(bestIndex == -1){
                bestIndex = i;
                smallestSize = blocks[i].size;
            }

            // if better one
            else if(blocks[i].size < smallestSize){
                bestIndex = i;
                smallestSize = blocks[i].size;
            }
        }
    }


    if(bestIndex == -1){
        failedAllocations++;
        cout << "Cannot Allocate" << size << " KB! " << endl;
        return -1;
    }

    // use the best block
    int i = bestIndex;
    int blockId = nextId++;


    // split the block
    if(blocks[i].size > size){
        int remainingSize = blocks[i].size - size;
        int remainingStart = blocks[i].start + size;


        MemoryBlock leftover;
        leftover.id = 0;
        leftover.start = remainingStart;
        leftover.size = remainingSize;
        leftover.isFree = true;
        
        blocks.insert(blocks.begin() + i + 1, leftover);
        blocks[i].size = size;
    }


    blocks[i].isFree = false;
    blocks[i].id = blockId;
    
    successfulAllocations++;
    cout << "Allocated Block #" << blockId << " (" << size << " KB) [BEST-FIT]" << endl;
    return blockId;
}




int MemoryAllocator::allocateBuddySystem(int size){
    totalAllocations++;

    int requiredSize = nextPowerOf2(size);

    int bestIndex = -1;
    int smallestSize = -1;

    for (int i = 0; i < blocks.size(); i++) {
        if (blocks[i].isFree && blocks[i].size >= requiredSize) {
            // Check if block size is power of 2
            if ((blocks[i].size & (blocks[i].size - 1)) == 0) {
                if (bestIndex == -1 || blocks[i].size < smallestSize) {
                    bestIndex = i;
                    smallestSize = blocks[i].size;
                }
            }
        }
    }

    // No suitable block found
    if (bestIndex == -1) {
        failedAllocations++;
        cout << "Cannot allocate " << size << " KB with Buddy System" << endl;
        return -1;
    }


     // Split block until we get exactly requiredSize
    int i = bestIndex;
    while (blocks[i].size > requiredSize) {
        int halfSize = blocks[i].size / 2;
        
        // Split: [Original] → [Left half][Right half]
        MemoryBlock rightBuddy;
        rightBuddy.id = 0;
        rightBuddy.start = blocks[i].start + halfSize;
        rightBuddy.size = halfSize;
        rightBuddy.isFree = true;
        
        // Update left half
        blocks[i].size = halfSize;
        
        // Insert right buddy
        blocks.insert(blocks.begin() + i + 1, rightBuddy);
        
        // Continue with left half
    }
    
    // Allocate the block
    int blockId = nextId++;
    blocks[i].isFree = false;
    blocks[i].id = blockId;
    
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