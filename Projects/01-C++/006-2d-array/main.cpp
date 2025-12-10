#include <iostream>

using namespace std;
void display2DArray(int arr[][5], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            std::cout << *(*(arr + i) + j) << " ";
        }
        std::cout << std::endl;
    }
    
}

int main(void) {
    int tdArr[3][5] = {
        {01, 02, 03, 04, 05},
        {6, 7, 8, 9, 10},
        {11, 12, 13, 14, 15}};
    display2DArray(tdArr, 3, 5);

    return 0;
}