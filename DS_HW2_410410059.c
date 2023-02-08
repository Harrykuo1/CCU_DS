#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

struct ansItem {
    int block_type; //Record block type
    int x, y;
};
int ans_pair = 0;   //Record ans pair
int ans_pair_tmp = 0; //Record ans pair temporary
struct ansItem ans_arr[10000]; //Record ans
struct ansItem ans_arr_tmp[10000]; //Record ans  temporary
bool sel[10000];    //Record what we select
bool sel_tmp[10000]; //Record what we select temporary


int yLimit, xLimit, num;  //Record ResourceY ResourceX Users
int xNow, x, y;
int tmp, tmpY, tmpX;
char input[10000000]; //Save input
int block[10000][20];   //Record block type for id
int block_origin[10000][20]; //Record block type for id

int block_list[10][10000]; //Record id for block type 
int block_list_origin[10][10000]; //Record id for block type 
int block_list_front[10];  
int block_list_length[10]; //Record block list's length
int block_list_canuse[10]; //Record how many block we have


void reset_block() {    //reset block array
    for (int j = 0;j < num;j++) {
        for (int i = 0;i <= 6;i++) {
            block[j][i] = block_origin[j][i];
        }
    }
}

void reset_block_list() { //reset block_list array
    for (int j = 1;j <= 5;j++) {
        for (int i = 0;i <= block_list_length[j];i++) {
            block_list[j][i] = block_list_origin[j][i];
        }
    }
}

void save_ans() {   //update answer
    if (ans_pair_tmp > ans_pair) { //if it is greater than origin, then update
        for (int i = 0;i < num;i++) {
            ans_arr[i].x = ans_arr_tmp[i].x;
            ans_arr[i].y = ans_arr_tmp[i].y;
            ans_arr[i].block_type = ans_arr_tmp[i].block_type;
            sel[i] = sel_tmp[i];
        }
        ans_pair = ans_pair_tmp;
        ans_pair_tmp = 0;
    }
    reset_block(); //reset block
    reset_block_list(); //reset block list
    memset(sel_tmp, false, sizeof(sel));
    for (int i = 0;i < 10;i++) {
        block_list_canuse[i] = block_list_length[i];
        block_list_front[i] = 0;
    }
}

int compare(const void* aa, const void* bb) { //Compare function for qsort
    int a = *(int*)aa;
    int b = *(int*)bb;
    if (block_origin[a][6] == block_origin[b][6]) { //if number of block is same
        if (yLimit < xLimit) {
            for (int i = 1;i <= 5;i++) {
                if (block_origin[a][i] != block_origin[b][i]) {
                    return (block_origin[b][6] - block_origin[a][6]);
                }
            }
        }
        else {
            for (int i = 5;i >= 1;i--) {
                if (block_origin[a][i] != block_origin[b][i]) {
                    return (block_origin[b][6] - block_origin[a][6]);
                }
            }
        }
    }
    return (block_origin[a][6] - block_origin[b][6]);
}

void subResource(int selNum) {// Substract our resource 
    for (int i = 1;i <= 5;i++) {
        if (block[selNum][i]) {
            block_list_canuse[i]--;
        }
    }
}

int fetch(int fetchBlockType) { // fetch id for block list
    int selNum, head;
    while (1) {
        head = block_list_front[fetchBlockType];
        selNum = block_list[fetchBlockType][head];
        if (sel_tmp[selNum] == false) {
            subResource(selNum);
            return selNum;
        }
        block_list_front[fetchBlockType]++;
    }
}


int hightest[10000]; //Record highest position

void addHigh(int start, int length, int num){ //Update highest array
    for(int i=0;i<length;i++){
        hightest[start + i]+= num;
    }
}


int cnt = 0; 
bool hasAdd = true;
void solve1(){ //Solve way 1(bottom to up)
    memset(hightest,0, sizeof(hightest)); //Reset highest array

    hasAdd = true;
    while(block_list_canuse[1] > 0 && hasAdd){ //Put 1x16 block
        cnt = 0;
        for(int i=0;i<=xLimit-16;i+=16){    //scan 0~xlimit
            if(hightest[i]+1 <= yLimit && block_list_canuse[1] > 0){ //If there are still blocks and there is enough space
                cnt++;
                int selNum = fetch(1); //Fetch block id
                sel_tmp[selNum] = true;
                ans_arr_tmp[selNum].block_type = 1;
                ans_arr_tmp[selNum].x = i;
                ans_arr_tmp[selNum].y = hightest[i];
                addHigh(i, 16, 1);   //Update highest array
                ans_pair_tmp++;
            }
        }
        if(cnt==0){
            hasAdd=false;
        }
    }

    hasAdd = true;
    while(block_list_canuse[2] > 0 && hasAdd){//Put 2x8 block
        cnt = 0;
        for(int i=0;i<=xLimit-8;i+=8){ //scan 0~xlimit
            if(hightest[i]+2 <= yLimit && block_list_canuse[2] > 0){//If there are still blocks and there is enough space
                cnt++;
                int selNum = fetch(2);  //Fetch block id
                sel_tmp[selNum] = true;
                ans_arr_tmp[selNum].block_type = 2;
                ans_arr_tmp[selNum].x = i;
                ans_arr_tmp[selNum].y = hightest[i];
                addHigh(i, 8, 2);   //Update highest array
                ans_pair_tmp++;
            }
        }
        if(cnt==0){
            hasAdd=false;
        }
    }

    hasAdd = true;
    while(block_list_canuse[3] > 0 && hasAdd){//Put 4x4 block
        cnt = 0;
        for(int i=0;i<=xLimit-4;i+=4){//scan 0~xlimit
            if(hightest[i]+4 <= yLimit && block_list_canuse[3] > 0){//If there are still blocks and there is enough space
                cnt++;
                int selNum = fetch(3);  //Fetch block id
                sel_tmp[selNum] = true;
                ans_arr_tmp[selNum].block_type = 3;
                ans_arr_tmp[selNum].x = i;
                ans_arr_tmp[selNum].y = hightest[i];
                addHigh(i, 4, 4);   //Update highest array
                ans_pair_tmp++;
            }
        }
        if(cnt==0){
            hasAdd=false;
        }
    }

    hasAdd = true;
    while(block_list_canuse[4] > 0 && hasAdd){//Put 8x2 block
        cnt = 0;
        for(int i=0;i<=xLimit-2;i+=2){//scan 0~xlimit
            if(hightest[i]+8 <= yLimit && block_list_canuse[4] > 0){//If there are still blocks and there is enough space
                cnt++;
                int selNum = fetch(4);  //Fetch block id
                sel_tmp[selNum] = true;
                ans_arr_tmp[selNum].block_type = 4;
                ans_arr_tmp[selNum].x = i;
                ans_arr_tmp[selNum].y = hightest[i];
                addHigh(i, 2, 8);   //Update highest array
                ans_pair_tmp++;
            }
        }
        if(cnt==0){
            hasAdd=false;
        }
    }

    hasAdd = true;
    while(block_list_canuse[5] > 0 && hasAdd){//Put 16x1 block
        cnt = 0;
        for(int i=0;i<=xLimit-1;i+=1){//scan 0~xlimit
            if(hightest[i]+16 <= yLimit && block_list_canuse[5] > 0){//If there are still blocks and there is enough space
                cnt++;
                int selNum = fetch(5);  //Fetch block id
                sel_tmp[selNum] = true;
                ans_arr_tmp[selNum].block_type = 5;
                ans_arr_tmp[selNum].x = i;
                ans_arr_tmp[selNum].y = hightest[i];
                addHigh(i, 1, 16);   //Update highest array
                ans_pair_tmp++;
            }
        }
        if(cnt==0){
            hasAdd=false;
        }
    }
    save_ans();
}

void solve2(){
    memset(hightest,0, sizeof(hightest));

    hasAdd = true;
    while(block_list_canuse[5] > 0 && hasAdd){//Put 16x1 block
        cnt = 0;
        for(int i=0;i<=yLimit-16;i+=16){//scan 0~xlimit
            if(hightest[i]+1 <= xLimit && block_list_canuse[5] > 0){//If there are still blocks and there is enough space
                cnt++;
                int selNum = fetch(5);  //Fetch block id
                sel_tmp[selNum] = true;
                ans_arr_tmp[selNum].block_type = 5;
                ans_arr_tmp[selNum].y = i;
                ans_arr_tmp[selNum].x = hightest[i];
                addHigh(i, 16, 1);   //Update highest array
                ans_pair_tmp++;
            }
        }
        if(cnt==0){
            hasAdd=false;
        }
    }

    hasAdd = true;
    while(block_list_canuse[4] > 0 && hasAdd){//Put 8x2 block
        cnt = 0;
        for(int i=0;i<=yLimit-8;i+=8){//scan 0~xlimit
            if(hightest[i]+2 <= xLimit && block_list_canuse[4] > 0){//If there are still blocks and there is enough space
                cnt++;
                int selNum = fetch(4);  //Fetch block id
                sel_tmp[selNum] = true;
                ans_arr_tmp[selNum].block_type = 4;
                ans_arr_tmp[selNum].y = i;
                ans_arr_tmp[selNum].x = hightest[i];
                addHigh(i, 8, 2);   //Update highest array
                ans_pair_tmp++;
            }
        }
        if(cnt==0){
            hasAdd=false;
        }
    }

    hasAdd = true;
    while(block_list_canuse[3] > 0 && hasAdd){//Put 4x4 block
        cnt = 0;
        for(int i=0;i<=yLimit-4;i+=4){//scan 0~xlimit
            if(hightest[i]+4 <= xLimit && block_list_canuse[3] > 0){//If there are still blocks and there is enough space
                cnt++;
                int selNum = fetch(3);  //Fetch block id
                sel_tmp[selNum] = true;
                ans_arr_tmp[selNum].block_type = 3;
                ans_arr_tmp[selNum].y = i;
                ans_arr_tmp[selNum].x = hightest[i];
                addHigh(i, 4, 4);   //Update highest array
                ans_pair_tmp++;
            }
        }
        if(cnt==0){
            hasAdd=false;
        }
    }

    hasAdd = true;
    while(block_list_canuse[2] > 0 && hasAdd){ //Put 2x8 block
        cnt = 0;
        for(int i=0;i<=yLimit-2;i+=2){//scan 0~xlimit
            if(hightest[i]+8 <= xLimit && block_list_canuse[2] > 0){//If there are still blocks and there is enough space
                cnt++;
                int selNum = fetch(2);  //Fetch block id
                sel_tmp[selNum] = true;
                ans_arr_tmp[selNum].block_type = 2;
                ans_arr_tmp[selNum].y = i;
                ans_arr_tmp[selNum].x = hightest[i];
                addHigh(i, 2, 8);   //Update highest array
                ans_pair_tmp++;
            }
        }
        if(cnt==0){
            hasAdd=false;
        }
    }

    hasAdd = true;
    while(block_list_canuse[1] > 0 && hasAdd){ //Put 1x16 block
        cnt = 0;
        for(int i=0;i<=yLimit-1;i+=1){//scan 0~xlimit
            if(hightest[i]+16 <= xLimit && block_list_canuse[1] > 0){//If there are still blocks and there is enough space
                cnt++;
                int selNum = fetch(1);  //Fetch block id
                sel_tmp[selNum] = true;
                ans_arr_tmp[selNum].block_type = 1;
                ans_arr_tmp[selNum].y = i;
                ans_arr_tmp[selNum].x = hightest[i];
                addHigh(i, 1, 16);   //Update highest array
                ans_pair_tmp++;
            }
        }
        if(cnt==0){
            hasAdd=false;
        }
    }
    save_ans();    //Update answer
}




void print_ans() {  //Print answer
    char type[100];

    printf("%d\n", ans_pair);  //Print answer pair
    for (int i = 0;i < num;i++) { //Scan id
        if (sel[i]) {
            switch (ans_arr[i].block_type) {
            case 1:
                strcpy(type, "1x16");
                break;
            case 2:
                strcpy(type, "2x8");
                break;
            case 3:
                strcpy(type, "4x4");
                break;
            case 4:
                strcpy(type, "8x2");
                break;
            case 5:
                strcpy(type, "16x1");
                break;
            }
            printf("%d %s %d %d\n", i, type, ans_arr[i].y, ans_arr[i].x);
        }
    }
}

int main() {
    memset(block, 0, sizeof(block));
    scanf("%d%d%d\n", &yLimit, &xLimit, &num); //Input yLimit xLimit num
    const char s1[2] = " ";
    char* token;
    for (int i = 0;i < num;i++) {
        gets(input);    //Input string
        token = strtok(input, s1);  //Split string
        token = strtok(NULL, s1);  //Split string
        while (token != NULL) {
            sscanf(token, "%dx%d", &tmpY, &tmpX);  //Save format to tmpY x tmpX
            block_origin[i][0] = i;
            block_origin[i][6] ++;
            switch (tmpY) {
            case 1:    //Block is 1x16
                block_origin[i][1] = 1;
                block_list_origin[1][block_list_length[1]++] = i;
                break;
            case 2:    //Block is 2x8
                block_origin[i][2] = 1;
                block_list_origin[2][block_list_length[2]++] = i;
                break;
            case 4:    //Block is 4x4
                block_origin[i][3] = 1;
                block_list_origin[3][block_list_length[3]++] = i;
                break;
            case 8:    //Block is 8x2
                block_origin[i][4] = 1;
                block_list_origin[4][block_list_length[4]++] = i;
                break;
            case 16:    //Block is 16x1
                block_origin[i][5] = 1;
                block_list_origin[5][block_list_length[5]++] = i;
                break;
            }
            token = strtok(NULL, s1);    //Split string
        }
    }
    for (int i = 1;i <= 5;i++) {
        qsort(&block_list_origin[i], block_list_length[i], sizeof(block_list_origin[i][0]), compare);//Sort block list
    }
    save_ans();//Init answer
    solve1();
    solve2(); 
    print_ans();//Print answer
}