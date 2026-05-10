# General Concepts

## Sync, Async, Blocking, Non-blocking

Sync -> means code is excuted line by line sequentially, previous must completely end to begin next
Blocking -> a process that will take time blocking others from execution (DB, FS, OS, etc...)
Async -> happening in same time - could be in multiprocessor (parallelism/concurrency)

## Higher Order Function (HOF)

Functions that take other functions as input. Inner functions might take input as well.

### Map - new array

### Filter - new filtered array

### Reduce - redurcer or accumulator

### ForEach - returns undefined (not chainable)