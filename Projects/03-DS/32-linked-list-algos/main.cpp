#include "linkedList.h"
#include <string>
#include "binary-search.h"
#include "linear-search.h"
#include "bubble-sort.h"
#include "insertion-sort.h"
#include "selection-sort.h"

int main() {
    LinkedList<int> ll;
    ll.insert(12);
    ll.insert(1);
    ll.insert(1);
    selectionSort(ll);
    ll.print();
    return 0;
}