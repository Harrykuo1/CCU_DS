#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main() {
    srand(time(NULL));

    /* 指定亂數範圍 */
    int min = 1;
    int max = 10;
    int node = 1000;
    int edge = 5000;
    int req = 5000;

    printf("%d %d %d\n", node, edge, req);
    for (int i = 0;i < node;i++) {
        printf("%d %d\n", i, 10);
    }
    min = 1;
    max = 5000;


    for (int i = 0;i < edge;i++) {
        int a = rand() % (max - min + 1) + min;
        int b = rand() % (max - min + 1) + min;
        while (a == b) {
            b = rand() % (max - min + 1) + min;
        }
        printf("%d %d %d %d\n", i, a, b, rand() % (max - min + 1) + min);
    }
    for (int i = 0;i < req;i++) {
        int a = rand() % (max - min + 1) + min;
        int b = rand() % (max - min + 1) + min;
        while (a == b) {
            b = rand() % (max - min + 1) + min;
        }
        printf("%d %d %d\n", i, a, b);
    }

    return 0;
}