#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
int impossiblenum = 99999999;
int nodes, links, reqs;
int passed = 0;
int graph[1005][1005];
int memory[1005];
int src[10005];
int end[10005];
int tempnum;
int result[10005][1005];
int dijkstra(int src, int end, int request, int nodes) {
    int path[nodes];
    int visited[nodes];
    int distance[nodes];
    int start = end;
    int count = 0;

    for (int a = 0; a < nodes; a++) {
        path[a] = -1;
        visited[a] = 0;
        distance[a] = impossiblenum;
    }
    distance[start] = 0;

    while(count < nodes) {
        visited[start] = 1;
        count++;
        for (int a = nodes - 1; a >= 0; a--) {
            if (graph[start][a] > 0) {
                if ((memory[a] > 1) && (memory[start] > 1) && ((start != end) || (a != src)) || (start == end) && (memory[a] > 1) || (a == src) && (memory[start] > 1)) {
                    if ((path[a] == -1) && ((distance[start]) + 1) < distance[a]) {
                        path[a] = start;
                        distance[a] = distance[start] + 1;
                    }
                }
            }
        }
        int min = impossiblenum - 1;
        for (int k = 0; k < nodes; k++) {
            if ((visited[k] == 0) && distance[k] < min) {
                start = k;
                min = distance[k];
            }
        }
    }

    //printf("Path:");
    //for (int n = 0; n < nodes; n++) {
    //    printf("%d ", path[n]);
    //}
    //printf("\n");

    //printf("distance:");
    //for (int n = 0; n < nodes; n++) {
        //printf("%d ", distance[n]);
    //}
    //printf("\n");

    if ((distance[src] > impossiblenum) || (distance[src] == impossiblenum) || (memory[src] == 0) || (memory[end] == 0)) {
        return 0;
    }

    start = src;
    int counter = 0;
    while(path[start] != -1) {
        graph[start][path[start]]--;
        graph[path[start]][start]--;
        memory[start]--;
        memory[path[start]]--;
        result[request][counter] = start;
        start = path[start];
        counter++;
    }
    result[request][counter] = start;
    passed++;
    return 0;
}


int main () {
    int nodeid, linkend1, linkend2, linkid;
    scanf("%d%d%d", &nodes, &links, &reqs);

    for (int a = 0; a < nodes; a++) {//將矩陣內所有的值和每個node的memory預設為0
        memory[a] = 0;
        for (int b = 0; b < nodes; b++) {
            graph[a][b] = 0;
        }
    }

    for (int a = 0; a < nodes; a++) {
        scanf("%d%d", &nodeid, &memory[a]);
    }

    int templink;
    for (int a = 0; a < links; a++) {
        scanf("%d%d%d%d", &linkid, &linkend1, &linkend2, &templink);
        graph[linkend2][linkend1]+=templink;
        graph[linkend1][linkend2]+=templink;
    }

    for (int a = 0; a < reqs; a++) {
        for (int b = 0; b < nodes; b++) {
            result[a][b] = impossiblenum;
        }
    }

    for (int a = 0; a < reqs; a++) {
        scanf("%d", &tempnum);
        scanf("%d%d", &src[tempnum], &end[tempnum]);
    }

    for (int a = 0; a < reqs; a++) {
        dijkstra(src[a], end[a], a, nodes);
    }

    printf("%d\n", passed);
    for (int a = 0; a < reqs; a++) {
        if (result[a][0] != impossiblenum) {
            printf("%d ", a);
            for (int b = 0; b < nodes; b++) {
                if (result[a][b] != impossiblenum) {
                   printf("%d ", result[a][b]);
                }
            }
            printf("\n");
        }
    }

    return 0;
}
