# Binary Search

Binary search is an efficient algorithm for finding an item from a **sorted** list of items.

It works by repeatedly dividing the search interval in half.

## Key Concepts

Sorted Data: Binary search requires the data to be sorted in either ascending or descending order. It cannot work on unsorted data.

Divide and Conquer: The algorithm's core principle is to repeatedly cut the search space in half.

Pointers: The algorithm typically uses three pointers:

- low: Points to the beginning of the search interval.
- high: Points to the end of the search interval.
- mid: Points to the middle of the search interval.

The Algorithm
Start: Set the low pointer to the first element and the high pointer to the last element of the array.

Loop: While low is less than or equal to high, repeat the following steps:

Calculate the mid index: mid = floor((low + high) / 2).

Check: Compare the element at mid with the target value.

If array[mid] is equal to the target, you've found the element. Return the mid index.

If array[mid] is less than the target, the target must be in the upper half of the remaining array. Move the low pointer to mid + 1.

If array[mid] is greater than the target, the target must be in the lower half of the remaining array. Move the high pointer to mid - 1.

End: If the loop finishes and the element hasn't been found, it doesn't exist in the array. Return a value like -1 to indicate this.

Example Walkthrough
Let's find the number 15 in the sorted array [2, 5, 8, 12, 15, 20, 25].

low = 0, high = 6.

mid = floor((0 + 6) / 2) = 3. array[3] is 12. 12 < 15, so we update low.

low = 4, high = 6.

mid = floor((4 + 6) / 2) = 5. array[5] is 20. 20 > 15, so we update high.

low = 4, high = 4.

mid = floor((4 + 4) / 2) = 4. array[4] is 15. 15 == 15, we found it!

Example Code (Python)
def binary_search(arr, target):
    """
    Performs a binary search on a sorted list.

    Args:
        arr: The sorted list of numbers.
        target: The number to search for.

    Returns:
        The index of the target if found, otherwise -1.
    """
    low = 0
    high = len(arr) - 1

    while low <= high:
        mid = (low + high) // 2  # Using integer division

        if arr[mid] == target:
            return mid
        elif arr[mid] < target:
            low = mid + 1
        else:
            high = mid - 1

    return -1

## Example usage

my_list = [2, 5, 8, 12, 15, 20, 25]
target_value = 15

result = binary_search(my_list, target_value)

if result != -1:
    print(f"Element found at index: {result}")
else:
    print("Element not found in the list.")
