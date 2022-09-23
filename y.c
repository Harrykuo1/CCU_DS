#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INF 1000000000
#define maxN 1000
#define maxR 1000 // defining some useful constants

int n, l, r; // number of nodes, number of links, number of requests
int M[maxN + 5]; // array to store memory
int path[maxN + 5]; // array to store current shortest path
int G[maxN + 5][maxN + 5]; // array to store the graph

int reqPath[maxR + 5][maxN + 5];
int reqId[maxR + 5]; // array for recording result

int dis[maxN + 5]; // array to compute the distance of shortest path
int dp[maxN + 5]; // for any i that dp[i] == 1, implies that I can reach the destination from point i.
int Q[maxN * maxN + 5]; // simulating queue

void dfs(int now) { // a function to fill in the dp array
	for(int i = 0; i < n; i ++) {
		if(dp[i]) {
			continue;
		}
		if(dis[i] == dis[now] - 1 && G[i][now] > 0) {
			dp[i] = 1;
			dfs(i);
		}
	}
}

int check(int from, int to) { // array to compute the shortest path and check if it can fit in
	int top = 0, bottom = 0; // variables for maintaining queue
	Q[bottom ++] = from;
	for(int i = 0; i < n; i ++) {
		dis[i] = INF;
	} // initial the dis array
	dis[from] = 0;
	while(top < bottom) { // simple bfs for shortest path
		int t = Q[top ++];
		for(int i = 0; i < n; i ++) {
			if(dis[i] != INF) {
				continue;
			}
			if(G[t][i] <= 0) {
				continue;
			}
			dis[i] = dis[t] + 1;
			Q[bottom ++] = i;
		}
	}
	if(dis[to] == INF) {
		return 0;
	} // cannot reach the destination
	memset(dp, 0, sizeof(dp));
	dp[to] = 1;
	dfs(to); // fill in the dp array
	int now = from;
	int m = 0;
	while(now != to) {
		path[m ++] = now;
		for(int i = 0; i < n; i ++) {
			if(G[now][i] && dp[i] == 1 && dis[i] == dis[now] + 1) {
				now = i;
				break;
			}
		}
	} // construct a shortest path with the smallest lexicological order
	path[m ++ ] = to;
	path[m] = -1;
	for(int i = 0; i < m; i ++) {
		if(i == 0 || i == m - 1) {
			if(M[path[i]] < 1) {
				return 0;
			}
		}
		else {
			if(M[path[i]] < 2) {
				return 0;
			}
		}
		if(i != m - 1 && G[path[i]][path[i + 1]] < 1) {
			return 0;
		}
	} // check if the constructed shortest path can fit in
	for(int i = 0; i < m; i ++) {
		if(i == 0 || i == m - 1) {
			M[path[i]] --;
		}
		else {
			M[path[i]] -= 2;
		}
		if(i != m - 1) {
			G[path[i]][path[i + 1]] --;
			G[path[i + 1]][path[i]] --;
		}
	} // if able to fit in, take of the resources used.
	return 1;
}

int main() {
	scanf("%d%d%d", &n, &l, &r); // input n, l, r
	for(int i = 0; i < n; i ++) {
		int x, y;
		scanf("%d%d", &x, &y);
		M[x] = y;
	} // input for memories
	for(int i = 0; i < l; i ++) {
		int id, x, y, z;
		scanf("%d%d%d%d", &id, &x, &y, &z);
		G[x][y] = G[y][x] = z;
	} // input for graph
	int res = 0;
	for(int i = 0; i < r; i ++) {
		int id, x, y;
		scanf("%d%d%d", &id, &x, &y); // input input for requests
		if(check(x, y) == 1) { // check for the (S, D)
			for(int j = 0; ; j ++) { // marked a -1 at the end of the array for convinence
				if(path[j] == -1) {
					reqPath[res][j] = -1;
					break;
				}
				reqPath[res][j] = path[j];
			}
			reqId[res ++] = id; // recording the result
		}
	}
	printf("%d\n", res);
	for(int i = 0; i < res; i ++) {
		printf("%d ", reqId[i]);
		for(int j = 0; ; j ++) {
			if(reqPath[i][j] == -1) {
				break;
			}
			printf("%d ", reqPath[i][j]);
		}
		printf("\n");
	} // output
	return 0;
}
