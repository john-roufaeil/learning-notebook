# Selection Sort

Selection sort is a simple sorting algorithm that works by repeatedly finding the minimum element from the unsorted part of the array and putting it at the beginning.

## Key Concepts

- modifies the array directly without extra memory
- divides the array into two parts:
  - sorted subarray on the left, built up
    from the beginning.
  - unsorted subarray on the right, containing the remaining elements.
- iteratively proceeds through the array. In each pass, it finds the smallest element in the unsorted part and swaps it with the first element of the unsorted part.

## The Algorithm

- Outer Loop: Iterate through the array from the first element to the second-to-last element. This loop represents the boundary between the sorted and unsorted portions.

- Inner Loop: For each element in the outer loop, start an inner loop to find the minimum element in the remaining unsorted part of the array.

- Swap: After the inner loop completes, you will have the index of the minimum element. Swap this minimum element with the element at the current position of the outer loop.

- Repeat: The process repeats for the next element in the outer loop, expanding the sorted portion by one element each time until the entire array is sorted.

## Example Walkthrough

Let's sort the array [64, 25, 12, 22, 11].

### Pass 1

Find the minimum element in [64, 25, 12, 22, 11]. The minimum is 11.

Swap 11 with 64. The array becomes [11, 25, 12, 22, 64]. The sorted part is now [11].

### Pass 2

Find the minimum element in the remaining unsorted part [25, 12, 22, 64]. The minimum is 12.

Swap 12 with 25. The array becomes [11, 12, 25, 22, 64]. The sorted part is now [11, 12].

### Pass 3

Find the minimum element in the remaining unsorted part [25, 22, 64]. The minimum is 22.

Swap 22 with 25. The array becomes [11, 12, 22, 25, 64]. The sorted part is now [11, 12, 22].

### Pass 4

Find the minimum element in the remaining unsorted part [25, 64]. The minimum is 25.

Swap 25 with 25 (no change). The array becomes [11, 12, 22, 25, 64]. The sorted part is now [11, 12, 22, 25].

The array is now sorted.

## Example Code (Python)

```python
def selection_sort(arr):
    """
    Sorts a list using the selection sort algorithm.

    Args:
        arr: The unsorted list of numbers to sort.
    """

    n = len(arr)

    # Traverse through each array element
    for i in range(n):
        # Find the minimum element in the remaining unsorted array (Compare each element to every other element)
        min_idx = i
        for j in range(i + 1, n):
            if arr[j] < arr[min_idx]:
                min_idx = j

        # Swap the found minimum element with the first element of the unsorted part
        arr[i], arr[min_idx] = arr[min_idx], arr[i]

# Example usage
my_list = [64, 25, 12, 22, 11]
selection_sort(my_list)
print("Sorted array is:", my_list)
```
