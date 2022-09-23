#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>

//setting
#define PRIORITY_MAX_LENGTH 1001
#define NODE_MAX 101
#define EDGE_MAX 1000

bool TEST = false;

int node, edge, request;
int nodeLimit[NODE_MAX];
int edgeArr[NODE_MAX][NODE_MAX];

int hasEdge[NODE_MAX][NODE_MAX];
int hasEdgeLen[NODE_MAX];

int finishPair = 0;

struct finishData {
    int path[NODE_MAX];
    int length;
    int id;
};

int finishData_compare(const void* a, const void* b) {
    struct finishData c = *(struct finishData*)a;
    struct finishData d = *(struct finishData*)b;
    if (c.id > d.id) {
        return 1;
    }
    else {
        return 0;
    }
}

struct finishData finishDataArr[EDGE_MAX];

int min(int a, int b) {
    if (a < b) {
        return a;
    }
    return b;
}

int max(int a, int b) {
    if (a > b) {
        return a;
    }
    return b;
}

struct stPair {
    int start;
    int end;
    int pathWay;
    int id;
};

struct stPair_priority_queue {
    struct stPair list[PRIORITY_MAX_LENGTH];
    int size;
};
struct stPair_priority_queue stPair_priority_Q;

void stPairQ_init(struct stPair_priority_queue* priority_Q) {
    priority_Q->size = 0;
}

void stPairQ_swap(struct stPair* edge1, struct stPair* edge2) {
    struct stPair tmp_stPair = *edge1;
    *edge1 = *edge2;
    *edge2 = tmp_stPair;
}

bool stPairQ_compare(struct stPair_priority_queue priority_Q, struct stPair edge1, struct stPair edge2) {
    if (edge1.pathWay == edge2.pathWay) {
        int edge1MinLimit = nodeLimit[edge1.start] < nodeLimit[edge1.end] ? nodeLimit[edge1.start] : nodeLimit[edge1.end];
        int edge2MinLimit = nodeLimit[edge2.start] < nodeLimit[edge2.end] ? nodeLimit[edge2.start] : nodeLimit[edge2.end];
        return edge1MinLimit > edge2MinLimit;
    }
    else if (edge1.pathWay < edge2.pathWay) {
        return false;
    }
    else {
        return true;
    }
}

void stPairQ_insert(struct stPair_priority_queue* priority_Q, struct stPair newEdge) {
    priority_Q->size++;
    priority_Q->list[priority_Q->size] = newEdge;
    int nowLocation = priority_Q->size;
    while (nowLocation > 1) {
        int parentLocation = nowLocation / 2;
        if (stPairQ_compare(*priority_Q, priority_Q->list[parentLocation], priority_Q->list[nowLocation])) {
            stPairQ_swap(&priority_Q->list[parentLocation], &priority_Q->list[nowLocation]);
            nowLocation /= 2;
        }
        else {
            break;
        }

    }
    return;
}

struct stPair stPairQ_top(struct stPair_priority_queue priority_Q) {
    return priority_Q.list[1];
}

bool stPairQ_checkRange(int n, int limit) {
    if (n <= limit) {
        return true;
    }
    else {
        return false;
    }
}

void stPairQ_pop(struct stPair_priority_queue* priority_Q) {
    stPairQ_swap(&priority_Q->list[1], &priority_Q->list[priority_Q->size]);
    priority_Q->size--;
    int nowLocation = 1;
    while (nowLocation < priority_Q->size) {
        int leftChildLocation = nowLocation * 2;
        int rightChildLocation = nowLocation * 2 + 1;
        if (stPairQ_checkRange(leftChildLocation, priority_Q->size)
            && stPairQ_checkRange(rightChildLocation, priority_Q->size)
            && (!stPairQ_compare(*priority_Q, priority_Q->list[nowLocation], priority_Q->list[leftChildLocation]))
            && (!stPairQ_compare(*priority_Q, priority_Q->list[nowLocation], priority_Q->list[rightChildLocation]))) {
            break;
        }
        else if (stPairQ_checkRange(rightChildLocation, priority_Q->size)
            && stPairQ_compare(*priority_Q, priority_Q->list[nowLocation], priority_Q->list[rightChildLocation])
            && !stPairQ_compare(*priority_Q, priority_Q->list[rightChildLocation], priority_Q->list[leftChildLocation])) {
            stPairQ_swap(&priority_Q->list[nowLocation], &priority_Q->list[rightChildLocation]);
            nowLocation = rightChildLocation;
        }
        else if (stPairQ_checkRange(leftChildLocation, priority_Q->size)
            && stPairQ_compare(*priority_Q, priority_Q->list[nowLocation], priority_Q->list[leftChildLocation])) {
            stPairQ_swap(&priority_Q->list[nowLocation], &priority_Q->list[leftChildLocation]);
            nowLocation = leftChildLocation;
        }
        else {
            break;
        }
    }
}

struct dijkstraNode {
    int pos;
    int path[NODE_MAX];
    int times;
};

struct dijkstra_priority_queue {
    struct dijkstraNode list[PRIORITY_MAX_LENGTH];
    int size;
};

void dijkstraQ_CopyPath(struct dijkstraNode src, struct dijkstraNode* dst) {
    for (int i = 0;i < src.times;i++) {
        dst->path[i] = src.path[i];
    }
    dst->path[src.times] = dst->pos;
}

bool dijkstraQ_checkedPath(struct dijkstraNode src, int checkNum) {
    for (int i = 0;i < src.times;i++) {
        if (src.path[i] == checkNum) {
            return false;
        }
    }
    return true;
}

void dijkstraQ_init(struct dijkstra_priority_queue* priority_Q) {
    priority_Q->size = 0;
}

void dijkstraQ_swap(struct dijkstraNode* edge1, struct dijkstraNode* edge2) {
    struct dijkstraNode tmp = *edge1;
    *edge1 = *edge2;
    *edge2 = tmp;
}

bool dijkstraQ_compare(struct dijkstra_priority_queue priority_Q, struct dijkstraNode edge1, struct dijkstraNode edge2) {
    if (edge1.times == edge2.times) {
        return false;
    }
    else if (edge1.times < edge2.times) {
        return false;
    }
    else {
        return true;
    }
}

void dijkstraQ_insert(struct dijkstra_priority_queue* priority_Q, struct dijkstraNode newEdge) {
    priority_Q->size++;
    priority_Q->list[priority_Q->size] = newEdge;
    int nowLocation = priority_Q->size;
    while (nowLocation > 1) {
        int parentLocation = nowLocation / 2;
        if (dijkstraQ_compare(*priority_Q, priority_Q->list[parentLocation], priority_Q->list[nowLocation])) {
            dijkstraQ_swap(&priority_Q->list[parentLocation], &priority_Q->list[nowLocation]);
            nowLocation /= 2;
        }
        else {
            break;
        }

    }
    return;
}

struct dijkstraNode dijkstraQ_top(struct dijkstra_priority_queue priority_Q) {
    return priority_Q.list[1];
}

bool dijkstraQ_checkRange(int n, int limit) {
    if (n <= limit) {
        return true;
    }
    else {
        return false;
    }
}

void dijkstraQ_pop(struct dijkstra_priority_queue* priority_Q) {
    dijkstraQ_swap(&priority_Q->list[1], &priority_Q->list[priority_Q->size]);
    priority_Q->size--;
    int nowLocation = 1;
    while (nowLocation < priority_Q->size) {
        int leftChildLocation = nowLocation * 2;
        int rightChildLocation = nowLocation * 2 + 1;
        if (dijkstraQ_checkRange(leftChildLocation, priority_Q->size)
            && dijkstraQ_checkRange(rightChildLocation, priority_Q->size)
            && (!dijkstraQ_compare(*priority_Q, priority_Q->list[nowLocation], priority_Q->list[leftChildLocation]))
            && (!dijkstraQ_compare(*priority_Q, priority_Q->list[nowLocation], priority_Q->list[rightChildLocation]))) {
            break;
        }
        else if (dijkstraQ_checkRange(rightChildLocation, priority_Q->size)
            && dijkstraQ_compare(*priority_Q, priority_Q->list[nowLocation], priority_Q->list[rightChildLocation])
            && !dijkstraQ_compare(*priority_Q, priority_Q->list[rightChildLocation], priority_Q->list[leftChildLocation])) {
            dijkstraQ_swap(&priority_Q->list[nowLocation], &priority_Q->list[rightChildLocation]);
            nowLocation = rightChildLocation;
        }
        else if (dijkstraQ_checkRange(leftChildLocation, priority_Q->size)
            && dijkstraQ_compare(*priority_Q, priority_Q->list[nowLocation], priority_Q->list[leftChildLocation])) {
            dijkstraQ_swap(&priority_Q->list[nowLocation], &priority_Q->list[leftChildLocation]);
            nowLocation = leftChildLocation;
        }
        else {
            break;
        }
    }
}

void init() {
    memset(nodeLimit, 0, sizeof(nodeLimit));
    memset(edgeArr, 0, sizeof(edgeArr));
    memset(hasEdgeLen, 0, sizeof(hasEdgeLen));
    return;
}
/*
int maxflow(struct stPair head, int edgeArr[], int nodeLimit[]) {
    return 0;
}
*/
int stPair_compare(const void* a, const void* b) {
    struct stPair c = *(struct stPair*)a;
    struct stPair d = *(struct stPair*)b;
    int maxC = max(nodeLimit[c.start], nodeLimit[c.end]);
    int maxD = max(nodeLimit[d.start], nodeLimit[d.end]);

    int edgeStartC = 0;
    for (int i = 0;i < node;i++) {
        edgeStartC += edgeArr[i][c.start];
    }

    int edgeEndC = 0;
    for (int i = 0;i < node;i++) {
        edgeEndC += edgeArr[i][c.end];
    }
    maxC = max(maxC, max(edgeStartC, edgeEndC));

    int edgeStartD = 0;
    for (int i = 0;i < node;i++) {
        edgeStartD += edgeArr[i][d.start];
    }

    int edgeEndD = 0;
    for (int i = 0;i < node;i++) {
        edgeEndD += edgeArr[i][d.end];
    }
    maxD = max(maxD, max(edgeStartD, edgeEndD));

    if (maxC < maxD) {
        return 1;
    }
    else if (maxC == maxD) {
        return c.id > d.id;
    }
    else {
        return 0;
    }

}

struct dijkstra_priority_queue dijkstraQ;
bool isVisited[NODE_MAX];
void dijkstra(int start, int end, int id) {
    if (TEST) {
        printf("st: %d %d\n", start, end);
    }
    dijkstraQ_init(&dijkstraQ);
    if (nodeLimit[start] == 0) {
        return;
    }
    if (nodeLimit[end] == 0) {
        return;
    }
    bool isArrived = false;
    memset(isVisited, false, sizeof(isVisited));
    memset(dijkstraQ.list, 0, sizeof(dijkstraQ.list));
    struct dijkstraNode tmp, ans;
    tmp.pos = start;
    tmp.path[0] = start;
    tmp.times = 1;
    dijkstraQ_insert(&dijkstraQ, tmp);
    while (dijkstraQ.size > 0 && !isArrived) {
        struct dijkstraNode head = dijkstraQ_top(dijkstraQ);
        if (TEST) {
            for (int i = 0;i < head.times;i++) {
                printf("%d ", head.path[i]);
            }
            printf("\n");
        }
        dijkstraQ_pop(&dijkstraQ);
        if(isVisited[head.pos]){
            continue;
        }
        isVisited[head.pos]=true;
        for (int i = 0;i < hasEdgeLen[head.pos] && !isArrived;i++) {
            if (edgeArr[head.pos][hasEdge[head.pos][i]] >= 1 && !isVisited[hasEdge[head.pos][i]]) {
                if (hasEdge[head.pos][i] == end && nodeLimit[hasEdge[head.pos][i]] >= 1) {
                    tmp.pos = hasEdge[head.pos][i];
                    tmp.times = head.times + 1;
                    dijkstraQ_CopyPath(head, &tmp);
                    dijkstraQ_insert(&dijkstraQ, tmp);
                    ans = tmp;
                    isArrived = true;
                    if (TEST) {
                        printf("OK\n");
                    }
                    break;
                }
                else if (nodeLimit[hasEdge[head.pos][i]] >= 2) {
                    tmp.pos = hasEdge[head.pos][i];
                    tmp.times = head.times + 1;
                    dijkstraQ_CopyPath(head, &tmp);
                    dijkstraQ_insert(&dijkstraQ, tmp);
                }
                else {
                    continue;
                }
            }
            else {
                continue;
            }
        }
    }

    if (isArrived) {
        finishDataArr[finishPair].id = id;
        for (int i = 0;i < ans.times - 1;i++) {
            finishDataArr[finishPair].path[i] = ans.path[i];

            edgeArr[ans.path[i]][ans.path[i + 1]]--;
            edgeArr[ans.path[i + 1]][ans.path[i]]--;
            if (i != 0) {
                nodeLimit[ans.path[i]] -= 2;
            }
            else {
                nodeLimit[ans.path[i]]--;
            }
        }
        finishDataArr[finishPair].path[ans.times - 1] = ans.path[ans.times - 1];
        nodeLimit[ans.path[ans.times - 1]]--;
        finishDataArr[finishPair].length = ans.times;
        finishPair++;
    }
}

signed main() {
    init();
    scanf("%d%d%d", &node, &edge, &request);

    for (int i = 0;i < node;i++) {
        scanf("%d%d", &nodeLimit[i], &nodeLimit[i]);
    }

    int LinkID, LinkEnd1, LinkEnd2, Channels;
    for (int i = 0;i < edge;i++) {
        scanf("%d%d%d%d", &LinkID, &LinkEnd1, &LinkEnd2, &Channels);
        edgeArr[LinkEnd1][LinkEnd2] += Channels;
        edgeArr[LinkEnd2][LinkEnd1] += Channels;
        hasEdge[LinkEnd1][hasEdgeLen[LinkEnd1]]=LinkEnd2;
        hasEdge[LinkEnd2][hasEdgeLen[LinkEnd2]]=LinkEnd1;
        hasEdgeLen[LinkEnd1]++;
        hasEdgeLen[LinkEnd2]++;
    }

    int ReqID, ReqSrc, ReqDst;
    struct stPair tmp_stPair;
    stPairQ_init(&stPair_priority_Q);
    for (int i = 0;i < request;i++) {
        scanf("%d%d%d", &ReqID, &ReqSrc, &ReqDst);
        tmp_stPair.start = ReqSrc;
        tmp_stPair.end = ReqDst;
        tmp_stPair.pathWay = -1;
        tmp_stPair.id = ReqID;
        stPairQ_insert(&stPair_priority_Q, tmp_stPair);
    }

    /*
    printf("\n");
    for (int i = 1;i <= stPair_priority_Q.size;i++) {
        printf("%d %d %d\n", stPair_priority_Q.list[i].start, stPair_priority_Q.list[i].end, stPair_priority_Q.list[i].pathWay);
    }
    */
    /*
     while (stPair_priority_Q.size > 0) {
         struct stPair head = stPairQ_top(stPair_priority_Q);
         struct stPair tmp;
         if (head.pathWay != -1) {
             break;
         }
         tmp = head;
         tmp.pathWay = maxFlow(head, edgeArr, nodeLimit);
         stPairQ_pop(&stPair_priority_Q);
         stPairQ_insert(&stPair_priority_Q, tmp);
     }
     */
    qsort((void*)&stPair_priority_Q.list[1], stPair_priority_Q.size, sizeof(stPair_priority_Q.list[0]), stPair_compare);
    /*while (stPair_priority_Q.size > 0) {
        struct stPair head = stPairQ_top(stPair_priority_Q);
        stPairQ_pop(&stPair_priority_Q);
        dijkstra(head.start, head.end, head.id);
    }*/
    
    for (int i = 1;i <= stPair_priority_Q.size;i++) {
        struct stPair head = stPair_priority_Q.list[i];
        dijkstra(head.start, head.end, head.id);
    }
    
    qsort((void*)finishDataArr, finishPair, sizeof(finishDataArr[0]), finishData_compare);
    printf("%d\n", finishPair);
    for (int i = 0;i < finishPair;i++) {
        printf("%d ", finishDataArr[i].id);
        for (int j = 0;j < finishDataArr[i].length;j++) {
            printf("%d ", finishDataArr[i].path[j]);
        }
        printf("\n");
    }



}





/*
6 6 7
0 7
1 4
2 5
3 10
4 8
5 3
0 0 1 1
1 1 2 1
2 0 3 6
3 3 4 5
4 2 4 4
5 1 5 8
0 0 2
1 0 5
2 0 2
3 0 5
4 0 5
5 0 2
6 0 2
*/

/*
6 6 16
0 7
1 4
2 5
3 10
4 8
5 3
0 0 1 1
1 1 2 1
2 0 3 6
3 3 4 5
4 2 4 4
5 1 5 8
0 0 2
1 0 5
2 0 2
3 0 5
4 0 5
5 0 2
6 0 2
7 0 2
8 0 2
9 0 2
10 0 2
11 0 2
12 0 2
13 0 2
14 0 2
15 0 2
*/