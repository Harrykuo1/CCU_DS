#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>


//setting
#define PRIORITY_MAX_LENGTH 100005
#define NODE_MAX 1005
#define EDGE_MAX 100005
#define REQUEST_MAX 10005

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

int max(int a, int b) {
    if (a > b) {
        return a;
    }
    return b;
}

int min(int a, int b) {
    if (a < b) {
        return a;
    }
    return b;
}

int node, edge, request;
int nodeLimit[NODE_MAX];
int edgeArr[NODE_MAX][NODE_MAX];
int totalEdge[NODE_MAX];

int hasEdge[NODE_MAX][NODE_MAX];
int hasEdgeLen[NODE_MAX];
bool cantPass[NODE_MAX][NODE_MAX];



struct stPair {
    int start;
    int end;
    int pathWay;
    int id;
};
struct stPair stPairArr[REQUEST_MAX];

int stPair_compare(const void* _a, const void* _b) {
    struct stPair* a = (struct stPair*)_a;
    struct stPair* b = (struct stPair*)_b;
    int minA = min(min(nodeLimit[a->start], nodeLimit[a->end]), min(totalEdge[a->start], totalEdge[a->end]));
    int minB = min(min(nodeLimit[b->start], nodeLimit[b->end]), min(totalEdge[b->start], totalEdge[b->end]));
    if (minA == minB) {
        return a->id - b->id;
    }
    return minA - minB;
}

struct dijkstraNode {
    int pos;
    int path[NODE_MAX];
    int times;
    int memory;
};

struct priority_queue {
    struct dijkstraNode arr[PRIORITY_MAX_LENGTH];
    int size;
};

void priority_queue_init(struct priority_queue* priority_Q) {
    priority_Q->size = 0;
    memset(priority_Q->arr,0,sizeof(priority_Q->arr));
    return;
}

bool priority_queue_compare(struct dijkstraNode a, struct dijkstraNode b) {
    if (a.times == b.times) {
        return a.memory > b.memory;
    }
    return a.times > b.times;
}

void priority_queue_swap(struct dijkstraNode* edge1, struct dijkstraNode* edge2) {
    struct dijkstraNode tmp_stPair = *edge1;
    *edge1 = *edge2;
    *edge2 = tmp_stPair;
}

bool priority_queue_checkRange(int n, int limit) {
    if (n <= limit) {
        return true;
    }
    else {
        return false;
    }
}

struct dijkstraNode priority_queue_top(struct priority_queue* priority_Q) {
    return priority_Q->arr[1];
}

void priority_queue_push(struct priority_queue* priority_Q, struct dijkstraNode newItem) {
    int newItemId = ++priority_Q->size;
    while (newItemId > 1) {
        if (!priority_queue_compare(priority_Q->arr[newItemId / 2], newItem)) {
            break;
        }
        priority_Q->arr[newItemId] = priority_Q->arr[newItemId / 2];
        newItemId /= 2;
    }
    priority_Q->arr[newItemId] = newItem;
    return;

}

void priority_queue_pop(struct priority_queue* priority_Q) {     
    priority_queue_swap(&priority_Q->arr[1], &priority_Q->arr[priority_Q->size]);
    priority_Q->size--;
    int nowLocation = 1;
    while (nowLocation < priority_Q->size) {
        int leftChildLocation = nowLocation * 2;
        int rightChildLocation = nowLocation * 2 + 1;
        if (priority_queue_checkRange(leftChildLocation, priority_Q->size)
            && priority_queue_checkRange(rightChildLocation, priority_Q->size)
            && (!priority_queue_compare(priority_Q->arr[nowLocation], priority_Q->arr[leftChildLocation]))
            && (!priority_queue_compare(priority_Q->arr[nowLocation], priority_Q->arr[rightChildLocation]))) {
            break;
        }
        else if (priority_queue_checkRange(rightChildLocation, priority_Q->size)
            && priority_queue_compare(priority_Q->arr[nowLocation], priority_Q->arr[rightChildLocation])
            && !priority_queue_compare(priority_Q->arr[rightChildLocation], priority_Q->arr[leftChildLocation])) {
            priority_queue_swap(&priority_Q->arr[nowLocation], &priority_Q->arr[rightChildLocation]);
            nowLocation = rightChildLocation;
        }
        else if (priority_queue_checkRange(leftChildLocation, priority_Q->size)
            && priority_queue_compare(priority_Q->arr[nowLocation], priority_Q->arr[leftChildLocation])) {
            priority_queue_swap(&priority_Q->arr[nowLocation], &priority_Q->arr[leftChildLocation]);
            nowLocation = leftChildLocation;
        }
        else {
            break;
        }
    }
}

void programInit() {
    memset(edgeArr, 0, sizeof(edgeArr));
    memset(cantPass, false, sizeof(cantPass));
}

struct priority_queue pq;
bool isVisited[NODE_MAX];
void dijkstra(int start, int end, int id,int I) {
   
    priority_queue_init(&pq);
    if (nodeLimit[start] == 0 || nodeLimit[end] == 0 || cantPass[start][end]) {
        return;
    }
    priority_queue_init(&pq);
    memset(isVisited, false, sizeof(isVisited));
    struct dijkstraNode tmp, ans;
    tmp.pos = start;
    tmp.path[0] = start;
    tmp.times = 1;
    priority_queue_push(&pq, tmp);
    bool isArrived = false;
   
    while (pq.size > 0 && !isArrived) {
       
      
        struct dijkstraNode head = priority_queue_top(&pq);
       
        priority_queue_pop(&pq);
        
        if (isVisited[head.pos]) {
            continue;
        }
        
        isVisited[head.pos] = true;
       
        for (int i = 0;i < hasEdgeLen[head.pos] && !isArrived;i++) {
            
            if (edgeArr[head.pos][hasEdge[head.pos][i]] >= 1) {
                if (hasEdge[head.pos][i] == end && nodeLimit[end] >= 1) {
                    isArrived = true;
                    tmp = head;
                    tmp.pos = hasEdge[head.pos][i];
                    tmp.path[tmp.times] = tmp.pos;
                    tmp.times++;
                    ans = tmp;
                    break;
                }
                else if (nodeLimit[hasEdge[head.pos][i]] >= 2) {
                    tmp = head;
                    tmp.pos = hasEdge[head.pos][i];
                    tmp.path[tmp.times] = tmp.pos;
                    tmp.times++;
                    priority_queue_push(&pq, tmp);
                }
            }
            
        }
        
    }
    
    if (isArrived) {
        finishDataArr[finishPair].id = id;
        for (int i = 0;i < ans.times - 1;i++) {
            finishDataArr[finishPair].path[i] = ans.path[i];
            edgeArr[ans.path[i]][ans.path[i + 1]]--;
            edgeArr[ans.path[i + 1]][ans.path[i]]--;
            if (i == 0) {
                nodeLimit[ans.path[i]]--;
            }
            else {
                nodeLimit[ans.path[i]] -= 2;
            }
        }
        finishDataArr[finishPair].path[ans.times - 1] = ans.path[ans.times - 1];
        finishDataArr[finishPair].length = ans.times;
        finishPair++;
    }
    else{
        cantPass[start][end]=true;
        cantPass[end][start]=true;
    }
}

signed main() {
    programInit();
    scanf("%d%d%d", &node, &edge, &request);
    for (int i = 0;i < node;i++) {
        scanf("%*d%d", &nodeLimit[i]);
    }

    int LinkID, LinkEnd1, LinkEnd2, Channels;
    for (int i = 0;i < edge;i++) {
        scanf("%d%d%d%d", &LinkID, &LinkEnd1, &LinkEnd2, &Channels);
        edgeArr[LinkEnd1][LinkEnd2] += Channels;
        edgeArr[LinkEnd2][LinkEnd1] += Channels;
        hasEdge[LinkEnd1][hasEdgeLen[LinkEnd1]] = LinkEnd2;
        hasEdge[LinkEnd2][hasEdgeLen[LinkEnd2]] = LinkEnd1;
        hasEdgeLen[LinkEnd1]++;
        hasEdgeLen[LinkEnd2]++;
    }

    int ReqID, ReqSrc, ReqDst;
    for (int i = 0;i < request;i++) {
        scanf("%d%d%d", &ReqID, &ReqSrc, &ReqDst);
        stPairArr[i].start = ReqSrc;
        stPairArr[i].end = ReqDst;
        stPairArr[i].id = ReqID;
    }
    for (int j = 0;j < node;j++) {
        int sum = 0;
        for (int i = 0;i < node;i++) {
            sum += edgeArr[j][i];
        }
        totalEdge[j] = sum;
    }
    qsort((void*)stPairArr, request, sizeof(stPairArr[0]), stPair_compare);
    for (int i = 0;i < request;i++) {
        dijkstra(stPairArr[i].start, stPairArr[i].end, stPairArr[i].id, i);
        //printf("\n\n");
    }
    

    qsort((void*)finishDataArr, finishPair, sizeof(finishDataArr[0]), finishData_compare);
    printf("%d\n", finishPair);
    for (int j = 0;j < finishPair;j++) {
        printf("%d ", finishDataArr[j].id);
        for (int i = 0;i < finishDataArr[j].length;i++) {
            printf("%d ", finishDataArr[j].path[i]);
        }
        printf("\n");
    }

}

/*
10 20 30
0 10
1 10
2 10
3 10
4 10
5 10
6 10
7 10
8 10
9 10
0 7 7 8
1 10 5 10
2 6 10 7
3 6 10 5
4 7 7 5
5 7 10 10
6 9 6 8
7 8 5 10
8 10 7 6
9 10 8 10
10 6 8 6
11 10 9 9
12 8 10 8
13 9 8 6
14 10 6 6
15 8 8 10
16 7 6 9
17 9 7 7
18 5 8 6
19 8 5 10
0 9 6
1 9 7
2 7 8
3 7 6
4 8 9
5 8 5
6 6 6
7 6 5
8 6 10
9 10 10
10 9 6
11 8 10
12 8 9
13 10 7
14 5 8
15 8 5
16 7 8
17 8 8
18 9 5
19 5 10
20 5 8
21 8 10
22 6 8
23 6 10
24 10 8
25 10 9
26 9 9
27 7 8
28 7 9
29 10 7
*/

