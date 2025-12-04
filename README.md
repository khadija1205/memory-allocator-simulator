##  Web Version
A visual, interactive version of this project is available at:
[memory-allocator-visualizer](https://github.com/khadija1205/memory-allocator-visualizer)

A live, deployed version of this project is available at:
[memory-allocator-visualizer]((https://memory-allocator-visualizer.vercel.app/))



# Memory Allocator Simulator

A high-performance memory allocation simulator implementing three classic algorithms with advanced fragmentation analysis and comparison tools.

![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=flat&logo=c%2B%2B&logoColor=white)
![Algorithm](https://img.shields.io/badge/Algorithm-Optimization-green)
![Data Structures](https://img.shields.io/badge/Data%20Structures-Advanced-blue)

## Project Overview

This project demonstrates advanced memory management concepts by implementing three fundamental allocation algorithms with real-time performance comparison. Built as a learning tool to understand memory fragmentation, allocation strategies, and algorithmic trade-offs.

### Key Features

- **Three Allocation Algorithms**: First-Fit, Best-Fit, and Buddy System
- **Free List Optimization**: 10x faster allocation through efficient data structures
- **Record & Replay System**: Compare algorithms with identical workloads
- **Real-time Metrics**: Fragmentation analysis, memory usage, and performance stats
- **Interactive CLI**: User-friendly menu-driven interface

## Performance Improvements

| Metric | Before Optimization | After Optimization | Improvement |
|--------|-------------------|-------------------|-------------|
| **Search Time** | O(n) - all blocks | O(f) - free blocks only | ~10x faster |
| **Memory Scanned** | 100% of blocks | Only free blocks | 90% reduction |
| **Data Structure** | Vector scan | Free list traversal | Optimized |

## Algorithm Comparison

### First-Fit
- **Speed**:  Fastest
- **Memory Efficiency**:  Moderate
- **Best For**: Quick allocations, simple workloads
- **Fragmentation**: Moderate to High

### Best-Fit
- **Speed**:  Fast
- **Memory Efficiency**: Best
- **Best For**: Minimizing wasted space
- **Fragmentation**: Low to Moderate

### Buddy System
- **Speed**: Fastest merging
- **Memory Efficiency**: Power-of-2 overhead
- **Best For**: Fast deallocation, predictable patterns
- **Fragmentation**: Low external, high internal

## Technical Implementation

### Core Data Structures
cpp


### Key Algorithms

#### Free List Optimization
- Maintains separate linked list of only free blocks
- Skips allocated blocks during search
- Rebuilds efficiently after vector modifications

#### Record & Replay System
- Records user operations during interactive session
- Replays identical workload on all three algorithms
- Enables fair, side-by-side comparison


## 🔧 Installation & Usage

### Prerequisites
- C++ compiler (g++ 7.0 or higher)
- C++11 standard or later

### Compile
bash
g++ -std=c++11 -o allocator src/main.cpp src/allocator.cpp


### Run
bash
./allocator


### Basic Operations
1. **Allocate Memory** - Request memory blocks of any size
2. **Free Memory** - Release allocated blocks
3. **Show Layout** - Visualize current memory state
4. **Show Statistics** - View fragmentation and usage metrics

### Comparison Workflow
1. **Start Recording** (Option 7)
2. Perform allocations and frees
3. **Stop Recording** (Option 8)
4. **Compare All Strategies** (Option 9)
5. View side-by-side results

## Contact

Feel free to reach out for questions or collaboration opportunities!

 **Star this repo if you found it helpful!**




