#include <iostream>
#include "allocator.h"
using namespace std;



// Comparison test function
// void compareStrategies() {
//     cout << "\n╔════════════════════════════════════════╗" << endl;
//     cout << "║  FIRST-FIT vs BEST-FIT COMPARISON    ║" << endl;
//     cout << "╚════════════════════════════════════════╝\n" << endl;

//     // Test 1: First-Fit
//     cout << "═══ TEST 1: FIRST-FIT STRATEGY ═══\n" << endl;
//     MemoryAllocator allocator1(64);

//     allocator1.allocateFirstFit(10);
//     allocator1.allocateFirstFit(30);
//     allocator1.allocateFirstFit(10);
//     allocator1.show();
    
//     cout << "\n--- Free middle block (30 KB) ---" << endl;
//     allocator1.free(2);
//     allocator1.show();
    
//     cout << "\n--- Try to allocate 8 KB with First-Fit ---" << endl;
//     allocator1.allocateFirstFit(8);
//     allocator1.show();
//     allocator1.showStats();


//     // Test 2: Best-Fit
//     cout << "\n\n═══ TEST 2: BEST-FIT STRATEGY ═══\n" << endl;
//     MemoryAllocator allocator2(64);
    
//     allocator2.allocateBestFit(10);
//     allocator2.allocateBestFit(30);
//     allocator2.allocateBestFit(10);
//     allocator2.show();
    
//     cout << "\n--- Free middle block (30 KB) ---" << endl;
//     allocator2.free(2);
//     allocator2.show();
    
//     cout << "\n--- Try to allocate 8 KB with Best-Fit ---" << endl;
//     allocator2.allocateBestFit(8);
//     allocator2.show();
//     allocator2.showStats();
    
//     cout << "\n╔════════════════════════════════════════╗" << endl;
//     cout << "║  COMPARISON COMPLETE!                 ║" << endl;
//     cout << "║  Notice: Both achieve same result     ║" << endl;
//     cout << "║  for this scenario!                   ║" << endl;
//     cout << "╚════════════════════════════════════════╝\n" << endl;


// }


void testBuddySystem() {
    cout << "\n╔════════════════════════════════════════╗" << endl;
    cout << "║      BUDDY SYSTEM TEST                ║" << endl;
    cout << "╚════════════════════════════════════════╝\n" << endl;
    
    MemoryAllocator allocator(64);
    
    cout << "--- Initial State ---" << endl;
    allocator.show();
    
    cout << "\n--- Allocate 10 KB (will use 16 KB) ---" << endl;
    allocator.allocateBuddySystem(10);
    allocator.show();
    
    cout << "\n--- Allocate 5 KB (will use 8 KB) ---" << endl;
    allocator.allocateBuddySystem(5);
    allocator.show();
    
    cout << "\n--- Allocate 20 KB (will use 32 KB) ---" << endl;
    allocator.allocateBuddySystem(20);
    allocator.show();
    
    cout << "\n--- Free Block #2 (8 KB) ---" << endl;
    allocator.free(2);
    allocator.show();
    
    cout << "\n--- Statistics ---" << endl;
    allocator.showStats();
    
    cout << "\n╔════════════════════════════════════════╗" << endl;
    cout << "║  Notice: Buddy System rounds up to    ║" << endl;
    cout << "║  powers of 2, wasting some space!     ║" << endl;
    cout << "╚════════════════════════════════════════╝\n" << endl;
}


int main(){

    // compareStrategies();

    testBuddySystem(); 

    // cout << "=== Memory Allocator Starting ====" << endl;

    // MemoryAllocator allocator(64);

    // cout << "\n--- Initial State ---" << endl;
    // allocator.show();
    // allocator.showStats();

    // cout << "\n--- Allocating Blocks ---" << endl;
    // int id1 = allocator.allocate(8);
    // int id2 = allocator.allocate(16);
    // int id3 = allocator.allocate(8);
    // allocator.show();
    // allocator.showStats();

    // // Free the middle block
    // cout << "\n--- Freeing Block #" << id2 << " ---" << endl;
    // allocator.free(id2);
    // allocator.show();
    // allocator.showStats();

    // // Try to allocate a large block
    // cout << "\n--- Trying to Allocate 20 KB ---" << endl;
    // allocator.allocate(20);
    // allocator.show();
    // allocator.showStats();

    // cout << "\n=== Demo Complete ===" << endl;

    return 0;
}
