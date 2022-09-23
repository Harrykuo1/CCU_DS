#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>


//setting
#define PRIORITY_MAX_LENGTH 100005
#define NODE_MAX 1005
#define EDGE_MAX 5005
#define REQUEST_MAX 50005

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
    return;
}

bool priority_queue_compare(struct dijkstraNode a, struct dijkstraNode b) {
    if (a.times == b.times) {
        return a.memory > b.memory;
    }
    return a.times > b.times;
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
    struct dijkstraNode item = priority_Q->arr[priority_Q->size--];
    int itemId = 1;
    while (itemId * 2 <= priority_Q->size) {
        int l = itemId * 2, r = itemId * 2 + 1;
        if (r <= priority_Q->size && !priority_queue_compare(priority_Q->arr[r], priority_Q->arr[l])) {
            l = r;
        }
        if (!priority_queue_compare(item, priority_Q->arr[l])) {
            break;
        }
        priority_Q->arr[itemId] = priority_Q->arr[l];
        itemId = 1;
    }
    priority_Q->arr[itemId] = item;
}

void programInit() {
    memset(edgeArr, 0, sizeof(edgeArr));
    memset(cantPass, false, sizeof(cantPass));
}

struct priority_queue pq;
bool isVisited[NODE_MAX];
int dfsPath[NODE_MAX];
bool isOver = false;
void dfs(int start, int end, int now, int index, int id) {
    if (nodeLimit[start] == 0 || nodeLimit[end] == 0 || cantPass[start][end]) {
        return;
    }
    dfsPath[index] = now;
    if (now == end) {
        isOver = true;
        finishDataArr[finishPair].id = id;
        for (int i = 0;i <= index;i++) {
            finishDataArr[finishPair].path[i] = dfsPath[i];
            //printf("%d ", dfsPath[i]);
        }
        //printf("\n");
        for (int i = 0;i < index;i++) {
            edgeArr[dfsPath[i]][dfsPath[i + 1]]--;
            edgeArr[dfsPath[i + 1]][dfsPath[i]]--;
            if (i == 0) {
                nodeLimit[dfsPath[i]]--;
            }
            else {
                nodeLimit[dfsPath[i]] -= 2;
            }
        }
        nodeLimit[dfsPath[index]]--;
        finishDataArr[finishPair].length = index + 1;
        finishPair++;
        return;
    }
    for (int i = 0;i < hasEdgeLen[now];i++) {
        if(isOver){
            return;
        }
        if (edgeArr[now][hasEdge[now][i]] >= 1) {
            if (hasEdge[now][i] == end) {

                dfs(start, end, hasEdge[now][i], index + 1, id);
            }
            else if (nodeLimit[hasEdge[now][i]] >= 2 && !isVisited[hasEdge[now][i]]) {
                isVisited[hasEdge[now][i]] = true;
                dfs(start, end, hasEdge[now][i], index + 1, id);
                isVisited[hasEdge[now][i]] = false;
            }
        }
    }
}

int hasEdgetmp;
int hasEdgeCompare(const void* _a, const void* _b) {
    int* a = (int*)_a;
    int* b = (int*)_b;
    return edgeArr[hasEdgetmp][*a] - edgeArr[hasEdgetmp][*b];
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
    for (int i = 0;i < node;i++) {
        hasEdgetmp = i;
        qsort((void*)&hasEdge[i], hasEdgeLen[i], sizeof(hasEdge[i][0]), hasEdgeCompare);
    }

    int ReqID, ReqSrc, ReqDst, cnt = 0;
    for (int i = 0;i < request;i++) {
        scanf("%d%d%d", &ReqID, &ReqSrc, &ReqDst);
        if (edgeArr[ReqSrc][ReqDst] != 0 && nodeLimit[ReqSrc] >= 1 && nodeLimit[ReqDst] >= 1) {
            finishDataArr[finishPair].id = ReqID;
            finishDataArr[finishPair].path[0] = ReqSrc;
            finishDataArr[finishPair].path[1] = ReqDst;
            finishDataArr[finishPair].length=2;
            finishPair++;
            nodeLimit[ReqSrc]--;
            nodeLimit[ReqDst]--;
            edgeArr[ReqSrc][ReqDst]--;
            edgeArr[ReqDst][ReqSrc]--;

        }
        else {
            stPairArr[cnt].start = ReqSrc;
            stPairArr[cnt].end = ReqDst;
            stPairArr[cnt].id = ReqID;
            cnt++;
        }
    }
    request = cnt;
    for (int j = 0;j < node;j++) {
        int sum = 0;
        for (int i = 0;i < node;i++) {
            sum += edgeArr[j][i];
        }
        totalEdge[j] = sum;
    }
    qsort((void*)stPairArr, request, sizeof(stPairArr[0]), stPair_compare);
    for (int i = 0;i < request;i++) {
        //dijkstra(stPairArr[i].start, stPairArr[i].end, stPairArr[i].id);
        isOver = false;
        memset(isVisited, false, sizeof(isVisited));
        dfs(stPairArr[i].start, stPairArr[i].end, stPairArr[i].start, 0, stPairArr[i].id);
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

