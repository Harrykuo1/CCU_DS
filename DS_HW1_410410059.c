#include<stdio.h>
#include<stdbool.h>
#include<string.h>

//setting
#define QUEUE_MAX 10000005 //Max size of queue array
#define NODE_MAX 9005      //Max size of node array
#define REQUEST_MAX 10005  //Max size of request array

int node, edge, request, directAns = 0;
int nodeLimit[NODE_MAX];  //memory
int nodeLimitOrigin[NODE_MAX];


int edgeArr[NODE_MAX][NODE_MAX]; //channel
int edgeArrOrigin[NODE_MAX][NODE_MAX];

int hasEdge[NODE_MAX][NODE_MAX];
int hasEdgeLen[NODE_MAX];
int totalEdge[NODE_MAX];

int finishPair = 0;
int ansFinishPair = 0;
struct finishData {
    int path[NODE_MAX];
    int length;
    int id;
};

int finishData_compare(const void* a, const void* b) { //Answer array sort compare id func
    struct finishData c = *(struct finishData*)a;
    struct finishData d = *(struct finishData*)b;
    if (c.id > d.id) {
        return 1;
    }
    else {
        return 0;
    }
}

struct finishData finishDataArr[REQUEST_MAX];   //Save ans data
struct finishData ansFinishDataArr[REQUEST_MAX];

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

struct sdPair { //src and dst pair struct
    int start;
    int end;
    int pathWay;
    int id;
};
struct sdPair sdPairArr[REQUEST_MAX];

int sdPair_compare_id_min(const void* _a, const void* _b) { //Sort by min id
    struct sdPair* a = (struct sdPair*)_a;
    struct sdPair* b = (struct sdPair*)_b;
    return a->id - b->id;
}

int sdPair_compare_id_max(const void* _a, const void* _b) { //Sort by max id
    struct sdPair* a = (struct sdPair*)_a;
    struct sdPair* b = (struct sdPair*)_b;
    return b->id - a->id;
}

int sdPair_compare(const void* _a, const void* _b) { //Sort by min resource
    struct sdPair* a = (struct sdPair*)_a;
    struct sdPair* b = (struct sdPair*)_b;
    int minA = min(min(nodeLimit[a->start], nodeLimit[a->end]), min(totalEdge[a->start], totalEdge[a->end]));
    int minB = min(min(nodeLimit[b->start], nodeLimit[b->end]), min(totalEdge[b->start], totalEdge[b->end]));
    if (minA == minB) {
        return a->id - b->id;
    }
    return minA - minB;
}

int sdPair_compare2(const void* _a, const void* _b) { //Sort by min resource
    struct sdPair* a = (struct sdPair*)_a;
    struct sdPair* b = (struct sdPair*)_b;
    int minA = min(max(nodeLimit[a->start], nodeLimit[a->end]), max(totalEdge[a->start], totalEdge[a->end]));
    int minB = min(max(nodeLimit[b->start], nodeLimit[b->end]), max(totalEdge[b->start], totalEdge[b->end]));
    if (minA == minB) {
        return a->id - b->id;
    }
    return minA - minB;
}

int sdPair_compare3(const void* _a, const void* _b) { //Sort by max resource
    struct sdPair* a = (struct sdPair*)_a;
    struct sdPair* b = (struct sdPair*)_b;
    int minA = max(max(nodeLimit[a->start], nodeLimit[a->end]), max(totalEdge[a->start], totalEdge[a->end]));
    int minB = max(max(nodeLimit[b->start], nodeLimit[b->end]), max(totalEdge[b->start], totalEdge[b->end]));
    if (minA == minB) {
        return a->id - b->id;
    }
    return minA - minB;
}

int sdPair_compare4(const void* _a, const void* _b) {  //Sort by max resource
    struct sdPair* a = (struct sdPair*)_a;
    struct sdPair* b = (struct sdPair*)_b;
    int minA = max(min(nodeLimit[a->start], nodeLimit[a->end]), min(totalEdge[a->start], totalEdge[a->end]));
    int minB = max(min(nodeLimit[b->start], nodeLimit[b->end]), min(totalEdge[b->start], totalEdge[b->end]));
    if (minA == minB) {
        return a->id - b->id;
    }
    return minA - minB;
}

int sdPair_compare5(const void* _a, const void* _b) { //Sort by half nodeLimit
    struct sdPair* a = (struct sdPair*)_a;
    struct sdPair* b = (struct sdPair*)_b;
    int minA = min((nodeLimit[a->start] + nodeLimit[a->end]) / 2, (totalEdge[a->start] + totalEdge[a->end]) / 2);
    int minB = min((nodeLimit[b->start] + nodeLimit[b->end]) / 2, (totalEdge[b->start] + totalEdge[b->end]) / 2);
    if (minA == minB) {
        return a->id - b->id;
    }
    return minA - minB;
}



struct qItem { //Item in queue
    int pos;
    int parent;
    int times;
    int weight;
};

struct queue { //Priority queue
    struct qItem* arr;
    int size;
    int cmpMode;
};

void queue_init(struct queue* queue, int cmpMode) { //Initialize queue
    queue->arr = (struct qItem*)malloc(QUEUE_MAX * sizeof(struct qItem));
    memset(queue->arr, 0, sizeof(queue->arr));
    queue->size = 0;
    queue->cmpMode = cmpMode;
    return;
}

void queue_swap(struct qItem* edge1, struct qItem* edge2) { //Swap queueItem position
    struct qItem tmp = *edge1;
    *edge1 = *edge2;
    *edge2 = tmp;
}

bool queue_compare(struct queue* queue, struct qItem edge1, struct qItem edge2, int cmpMode) { //Compare func for priority queue
    if (cmpMode == 0) {
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
    else if (cmpMode == 1 || cmpMode == 2) {
        if (edge1.times-edge2.times==0) {
            if (edge1.weight == edge2.weight) {
                return edge1.pos > edge2.pos;
            }
            return edge1.weight < edge2.weight;
        }
        else if (edge1.times < edge2.times) {
            return false;
        }
        else {
            return true;
        }
    }
}

bool queue_checkRange(int n, int limit) { //Check range over limit or not
    if (n <= limit) {
        return true;
    }
    else {
        return false;
    }
}

void queue_push(struct queue* queue, struct qItem newItem) { //Push qItem into queue
    queue->size++;
    queue->arr[queue->size] = newItem;
    int nowLocation = queue->size;
    while (nowLocation > 1) {
        int parentLocation = nowLocation / 2;
        if (queue_compare(queue, queue->arr[parentLocation], queue->arr[nowLocation], queue->cmpMode)) {
            queue_swap(&queue->arr[parentLocation], &queue->arr[nowLocation]);
            nowLocation /= 2;
        }
        else {
            break;
        }

    }
    return;
}
void queue_pop(struct queue* queue) { // Pop qItem from queue
    queue_swap(&queue->arr[1], &queue->arr[queue->size]);
    queue->size--;
    int nowLocation = 1;
    while (nowLocation < queue->size) {
        int leftChildLocation = nowLocation * 2;
        int rightChildLocation = nowLocation * 2 + 1;
        if (queue_checkRange(leftChildLocation, queue->size)
            && queue_checkRange(rightChildLocation, queue->size)
            && (!queue_compare(queue, queue->arr[nowLocation], queue->arr[leftChildLocation], queue->cmpMode))
            && (!queue_compare(queue, queue->arr[nowLocation], queue->arr[rightChildLocation], queue->cmpMode))) {
            break;
        }
        else if (queue_checkRange(rightChildLocation, queue->size)
            && queue_compare(queue, queue->arr[nowLocation], queue->arr[rightChildLocation], queue->cmpMode)
            && !queue_compare(queue, queue->arr[rightChildLocation], queue->arr[leftChildLocation], queue->cmpMode)) {
            queue_swap(&queue->arr[nowLocation], &queue->arr[rightChildLocation]);
            nowLocation = rightChildLocation;
        }
        else if (queue_checkRange(leftChildLocation, queue->size)
            && queue_compare(queue, queue->arr[nowLocation], queue->arr[leftChildLocation], queue->cmpMode)) {
            queue_swap(&queue->arr[nowLocation], &queue->arr[leftChildLocation]);
            nowLocation = leftChildLocation;
        }
        else {
            break;
        }
    }
}

struct qItem queue_front(struct queue* queue) { //Fecth first qItem from queue
    return queue->arr[1];
}

int queue_getSize(struct queue* queue) { //Fecth queue size
    return queue->size;
}



void programInit() { //Initialize program
    memset(edgeArr, 0, sizeof(edgeArr));
    memset(hasEdgeLen, 0, sizeof(hasEdgeLen));
    memset(hasEdge, 0, sizeof(hasEdge));
    memset(totalEdge, 0, sizeof(totalEdge));
    return;
}

struct queue queue[QUEUE_MAX];
int visited[NODE_MAX]; //Record visited info for bfs
void bfs(int start, int end, int id, int cmpMode) { //bfs
    queue_init(queue, cmpMode); //Init queue
    for (int i = 0;i < NODE_MAX;i++) { //Init visited to false
        visited[i] = -100;
    }

    if (nodeLimit[start] < 1 || nodeLimit[end] < 1) { //Node memory not enough
        return;
    }

    struct qItem tmp, head;
    tmp.pos = start;
    tmp.parent = -1;
    tmp.times = 1;
    tmp.weight = 0;
    queue_push(queue, tmp); //Push src to queue
    bool isArrived = false; //Is dst be found
    int ansLen = 0; 

    while (queue_getSize(queue) > 0) { //If queue is not empty
        head = queue_front(queue); //get head
        queue_pop(queue);   //Remove first qItem
        

        if (visited[head.pos] != -100) { //If is visitedS
            continue;
        }
        visited[head.pos] = head.parent;
        if (head.pos == end) { //Found dst
            isArrived = true;
            ansLen = head.times;
            break;
        }
        for (int i = 0;i < hasEdgeLen[head.pos];i++) { //Search all path can pass
            if (isArrived) {
                break;
            }
            if (edgeArr[head.pos][hasEdge[head.pos][i]] >= 1) { //If resource enough
                if (hasEdge[head.pos][i] == end && nodeLimit[end] >= 1) { //If it is dst
                    tmp.pos = end;
                    tmp.parent = head.pos;
                    tmp.times = head.times + 1;
                    if (cmpMode == 1) {
                        tmp.weight = nodeLimit[hasEdge[head.pos][i]];
                    }
                    if (cmpMode == 2) {
                        tmp.weight = edgeArr[head.pos][hasEdge[head.pos][i]];
                    }
                    if (cmpMode == 3) {
                        tmp.weight = min(nodeLimit[hasEdge[head.pos][i]], edgeArr[head.pos][hasEdge[head.pos][i]]);
                    }
                    queue_push(queue, tmp); //Push it into queue
                }
                else if (nodeLimit[hasEdge[head.pos][i]] >= 2) {//If it is not dst
                    tmp.pos = hasEdge[head.pos][i];
                    tmp.parent = head.pos;
                    tmp.times = head.times + 1;
                    if (cmpMode == 1) {
                        tmp.weight = nodeLimit[hasEdge[head.pos][i]];
                    }
                    if (cmpMode == 2) {
                        tmp.weight = edgeArr[head.pos][hasEdge[head.pos][i]];
                    }
                    if (cmpMode == 3) {
                        tmp.weight = max(nodeLimit[hasEdge[head.pos][i]] / 2, edgeArr[head.pos][hasEdge[head.pos][i]]);
                    }
                    queue_push(queue, tmp); //Push it into queue
                }
            }
        }
    }
    if (isArrived) { //If it is arrived
        finishDataArr[finishPair].id = id;
        finishDataArr[finishPair].path[ansLen - 1] = end;
        finishDataArr[finishPair].length = ansLen;
        for (int i = ansLen - 2;i >= 0;i--) { //Find path
            finishDataArr[finishPair].path[i] = visited[finishDataArr[finishPair].path[i + 1]];
        }
        for (int i = 0;i < ansLen - 1;i++) { //Sub resource
            if (i == 0) {
                nodeLimit[start] -= 1;
            }
            else {
                nodeLimit[finishDataArr[finishPair].path[i]] -= 2;
            }
            edgeArr[finishDataArr[finishPair].path[i]][finishDataArr[finishPair].path[i + 1]]--;
            edgeArr[finishDataArr[finishPair].path[i + 1]][finishDataArr[finishPair].path[i]]--;
        }
        nodeLimit[end] -= 1;
        finishPair++;
    }
}

int hasEdge_compare_base;
int hasEdge_compare(const void* _a, const void* _b) { //Compare func for edgeArr
    int* a = (int*)_a;
    int* b = (int*)_b;
    return edgeArr[hasEdge_compare_base][*b] - edgeArr[hasEdge_compare_base][*a];
}
/*
void reSort() {
    for (int i = 0;i < node;i++) {
        hasEdge_compare_base = i;
        qsort(hasEdge[i], hasEdgeLen[i], sizeof(hasEdge[0]), hasEdge_compare);
    }
}*/

void resetSource() { //Reset resource from origin 
    for (int i = 0;i < node;i++) {
        nodeLimit[i] = nodeLimitOrigin[i];
    }
    for (int i = 0;i < node;i++) {
        for (int j = 0;j < node;j++) {
            edgeArr[j][i] = edgeArrOrigin[j][i];
        }
    }
}

void save() { //Compare better ans and save
    if (finishPair > ansFinishPair - directAns) {
        for (int i = 0;i < finishPair;i++) {
            ansFinishDataArr[i + directAns] = finishDataArr[i];
        }
        ansFinishPair = finishPair + directAns;
    }
    finishPair = 0;
    memset(finishDataArr, 0, sizeof(finishDataArr));
    resetSource();//Reset resource from origin 
}


signed main() { //Main func
    programInit();
    scanf("%d%d%d", &node, &edge, &request); //Input node, edge, request
    for (int i = 0;i < node;i++) { //Input memory
        scanf("%*d%d", &nodeLimit[i]);
        nodeLimitOrigin[i] = nodeLimit[i];
    }

    int LinkID, LinkEnd1, LinkEnd2, Channels;
    for (int i = 0;i < edge;i++) { //Input channel
        scanf("%d%d%d%d", &LinkID, &LinkEnd1, &LinkEnd2, &Channels);
        edgeArr[LinkEnd1][LinkEnd2] += Channels;
        edgeArr[LinkEnd2][LinkEnd1] += Channels;
        edgeArrOrigin[LinkEnd1][LinkEnd2] = edgeArr[LinkEnd1][LinkEnd2];
        edgeArrOrigin[LinkEnd2][LinkEnd1] = edgeArr[LinkEnd2][LinkEnd1];

        hasEdge[LinkEnd1][hasEdgeLen[LinkEnd1]] = LinkEnd2;
        hasEdge[LinkEnd2][hasEdgeLen[LinkEnd2]] = LinkEnd1;
        hasEdgeLen[LinkEnd1]++;
        hasEdgeLen[LinkEnd2]++;
    }

    int ReqID, ReqSrc, ReqDst, cnt = 0;
    for (int i = 0;i < request;i++) { //Input Req
        scanf("%d%d%d", &ReqID, &ReqSrc, &ReqDst);

        if (edgeArr[ReqSrc][ReqDst] != 0 && nodeLimit[ReqSrc] >= 1 && nodeLimit[ReqDst] >= 1) { //If it is directly then choose
            ansFinishDataArr[ansFinishPair].id = ReqID;
            ansFinishDataArr[ansFinishPair].path[0] = ReqSrc;
            ansFinishDataArr[ansFinishPair].path[1] = ReqDst;
            ansFinishDataArr[ansFinishPair].length = 2;
            ansFinishPair++;
            nodeLimit[ReqSrc]--;
            nodeLimit[ReqDst]--;
            edgeArr[ReqSrc][ReqDst]--;
            edgeArr[ReqDst][ReqSrc]--;
            edgeArrOrigin[ReqSrc][ReqDst]--;
            edgeArrOrigin[ReqDst][ReqSrc]--;
            directAns++;
        }
        else {
            sdPairArr[cnt].start = ReqSrc;
            sdPairArr[cnt].end = ReqDst;
            sdPairArr[cnt].id = ReqID;
            cnt++;
        }
    }
    request = cnt;

    for (int j = 0;j < node;j++) { //Count total channel for sort
        int sum = 0;
        for (int i = 0;i < node;i++) {
            sum += edgeArr[j][i];
        }
        totalEdge[j] = sum;
    }

    for (int cmpMode = 0;cmpMode <= 3;cmpMode++) { //Use different sort to bfs
        qsort((void*)sdPairArr, request, sizeof(sdPairArr[0]), sdPair_compare_id_min);
        for (int i = 0;i < request;i++) {
            //reSort();
            bfs(sdPairArr[i].start, sdPairArr[i].end, sdPairArr[i].id, cmpMode);
        }
        save();

        qsort((void*)sdPairArr, request, sizeof(sdPairArr[0]), sdPair_compare_id_max);
        for (int i = 0;i < request;i++) {
            //reSort();
            bfs(sdPairArr[i].start, sdPairArr[i].end, sdPairArr[i].id, cmpMode);
        }
        save();

        qsort((void*)sdPairArr, request, sizeof(sdPairArr[0]), sdPair_compare);
        for (int i = 0;i < request;i++) {
            //reSort();
            bfs(sdPairArr[i].start, sdPairArr[i].end, sdPairArr[i].id, cmpMode);
        }
        save();

        qsort((void*)sdPairArr, request, sizeof(sdPairArr[0]), sdPair_compare2);
        for (int i = 0;i < request;i++) {
            //reSort();
            bfs(sdPairArr[i].start, sdPairArr[i].end, sdPairArr[i].id, cmpMode);
        }
        save();

        qsort((void*)sdPairArr, request, sizeof(sdPairArr[0]), sdPair_compare3);
        for (int i = 0;i < request;i++) {
            //reSort();
            bfs(sdPairArr[i].start, sdPairArr[i].end, sdPairArr[i].id, cmpMode);
        }
        save();

        qsort((void*)sdPairArr, request, sizeof(sdPairArr[0]), sdPair_compare4);
        for (int i = 0;i < request;i++) {
            //reSort();
            bfs(sdPairArr[i].start, sdPairArr[i].end, sdPairArr[i].id, cmpMode);
        }
        save();

        qsort((void*)sdPairArr, request, sizeof(sdPairArr[0]), sdPair_compare5);
        for (int i = 0;i < request;i++) {
            //reSort();
            bfs(sdPairArr[i].start, sdPairArr[i].end, sdPairArr[i].id, cmpMode);
        }
        save();
    }

    qsort((void*)ansFinishDataArr, ansFinishPair, sizeof(ansFinishDataArr[0]), finishData_compare); //Sort ans array
    printf("%d\n", ansFinishPair); //Print ans
    for (int j = 0;j < ansFinishPair;j++) {
        printf("%d ", ansFinishDataArr[j].id);
        for (int i = 0;i < ansFinishDataArr[j].length;i++) {
            printf("%d ", ansFinishDataArr[j].path[i]);
        }
        printf("\n");
    }

}
