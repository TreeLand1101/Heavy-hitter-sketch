CPU Code
============

Repository structure
--------------------
*  `Common/`: the hash and mmap functions
*  `Struct/`: the data structures, such as heap and hash table
*  `Src/`: sketch algorithms
*  `Benchmark.h`: the benchmarks about ARE, recall rate, and precision rate

Requirements
-------
- cmake
- g++

How to run
-------
```bash
$ cmake .
$ make
$ ./CPU memory threshold datset