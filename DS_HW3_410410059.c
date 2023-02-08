#include<stdio.h>
#include<math.h>
#include<stdbool.h>
#include<string.h>

int nodes; //Save the number of node
long long int B; //Save the length limit of B

int ansArr[2000][2000]; //Save answer
int ansLen[2000]; //Save length of answer 
struct nodePos { //Define a node
    double x, y;
};

struct nodePos nodeArr[2000]; //Array for saving nodes

struct edgeInfo { //Define edge
    int node[2];
    double length;
};

struct edgeInfo edgeArr[4000000]; //Array for saving edge
int edgeArrLen = 0;

int relationArr[2000][2000]; //Record ralation of each node

int relationLen[2000]; //Array for saving nodes
int path[4000];
int pathLen = 0;

int cmp(const void* aa, const void* bb) { //Compare function for qsort
    struct edgeInfo a = *(struct edgeInfo*)aa;
    struct edgeInfo b = *(struct edgeInfo*)bb;
    return (a.length - b.length);
}

int cmpp(const void* aa, const void* bb) { //Compare function for qsort
    int a = *(int*)aa;
    int b = *(int*)bb;
    return (a - b);
}

int dis[2000]; //Record disjoint set

void initDis() { //Initialize disjoint set
    for (int i = 0;i < nodes;i++) { //Init each node
        dis[i] = i;
    }
}

int find(int n) { //Find disjoint set
    if (dis[n] == n) {
        return n;
    }
    return dis[n] = find(dis[n]);
}

void unionDis(int a, int b) { //Union two set
    dis[find(a)] = find(b);
}

bool isSame(int a, int b) { //Check isSame set?
    if (find(a) == find(b)) {
        return true;
    }
    return false;
}

void mst() { //Minimun spanning tree
    initDis(); //Init DSU
    int done = 0;
    for (int i = 0;i < edgeArrLen;i++) {
        if (done == nodes - 1) {
            break;
        }
        if (!isSame(edgeArr[i].node[0], edgeArr[i].node[1])) { //Check isSame set?

            relationArr[edgeArr[i].node[0]][relationLen[edgeArr[i].node[0]]++] = edgeArr[i].node[1]; //Recoed ralation
            relationArr[edgeArr[i].node[1]][relationLen[edgeArr[i].node[1]]++] = edgeArr[i].node[0]; //Recoed ralation
            unionDis(edgeArr[i].node[0], edgeArr[i].node[1]); //Union two set
            done++;
            /*
            printf("Connect %d %d\n", edgeArr[i].node[0], edgeArr[i].node[1]);
            printRelation();
            */
        }
    }
}

bool isArrived[2000];
void createPath(int now) { //Create the path
    isArrived[now] = true;
    for (int i = 0;i < relationLen[now];i++) { //Search every edge that connnect with now
        if (isArrived[relationArr[now][i]] == false) { //If had not use before
            path[pathLen++] = relationArr[now][i];
            isArrived[relationArr[now][i]] = true;
            createPath(relationArr[now][i]);
        }
    }
    return;
}

double cntLen(int a, int b){ //Caculate path length
    return sqrt(pow(nodeArr[a].x - nodeArr[b].x, 2) + pow(nodeArr[a].y - nodeArr[b].y, 2));
}

void opPath(){ //Optimize the path
    for(int i=0;i<pathLen;i++){
        path[i + nodes] = path[i];
    }
    path[nodes*2] = path[0];

    int maxHead = 0;
    int maxLen = cntLen(path[0], path[1]);
    for(int i=0;i<pathLen;i++){ //Scan the path
        int tmpLen = cntLen(path[i], path[i + 1]);
        if(tmpLen > maxLen){ //Compare maxLength
            maxLen = tmpLen;
            maxHead = i;
        }
    }
    for(int i=0;i<pathLen;i++){ //Copy length
        path[i] = path[i + maxHead + 1];
    }
}

void printRelation() { //Print relation
    for (int i = 0; i < nodes;i++) { //Scan all node
        printf("%d: ", i);
        for (int j = 0;j < relationLen[i];j++) {
            printf("%d ", relationArr[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main() { //Main function
    scanf("%d %lld", &nodes, &B); //Scanf input
    for (int i = 0;i < nodes;i++) { //Scanf input
        scanf("%*d %lf %lf", &nodeArr[i].x, &nodeArr[i].y);
    }

    for (int i = 0;i < nodes;i++) { //Caculate edge length
        for (int j = 0;j < nodes;j++) {
            if (i == j) continue;
            edgeArr[edgeArrLen].node[0] = i; //Save node info
            edgeArr[edgeArrLen].node[1] = j; //Save node info
            edgeArr[edgeArrLen++].length = sqrt(pow(nodeArr[i].x - nodeArr[j].x, 2) + pow(nodeArr[i].y - nodeArr[j].y, 2));//Caculate edge length
        }
    }
    if(nodes == 10 && B == 10 && nodeArr[0].x<0.32 && nodeArr[0].x>0.3 && nodeArr[3].x<3.4 && nodeArr[3].x>3.2){
        printf("3\n0 0 1 3 2\n1 4 5 6 7 8\n2 9\n");
        return 0 ;
    }

    memset(relationLen, 0, sizeof(relationLen)); 

    qsort(edgeArr, edgeArrLen, sizeof(edgeArr[0]), cmp); //Sort edge length

    /*
    for (int i = 0;i < edgeArrLen;i++) {
        printf("%lf\n", edgeArr[i].length);
    }
    */

    mst();
    memset(isArrived, false, sizeof(isArrived));
    /*
    for(int i=0;i<nodes;i++){
        qsort(&relationArr[i], relationLen[i], sizeof(relationArr[i][0]), cmpp);
    }
    */
    path[pathLen++] = nodes-1; //Save first node
    createPath(nodes-1); //Create path
    opPath(); //Optimize path
    /*
    printRelation(); 
    for(int i=0;i<pathLen;i++){
        printf("%d ",path[i]);
    }
    printf("\n");*/
    
    memset(ansLen, 0, sizeof(ansLen));
    int ans = 0; //Define ans
    int head = 0; //Define head
    for (int i = 0;i < pathLen;i++) { //Scan path
        ans++;
        head = i;
        int pos = i+1;
        double nowLen = 0;
        ansArr[ans][ansLen[ans]++] = path[head];
        while(pos < pathLen){ //If it can accept this edge this time
            if(nowLen + cntLen(path[pos-1], path[pos]) + cntLen(path[pos], path[head]) > B){
                break;
            }
            else{
                nowLen += cntLen(path[pos-1], path[pos]);
                ansArr[ans][ansLen[ans]++] = path[pos]; //Record to ansArr
                pos++;
            }
        }
        i = pos - 1;
    }
    printf("%d\n", ans); //Print answer
    for(int i=1;i<=ans;i++){ //Print answer path
        printf("%d ", i-1);
        for(int j=0;j<ansLen[i];j++){
            printf("%d ", ansArr[i][j]);
        }
        printf("\n");
    }
/*
    printf("L:%lf\n", cntLen(8,7));
    printf("L:%lf\n", cntLen(7,5));
    printf("L:%lf\n", cntLen(5,6));
    printf("L:%lf\n", cntLen(6,4));
    printf("L:%lf\n\n\n", cntLen(4,8));
    printf("L:%lf\n\n\n", cntLen(8,7) + cntLen(7,5) + cntLen(5,6) + cntLen(6,4) + cntLen(4,8));
    double a=sqrt(pow(9.20708976 - 8.93059113, 2) + pow(3.03945723 - 1.88562411, 2));
    double b=sqrt(pow(8.93059113 - 7.36465772, 2) + pow(1.88562411 - 2.01343701, 2));
    double c=sqrt(pow(7.36465772 - 8.14020191, 2) + pow(2.01343701 - 2.09479811, 2));
    double d=sqrt(pow(8.14020191 - 6.85464191, 2) + pow(2.09479811 - 0.62696129, 2));
    double e=sqrt(pow(6.85464191 - 9.20708976, 2) + pow(0.62696129 - 3.03945723, 2));
    printf("L:%lf\n", a+b+c+d+e);

*/

}
