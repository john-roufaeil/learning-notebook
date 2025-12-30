# Linked lists vs arrays in search and sort

## Sort

### Bubble Sort
- Arrays in O(n^2) time, simple element swapping
- Linked Lists in O(n^2) time, complex pointer swapping
- Works with linked lists

### Selection Sort
- Arrays in O(n^2) time, simple element swapping
- Linked Lists in O(n^2) time, complex pointer swapping
- Works with linked lists but complicated

### Insertion Sort
- Arrays in O(n^2) time, requires shifting elements
- Linked Lists in O(n^2) time, no shifting 
- Works with linked lists 


## Search

### Linear Search
- Arrays in O(n) time, simple index-based traversal
- Linked Lists in O(n) time, traverse with one pointer from head, equally efficient
- Works well with linked lists

### Binary Search
- Arrays in O(log n) time, works efficiently with direct middle access
- Linked Lists in O(n log n) time, finding middle costs O(n) each time, inefficient
- Bad for linked lists but can be done

## Take aways

- Linear search suits linked lists
- Binary search is not efficient in linked lists, which is a technique for faster search actually performs worse than linear
- Bubble and selection sort do too many pointer swaps with complex implementation
- Random access is always O(n) instead of O(1) in linked lists
- A simple workaround is to convert to array, sort, then rebuild linked list
- Arrays are better for searching and most sorting due to random access