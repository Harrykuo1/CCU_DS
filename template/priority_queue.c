#include<stdio.h>
#include<stdbool.h>

#define int long long

//setting
#define PRIORITY_MAX_LENGTH 1000005

struct edge {
    int start;
    int end;
    int weight;
};

struct priority_queue {
    struct edge list[PRIORITY_MAX_LENGTH];
    int size;
    int mode;
};

void init(struct priority_queue* priority_Q) {
    priority_Q->size = 0;
}

void swap(struct edge* edge1, struct edge* edge2) {
    struct edge tmp = *edge1;
    *edge1 = *edge2;
    *edge2 = tmp;
}

bool compare(struct priority_queue priority_Q, struct edge edge1, struct edge edge2) {
    if (edge1.weight == edge2.weight) {
        return false;
    }
    else if (edge1.weight < edge2.weight) {
        return priority_Q.mode == 1 ? false : true;
    }
    else {
        return priority_Q.mode == 1 ? true : false;
    }
}

void insert(struct priority_queue* priority_Q, struct edge newEdge) {
    priority_Q->size++;
    priority_Q->list[priority_Q->size] = newEdge;
    int nowLocation = priority_Q->size;
    while (nowLocation > 1) {
        int parentLocation = nowLocation / 2;
        if (compare(*priority_Q, priority_Q->list[parentLocation], priority_Q->list[nowLocation])) {
            swap(&priority_Q->list[parentLocation], &priority_Q->list[nowLocation]);
            nowLocation /= 2;
        }
        else {
            break;
        }

    }
    return;
}

struct edge top(struct priority_queue priority_Q) {
    return priority_Q.list[1];
}

bool checkRange(int n, int limit) {
    if (n <= limit) {
        return true;
    }
    else {
        return false;
    }
}

void pop(struct priority_queue* priority_Q) {
    swap(&priority_Q->list[1], &priority_Q->list[priority_Q->size]);
    priority_Q->size--;
    int nowLocation = 1;
    while (nowLocation < priority_Q->size) {
        int leftChildLocation = nowLocation * 2;
        int rightChildLocation = nowLocation * 2 + 1;
        if (checkRange(leftChildLocation, priority_Q->size)
            && checkRange(rightChildLocation, priority_Q->size)
            && (!compare(*priority_Q, priority_Q->list[nowLocation], priority_Q->list[leftChildLocation]))
            && (!compare(*priority_Q, priority_Q->list[nowLocation], priority_Q->list[rightChildLocation]))) {
            break;
        }
        else if (checkRange(rightChildLocation, priority_Q->size)
            && compare(*priority_Q, priority_Q->list[nowLocation], priority_Q->list[rightChildLocation])
            && !compare(*priority_Q, priority_Q->list[rightChildLocation], priority_Q->list[leftChildLocation])) {
            swap(&priority_Q->list[nowLocation], &priority_Q->list[rightChildLocation]);
            nowLocation = rightChildLocation;
        }
        else if (checkRange(leftChildLocation, priority_Q->size)
            && compare(*priority_Q, priority_Q->list[nowLocation], priority_Q->list[leftChildLocation])) {
            swap(&priority_Q->list[nowLocation], &priority_Q->list[leftChildLocation]);
            nowLocation = leftChildLocation;
        }
        else {
            break;
        }
    }
}

void show(struct priority_queue* priority_Q) {
	/*
    for (int i = 1;i <= priority_Q->size;i++) {
        printf("%lld ", priority_Q->list[i].weight);
    }
    printf("\n");
	*/
    while (priority_Q->size > 0) {
        printf("%lld ", top(*priority_Q).weight);
        pop(priority_Q);
    }
    return;
}

struct priority_queue priority_Q;

signed main() {
    init(&priority_Q);
    priority_Q.mode = 1;

    struct edge tmp;

    int n, num;
    scanf("%lld", &n);
    for (int i = 0;i < n;i++) {
        scanf("%lld", &num);
        tmp.weight = num;
        insert(&priority_Q, tmp);
    }
    show(&priority_Q);

    return 0;
}