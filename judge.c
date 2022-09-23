#include<stdio.h>

int nodeLimit[1005];
int edgeArr[1005][1005];

struct stPair {
    int src, dst;
};

struct stPair stArr[1005];

int main() {
    int node, edge, request;
    scanf("%d%d%d", &node, &edge, &request);
    for (int i = 0;i < node;i++) {
        scanf("%*d%d", &nodeLimit[i]);
    }

    int LinkID, LinkEnd1, LinkEnd2, Channels;
    for (int i = 0;i < edge;i++) {
        scanf("%d%d%d%d", &LinkID, &LinkEnd1, &LinkEnd2, &Channels);
        edgeArr[LinkEnd1][LinkEnd2] += Channels;
        edgeArr[LinkEnd2][LinkEnd1] += Channels;
    }

    int ReqID, ReqSrc, ReqDst, cnt = 0;
    for (int i = 0;i < request;i++) {
        scanf("%d%d%d", &ReqID, &ReqSrc, &ReqDst);
        stArr[i].src = ReqSrc;
        stArr[i].dst = ReqDst;
    }
}