#include<stdio.h>
#include<stdbool.h>

#define TEST 0 //Set TEST

struct node { //Define node
    unsigned long value;
    char* string;
    bool hasLeft;
    bool hasRight;
    bool isCorrect;
};
struct node* tree; //Create tree
int n, strLen, layer, nTotal, leafHead, ans=0;

int myPow(int base, int power) { //Count power
    int ans = 1;
    for (int i = 0;i < power;i++) {
        ans *= base;
    }
    return ans;
}

unsigned long MurmurOAAT32(char* key) { //Count hash value
    unsigned long h = 3323198485ul;
    for (;*key;++key) {
        h ^= *key;
        h *= 0x5bd1e995;
        h ^= h >> 15;
    }
    return h;
}

char* NumToStr(unsigned long num) { //Convert num to string
    char* s = malloc(sizeof(char) * (strLen + 100));
    memset(s, 0, sizeof(s));
    sprintf(s, "%lu", num);
    return s;
}

unsigned long hash(unsigned long num) { //Count hash
    char* key = NumToStr(num);
    unsigned long ans = MurmurOAAT32(key);
    free(key);
    return ans;
}

void init_tree() { //Init tree
    for (int i = 0;i < nTotal;i++) {
        tree[i].value = 0ul;
        tree[i].hasLeft = false;
        tree[i].hasRight = false;
        tree[i].isCorrect = true;
    }
}

void input_data() { //Input data
    for (int i = leafHead;i < nTotal;i++) {
        tree[i].string = (char*)malloc((strLen + 1) * sizeof(char));
        scanf("%s", tree[i].string);
    }
}

void hash_leaf() { //Hash leaf
    for (int i = leafHead;i < nTotal;i++) {
        tree[i].value = MurmurOAAT32(tree[i].string);
    }
}

void set_tree_status() { //set_tree_status
    for (int i = leafHead;i < nTotal;i++) {
        int now = i, parent;
        do {
            parent = (now - 1) / 2;
            if (parent * 2 + 1 == now) {
                tree[parent].hasLeft = true;
            }
            else {
                tree[parent].hasRight = true;
            }
            now = parent;
        } while (now != 0);
    }
}

void build_tree(int now) { //Build tree
    if (now >= nTotal) {
        return;
    }
    if (!tree[now].hasLeft && !tree[now].hasRight) {
        return;
    }
    else if (tree[now].hasLeft && !tree[now].hasRight) { //Only left
        build_tree(now * 2 + 1);
        tree[now].value = hash(tree[now * 2 + 1].value * 2);
    }
    else if (tree[now].hasLeft && tree[now].hasRight) { //Have left and right
        build_tree(now * 2 + 1);
        build_tree(now * 2 + 2);
        tree[now].value = hash(tree[now * 2 + 1].value + tree[now * 2 + 2].value);
    }
    return;
}

void ask_tree(int deep, int index, int num) { //Ask question
    if (num >= nTotal) {
        return;
    }
    if(!TEST){
        printf("1 %d %d\n", deep, index);
    }
    else{
        printf("1 %d %d %d\n", deep, index, num);
    }
    
    fflush(NULL);

    unsigned long askValue;
    scanf("%lu", &askValue);
    if (tree[num].value != askValue) {
        tree[num].isCorrect = false;
        if(tree[num].hasLeft){
            ask_tree(deep + 1, index * 2, num * 2 + 1);
        }
        if(tree[num].hasRight){
            ask_tree(deep + 1, index * 2 + 1, num * 2 + 2);
        }
    }

}



int main() {
    scanf("%d%d", &n, &strLen);
    layer = 0;
    while (n > myPow(2, layer)) {
        layer++;
    }
    leafHead = myPow(2, layer) - 1;
    nTotal = myPow(2, layer) + n - 1;
    tree = (struct node*)malloc(myPow(2, layer + 1) * sizeof(struct node));
    memset(tree, 0, sizeof(tree));

    init_tree();

    input_data();

    hash_leaf();

    set_tree_status();
    if (TEST) {
        printf("\n");
        for (int i = 0;i < nTotal;i++) {
            printf("%d %c %c\n", i, "YN"[tree[i].hasLeft == true ? 0 : 1], "YN"[tree[i].hasRight == true ? 0 : 1]);
        }
    }

    build_tree(0);
    if (TEST) {
        printf("\n");
        for (int i = 0;i < nTotal;i++) {
            printf("%d %lu\n", i, tree[i].value);
        }
    }

    ask_tree(0, 0, 0);

    for(int i=leafHead;i<nTotal;i++){
        if(!tree[i].isCorrect){
            ans++;
        }
    }

    printf("2 %d\n",ans);
    for(int i=leafHead;i<nTotal;i++){
        if(!tree[i].isCorrect){
            printf("%s\n",tree[i].string);
        }
    }

}