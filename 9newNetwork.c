#include<stdio.h>
#include<stdbool.h>
#include<string.h>

//setting
#define QUEUE_MAX 10000005
#define NODE_MAX 1005
#define REQUEST_MAX 5005

int node, edge, request;
int nodeLimit[NODE_MAX];
int edgeArr[NODE_MAX][NODE_MAX];

int hasEdge[NODE_MAX][NODE_MAX];
int hasEdgeLen[NODE_MAX];
int totalEdge[NODE_MAX];

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

struct finishData finishDataArr[REQUEST_MAX];

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

struct qItem {
    int pos;
    int parent;
    int times;
};

struct queue {
    struct qItem* arr;
    int size;
};

void queue_init(struct queue* queue) {
    queue->arr = (struct qItem*)malloc(QUEUE_MAX * sizeof(struct qItem));
    memset(queue->arr, 0, sizeof(queue->arr));
    queue->size = 0;
    return;
}

void queue_swap(struct qItem* edge1, struct qItem* edge2) {
    struct qItem tmp = *edge1;
    *edge1 = *edge2;
    *edge2 = tmp;
}

bool queue_compare(struct queue* queue, struct qItem edge1, struct qItem edge2) {
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

bool queue_checkRange(int n, int limit) {
    if (n <= limit) {
        return true;
    }
    else {
        return false;
    }
}

void queue_push(struct queue* queue, struct qItem newItem) {
    queue->size++;
    queue->arr[queue->size] = newItem;
    int nowLocation = queue->size;
    while (nowLocation > 1) {
        int parentLocation = nowLocation / 2;
        if (queue_compare(queue, queue->arr[parentLocation], queue->arr[nowLocation])) {
            queue_swap(&queue->arr[parentLocation], &queue->arr[nowLocation]);
            nowLocation /= 2;
        }
        else {
            break;
        }

    }
    return;
}
void queue_pop(struct queue* queue) {
    queue_swap(&queue->arr[1], &queue->arr[queue->size]);
    queue->size--;
    int nowLocation = 1;
    while (nowLocation < queue->size) {
        int leftChildLocation = nowLocation * 2;
        int rightChildLocation = nowLocation * 2 + 1;
        if (queue_checkRange(leftChildLocation, queue->size)
            && queue_checkRange(rightChildLocation, queue->size)
            && (!queue_compare(queue, queue->arr[nowLocation], queue->arr[leftChildLocation]))
            && (!queue_compare(queue, queue->arr[nowLocation], queue->arr[rightChildLocation]))) {
            break;
        }
        else if (queue_checkRange(rightChildLocation, queue->size)
            && queue_compare(queue, queue->arr[nowLocation], queue->arr[rightChildLocation])
            && !queue_compare(queue, queue->arr[rightChildLocation], queue->arr[leftChildLocation])) {
            queue_swap(&queue->arr[nowLocation], &queue->arr[rightChildLocation]);
            nowLocation = rightChildLocation;
        }
        else if (queue_checkRange(leftChildLocation, queue->size)
            && queue_compare(queue, queue->arr[nowLocation], queue->arr[leftChildLocation])) {
            queue_swap(&queue->arr[nowLocation], &queue->arr[leftChildLocation]);
            nowLocation = leftChildLocation;
        }
        else {
            break;
        }
    }
}

struct qItem queue_front(struct queue* queue) {
    return queue->arr[1];
}

int queue_getSize(struct queue* queue) {
    return queue->size;
}



void programInit() {
    memset(edgeArr, 0, sizeof(edgeArr));
    memset(hasEdgeLen, 0, sizeof(hasEdgeLen));
    memset(hasEdge, 0, sizeof(hasEdge));
    memset(totalEdge, 0, sizeof(totalEdge));
    return;
}

struct queue queue[QUEUE_MAX];
int visited[NODE_MAX];
void bfs(int start, int end, int id) {
    queue_init(queue);
    for (int i = 0;i < NODE_MAX;i++) {
        visited[i] = -100;
    }

    if (nodeLimit[start] < 1 || nodeLimit[end] < 1) {
        return;
    }

    struct qItem tmp, head;
    tmp.pos = start;
    tmp.parent = -1;
    tmp.times = 1;
    queue_push(queue, tmp);
    bool isArrived = false;
    int ansLen = 0;

    while (queue_getSize(queue) > 0) {
        head = queue_front(queue);
        queue_pop(queue);

        if (visited[head.pos] != -100) {
            continue;
        }
        visited[head.pos] = head.parent;
        if (head.pos == end) {
            isArrived = true;
            ansLen = head.times;
            break;
        }
        for (int i = 0;i < hasEdgeLen[head.pos];i++) {
            if (isArrived) {
                break;
            }
            if (edgeArr[head.pos][hasEdge[head.pos][i]] >= 1) {
                if (hasEdge[head.pos][i] == end && nodeLimit[end] >= 1) {
                    tmp.pos = end;
                    tmp.parent = head.pos;
                    tmp.times = head.times + 1;
                    queue_push(queue, tmp);
                }
                else if (nodeLimit[hasEdge[head.pos][i]] >= 2) {
                    tmp.pos = hasEdge[head.pos][i];
                    tmp.parent = head.pos;
                    tmp.times = head.times + 1;
                    queue_push(queue, tmp);
                }
            }
        }
    }
    if (isArrived) {
        finishDataArr[finishPair].id = id;
        finishDataArr[finishPair].path[ansLen - 1] = end;
        finishDataArr[finishPair].length = ansLen;
        nodeLimit[end] -= 1;
        for (int i = ansLen - 2;i >= 0;i--) {
            finishDataArr[finishPair].path[i] = visited[finishDataArr[finishPair].path[i + 1]];
        }
        for (int i = 0;i < ansLen - 1;i++) {
            if (i == 0) {
                nodeLimit[start] -= 1;
            }
            else {
                nodeLimit[finishDataArr[finishPair].path[i]] -= 2;
            }
            edgeArr[finishDataArr[finishPair].path[i]][finishDataArr[finishPair].path[i + 1]]--;
            edgeArr[finishDataArr[finishPair].path[i + 1]][finishDataArr[finishPair].path[i]]--;
        }
        finishPair++;
    }
}

int hasEdge_compare_base;
int hasEdge_compare(const void* _a, const void* _b) {
    int* a = (int*)_a;
    int* b = (int*)_b;
    return edgeArr[hasEdge_compare_base][*b] - edgeArr[hasEdge_compare_base][*a];
}

void reSort() {
    for (int i = 0;i < node;i++) {
        hasEdge_compare_base = i;
        qsort(hasEdge[i], hasEdgeLen[i], sizeof(hasEdge[0]), hasEdge_compare);
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

    int ReqID, ReqSrc, ReqDst, cnt = 0;
    for (int i = 0;i < request;i++) {
        scanf("%d%d%d", &ReqID, &ReqSrc, &ReqDst);

        if (edgeArr[ReqSrc][ReqDst] != 0 && nodeLimit[ReqSrc] >= 1 && nodeLimit[ReqDst] >= 1) {
            finishDataArr[finishPair].id = ReqID;
            finishDataArr[finishPair].path[0] = ReqSrc;
            finishDataArr[finishPair].path[1] = ReqDst;
            finishDataArr[finishPair].length = 2;
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

        /*
        stPairArr[i].start = ReqSrc;
        stPairArr[i].end = ReqDst;
        stPairArr[i].id = ReqID;*/

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
        //reSort();
        bfs(stPairArr[i].start, stPairArr[i].end, stPairArr[i].id);
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
