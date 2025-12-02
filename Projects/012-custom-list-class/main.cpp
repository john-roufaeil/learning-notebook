#include "listitem.h"
#include "list.h"

int main() {
    List l1 = List(20);
    l1.add(5);
    l1.add("Hello");
    l1.add('c');
    l1.add(false);
    l1.add(3.1f);
    l1.add(5.5);
    l1.detailPrint();
    l1.fullPrint();
}