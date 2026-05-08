import java.util.Random;

class Search {
    public static void main(String[] args) {
        Random rand = new Random();
        int numbers[] = new int[100000];
        for (int i = 0; i < numbers.length; i++) {
            numbers[i] = rand.nextInt(1000);
        }
        System.out.println("Minimum is: " + findMin(numbers));
        System.out.println("Maximum is: " + findMax(numbers));
        System.out.println("Searching for 5: " + binarySearch(numbers, 5));
        System.out.println("Searching for -124: " + binarySearch(numbers, numbers[0]));
    }

    static int findMin(int[] arr) {
        long startTime = System.currentTimeMillis();
        int min = arr[0];
        for (int i = 0; i < arr.length; i++) {
            if (arr[i] < min) {
                min = arr[i];
            }
        }
        long endTime = System.currentTimeMillis();
        System.out.println("Took " + (endTime-startTime) + "milliseconds");
        return min;
    }

    static int findMax(int[] arr) {
        long startTime = System.currentTimeMillis();
        int max = arr[0];
        for (int i = 0; i < arr.length; i++) {
            if (arr[i] > max) {
                max = arr[i];
            }
        }
        long endTime = System.currentTimeMillis();
        System.out.println("Took " + (endTime-startTime) + "milliseconds");
        return max;
    }

    static int binarySearch(int[] arr, int target) {
        long startTime = System.currentTimeMillis();
        int left = 0;
        int right = arr.length - 1;
        while (left <= right) {
            int mid = (left + right) / 2;
            if (target < mid) {
                right = mid - 1;
            } else if (target > mid) {
                left = mid + 1;
            } else {
                long endTime = System.currentTimeMillis();
                System.out.println("Took " + (endTime-startTime) + "milliseconds");
                return mid;
            }
        }
        long endTime = System.currentTimeMillis();
        System.out.println("Took " + (endTime-startTime) + "milliseconds");
        return -1;
    }
}
