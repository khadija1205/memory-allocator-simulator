#include <iostream>
#include <limits>
#include <cstdlib> 
#include "allocator.h"

using namespace std;

// Clear screen (cross-platform)
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}
    

// Clear input buffer
void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Get valid integer input
int getIntInput(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            clearInputBuffer();
            return value;
        } else {
            cout << "\033[1;31mInvalid input! Please enter a number.\033[0m\n";
            clearInputBuffer();
        }
    }
}

// Get strategy choice
int getStrategyChoice() {
    cout << "\n\033[1;36m=== Select Strategy ===\033[0m\n";
    cout << "1. First-Fit (Fast)\n";
    cout << "2. Best-Fit (Efficient)\n";
    cout << "3. Buddy System (Low Fragmentation)\n";
    cout << "Choice: ";
    
    int choice;
    cin >> choice;
    clearInputBuffer();
    
    return choice;
}

// Display menu
void displayMenu() {
    cout << "\033[1;36m    MEMORY ALLOCATOR MENU          \033[0m\n";
    cout << "1. Allocate Memory\n";
    cout << "2. Free Memory\n";
    cout << "3. Show Memory Layout\n";
    cout << "4. Show Statistics\n";
    cout << "5. Change Strategy\n";
    cout << "6. Reset Memory\n";
    cout << "7. Exit\n";
    cout << "\n\033[1;33mChoice: \033[0m";
}

int main() {
    const int MEMORY_SIZE = 64; // KB
    MemoryAllocator allocator(MEMORY_SIZE);
    
    int currentStrategy = 1; // 1=First-Fit, 2=Best-Fit, 3=Buddy
    bool running = true;
    
    clearScreen();
    cout << "\033[1;35m";
    cout << "  MEMORY ALLOCATOR SIMULATOR         \n";
    cout << "   Interactive Memory Management         \n";
  
    cout << "\033[0m\n";
    
    cout << "Total Memory: " << MEMORY_SIZE << " KB\n";
    cout << "Current Strategy: \033[1;32mFirst-Fit\033[0m\n";
    cout << "\nPress Enter to continue...";
    cin.get();
    
    while (running) {
        clearScreen();
        
        // Show current strategy
        cout << "\033[1;36mCurrent Strategy: \033[0m";
        switch(currentStrategy) {
            case 1: cout << "\033[1;32mFirst-Fit\033[0m\n"; break;
            case 2: cout << "\033[1;32mBest-Fit\033[0m\n"; break;
            case 3: cout << "\033[1;32mBuddy System\033[0m\n"; break;
        }
        
        displayMenu();
        
        int choice;
        cin >> choice;
        clearInputBuffer();
        
        switch(choice) {
            case 1: { // Allocate
                int size = getIntInput("\nEnter size to allocate (KB): ");
                
                if (size <= 0) {
                    cout << "\033[1;31m✗ Size must be positive!\033[0m\n";
                    break;
                }
                
                int blockId = -1;
                
                switch(currentStrategy) {
                            case 1:
                                blockId = allocator.allocateFirstFit(size);
                                break;
                            case 2:
                                blockId = allocator.allocateBestFit(size);
                                break;
                            case 3:
                                blockId = allocator.allocateBuddySystem(size);
                                break;
                        }
                                        
                if (blockId != -1) {
                    cout << "\n";
                    allocator.show();
                } else {
                    cout << "\n\033[1;31m✗ Allocation failed! Not enough memory.\033[0m\n";
                }
                break;
            }
            
            case 2: { // Free
                allocator.show();
                cout << "\n";
                
                int blockId = getIntInput("Enter block ID to free: ");
                
                if (allocator.free(blockId)) {
                    cout << "\n";
                    allocator.show();
                } else {
                    cout << "\n\033[1;31m✗ Block not found!\033[0m\n";
                }
                break;
            }
            
            case 3: { // Show Memory
                allocator.show();
                break;
            }
            
            case 4: { // Show Statistics
                allocator.showStats();
                break;
            }
            
            case 5: { // Change Strategy
                currentStrategy = getStrategyChoice();
                if (currentStrategy < 1 || currentStrategy > 3) {
                    currentStrategy = 1;
                }
                cout << "\n\033[1;32m Strategy changed!\033[0m\n";
                break;
            }
            
            case 6: { // Reset
                cout << "\n\033[1;33mAre you sure you want to reset? (y/n): \033[0m";
                char confirm;
                cin >> confirm;
                clearInputBuffer();
                
                if (confirm == 'y' || confirm == 'Y') {
                    // Recreate allocator
                    allocator = MemoryAllocator(MEMORY_SIZE);
                    cout << "\n\033[1;32m✓ Memory reset!\033[0m\n";
                }
                break;
            }
            
            case 7: { // Exit
                cout << "\n\033[1;35m Thanks for using Memory Allocator!\033[0m\n";
                running = false;
                break;
            }
            
            default:
                cout << "\n\033[1;31m✗ Invalid choice!\033[0m\n";
        }
        
        if (running) {
            cout << "\n\033[1;33mPress Enter to continue...\033[0m";
            cin.get();
        }
    }
    
    return 0;
}