#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>

#define MAXR 100
#define MAXC 100
#define MAXINDEX MAXR * MAXC

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3
#define DIR 4

#define VISITED 0
#define CLOSED 1
#define STATUS 2

#define UPPER 0
#define LOWER 1

typedef struct {
    int Rank;
    int Bread;
} Bakery; // for T

typedef struct Node{
    int rank,Bread;
    struct Node *vt, *hr, *wormhole;
} Node;

Node *genPairNode(int rank, int bread)
{
    Node *upper = (Node *)malloc(sizeof(Node));
    Node *lower = (Node *)malloc(sizeof(Node));
    upper->hr = NULL, upper->vt = NULL, lower->hr = NULL, lower->vt = NULL;
    upper->rank = rank,upper->Bread = bread;
    lower->rank = rank,lower->Bread = bread;
    upper->wormhole = lower,lower->wormhole = upper;
    return upper;
}

Node *find_corner(Node *center, int r)
{
    Node *ptr = center;
    for (int i = 0; i < r; i++){
        ptr = ptr->hr->vt;
    }
    return ptr;
}

void put_to_array(int rank_to_index[MAXINDEX], Bakery map[MAXR][MAXC], Node *head, int R, int C, int d_dis[DIR][STATUS][MAXINDEX])
{
    Node *tmp_head = head;
    assert(head->vt != NULL);
    Node *ptr = head;
    int index = 0;
    while (ptr != NULL){
        map[index / C][index % C].Bread = ptr->Bread, map[index / C][index % C].Rank = ptr->rank;
        rank_to_index[ptr->rank] = index;
        for (int dir = 0; dir < DIR; dir++){
            d_dis[dir][CLOSED][index] = 0;
        }
        if (ptr->wormhole->vt == NULL){
            d_dis[UP][CLOSED][index] = MAXINDEX;
        }
        if (ptr->hr == NULL){
            d_dis[RIGHT][CLOSED][index] = MAXINDEX;
        }
        if (ptr->vt == NULL){
            d_dis[DOWN][CLOSED][index] = MAXINDEX;
        }
        if (ptr->wormhole->hr == NULL){
            d_dis[LEFT][CLOSED][index] = MAXINDEX;
        }
        
        if (ptr->hr != NULL){
            ptr = ptr->hr;
        }
        else{
            ptr = tmp_head->vt;
            tmp_head = tmp_head->vt;
        }
        index++;
    }
    return;
}

bool in_map(int R, int C, int nowR, int nowC)
{
    if (nowR < R && nowR >= 0 && nowC < C && nowC >= 0){
        return 1;
    }
    return 0;
}

void init_ddis(Bakery map[MAXR][MAXC], int d_dis[DIR][STATUS][MAXINDEX], int R, int C)
{
    for (int r = 0; r < R; r++){
        for (int c = 0; c < C; c++){
            int nowIndex = r * C + c;
            if (map[r][c].Bread > 0){
                continue;
            }
            int tmp_r = -1, tmp_c = -1;
            if (in_map(R, C, r - (1 + d_dis[UP][CLOSED][nowIndex]), c)){ // 上
                tmp_r = r - (1 + d_dis[UP][CLOSED][nowIndex]), tmp_c = c;
                d_dis[DOWN][CLOSED][tmp_r * C + tmp_c] += 1 + d_dis[DOWN][CLOSED][nowIndex];
            }
            if (in_map(R, C, r, c + (1 + d_dis[RIGHT][CLOSED][nowIndex]))){ // 右 
                tmp_r = r, tmp_c = c + (1 + d_dis[RIGHT][CLOSED][nowIndex]);
                d_dis[LEFT][CLOSED][tmp_r * C + tmp_c] += 1 + d_dis[LEFT][CLOSED][nowIndex];
            }
            if (in_map(R, C, r + (1 + d_dis[DOWN][CLOSED][nowIndex]), c)){ // 下
                tmp_r = r + (1 + d_dis[DOWN][CLOSED][nowIndex]), tmp_c = c;
                d_dis[UP][CLOSED][tmp_r * C + tmp_c] += 1 + d_dis[UP][CLOSED][nowIndex];
            }
            if (in_map(R, C, r, c - (1 + d_dis[LEFT][CLOSED][nowIndex]))){ // 左
                tmp_r = r, tmp_c = c - (1 + d_dis[LEFT][CLOSED][nowIndex]);
                d_dis[RIGHT][CLOSED][tmp_r * C + tmp_c] += 1 + d_dis[RIGHT][CLOSED][nowIndex];
            }
        }
    }
    return;
}


int abs(int a)
{
    return (a >= 0)? a: -a;
}

void visited(int d_dis[DIR][STATUS][MAXINDEX], int nowR, int nowC, int tmp_rest[DIR],int R, int C)
{
    int nowIndex = nowR * C + nowC;
    for (int dir = 0; dir < DIR; dir++){
        if (!in_map(R, C, tmp_rest[(dir + 2) % DIR] / C, tmp_rest[(dir + 2) % DIR] % C)){
            continue;
        }
        d_dis[dir][VISITED][tmp_rest[(dir + 2) % DIR]] += (1 + d_dis[dir][VISITED][nowIndex] + d_dis[dir][CLOSED][nowIndex]);
    }
    
    return;
}

void closed(int nowR, int nowC, int d_dis[DIR][STATUS][MAXINDEX], int tmp_rest[DIR], int tmp_visited[DIR], int R, int C)
{
    int nowIndex = nowR * C + nowC;
    for (int dir = 0; dir < DIR; dir++){
        if (!in_map(R, C, tmp_visited[(dir + 2) % DIR] / C, tmp_visited[(dir + 2) % DIR] % C)){
            continue;
        }
        if (tmp_rest[(dir + 2) % DIR] == tmp_visited[(dir + 2) % DIR]){
            d_dis[dir][VISITED][tmp_rest[(dir + 2) % DIR]] += d_dis[dir][VISITED][nowIndex];
            d_dis[dir][CLOSED][tmp_rest[(dir + 2) % DIR]] += (1 + d_dis[dir][CLOSED][nowIndex]);
        }
        else{
            d_dis[dir][VISITED][tmp_visited[(dir + 2) % DIR]] += d_dis[dir][VISITED][nowIndex];
            d_dis[dir][CLOSED][tmp_visited[(dir + 2) % DIR]] += (1 + d_dis[dir][CLOSED][nowIndex]);
            if (in_map(R, C, tmp_rest[(dir + 2) % DIR] / C, tmp_rest[(dir + 2) % DIR] % C)){
                d_dis[dir][VISITED][tmp_rest[(dir + 2) % DIR]] += (1 + d_dis[dir][VISITED][nowIndex] + d_dis[dir][CLOSED][nowIndex]);
            }
        }

    }

    return;
}

int find_valid(Bakery map[MAXR][MAXC], int nowR, int nowC, int d_dis[DIR][STATUS][MAXINDEX], int tmp_rest[DIR], int tmp_visited[DIR], int R, int C, int visited_arr[MAXINDEX], int whether_visited[MAXINDEX], int *visited_num) // 回傳ansIndex，並更新候選點
{
    /* 更新tmp_rest與tmp_visted */

    int nowIndex = nowR * C + nowC;
    for (int dir = 0; dir < DIR; dir++){
        if (dir == UP){
            tmp_rest[UP] = (nowR - (1 + d_dis[UP][CLOSED][nowR * C + nowC] + d_dis[UP][VISITED][nowR * C + nowC])) * C + nowC;
            tmp_visited[UP] = (nowR - (1 + d_dis[UP][CLOSED][nowR * C + nowC])) * C + nowC;
        }
        else if (dir == DOWN){
            tmp_rest[DOWN] = (nowR + (1 + d_dis[DOWN][CLOSED][nowR * C + nowC] + d_dis[DOWN][VISITED][nowR * C + nowC])) * C + nowC;
            tmp_visited[DOWN] = (nowR + (1 + d_dis[DOWN][CLOSED][nowR * C + nowC])) * C + nowC;
        }
        else if (dir == LEFT){
            tmp_rest[LEFT] = nowR * C + nowC - (1 + d_dis[LEFT][CLOSED][nowR * C + nowC] + d_dis[LEFT][VISITED][nowR * C + nowC]);
            tmp_visited[LEFT] = nowR * C + nowC - (1 + d_dis[LEFT][CLOSED][nowR * C + nowC]);
        }
        else if (dir == RIGHT){
            tmp_rest[RIGHT] = nowR * C + nowC + (1 + d_dis[RIGHT][CLOSED][nowR * C + nowC] + d_dis[RIGHT][VISITED][nowR * C + nowC]);
            tmp_visited[RIGHT] = nowR * C + nowC + (1 + d_dis[RIGHT][CLOSED][nowR * C + nowC]);
        }
    }

    int ansIndex = -1,ans_rank, ans_dis;
    for (int dir = 0; dir < DIR; dir++){
        int tmp_r = tmp_rest[dir] / C, tmp_c = tmp_rest[dir] % C;
        if (!in_map(R, C, tmp_r, tmp_c)){
            continue;
        }
        if (whether_visited[tmp_rest[dir]] == 0){
            whether_visited[tmp_rest[dir]] = 1;
            visited_arr[(*visited_num)++] = tmp_rest[dir];
        }
        if (whether_visited[tmp_visited[dir]] == 0){
            whether_visited[tmp_visited[dir]] = 1;
            visited_arr[(*visited_num)++] = tmp_visited[dir];
        }


        assert((tmp_r - nowR != 0) ^ (tmp_c - nowC != 0));
        assert(map[tmp_r][tmp_c].Bread > 0);
        int now_dis = abs(tmp_r - nowR) + abs(tmp_c - nowC);
        int nowIndex = tmp_rest[dir];
        int nowRank = map[tmp_r][tmp_c].Rank;

        if (ansIndex == -1 || ansIndex != -1 && now_dis < ans_dis || ansIndex != -1 && now_dis == ans_dis && nowRank < ans_rank){
            ansIndex = nowIndex;
            ans_dis = now_dis;
            ans_rank = nowRank;
        }
    }

    return ansIndex;

}

void printMap(Bakery map[MAXR][MAXC], int R, int C)
{
    for (int r = 0; r < R; r++){
        for (int c = 0; c < C; c++){
            if (map[r][c].Bread > 0){
                printf("%d%c",map[r][c].Bread, " \n"[c == C - 1]);
                continue;
            }
            printf("0%c"," \n"[c == C - 1]);
        }
    }
    return;
}

int main(void)

{
    const int R,C;
    scanf("%d%d",&R,&C);
    static Bakery map[MAXR][MAXC];

    int rank_to_index[MAXINDEX] = {-1}; // rank是從1開始的
    int d_dis[DIR][STATUS][MAXINDEX] = {{{}}}; // 最外面那圈用in_map判斷

    Node *rank_to_node[MAXINDEX] = {NULL}; // 指向的是上層地圖的ptr

    /* 讀rank */
    for (int r = 0; r < R; r++){
        for (int c = 0; c < C; c++){
            scanf("%d",&map[r][c].Rank);
            rank_to_index[map[r][c].Rank] = r * C + c;
            if (r == 0){
                d_dis[UP][CLOSED][r * C + c] = MAXINDEX;
            }
            if (r == R - 1){
                d_dis[DOWN][CLOSED][r * C + c] = MAXINDEX;
            }
            if (c == 0){
                d_dis[LEFT][CLOSED][r * C + c] = MAXINDEX;
            }
            if (c == C - 1){
                d_dis[RIGHT][CLOSED][r * C + c] = MAXINDEX;
            }
        }
    }

    /* 讀bread */
    for (int r = 0; r < R; r++){
        for (int c = 0; c < C; c++){
            scanf("%d",&map[r][c].Bread);
        }
    }

    int T1,TR,T2;
    scanf("%d%d%d",&T1, &TR, &T2);

    int rank,len,size,day = 1;
    
    for (; day <= T1; day++){
        scanf("%d%d%d",&rank, &len, &size);
        int tmp_rest[DIR],tmp_visited[DIR],nowR = rank_to_index[rank] / C,nowC = rank_to_index[rank] % C;
        int whether_visited[MAXINDEX],visited_arr[MAXINDEX],visited_num = 0,start = 1, track_arr[MAXINDEX],track_num = 0;
        while (len >= 0){
            int ansIndex = find_valid(map, nowR, nowC, d_dis, tmp_rest, tmp_visited, R, C,visited_arr, whether_visited, &visited_num);
            if (!start){
                map[nowR][nowC].Bread -= size;
            }
            if (map[nowR][nowC].Bread > 0){
                visited(d_dis, nowR, nowC, tmp_rest, R, C);
            }
            else{
                closed(nowR, nowC, d_dis, tmp_rest, tmp_visited, R, C);
            }

            track_arr[track_num++] = nowR * C + nowC;

            if (ansIndex == -1){
                break;
            } 
            nowR = ansIndex / C;
            nowC = ansIndex % C;
            len--,start = 0;
        }
        for (int i = 0; i < visited_num; i++){
            for (int dir = 0; dir < DIR; dir++){
                d_dis[dir][VISITED][visited_arr[i]] = 0;
                whether_visited[visited_arr[i]] = 0;
            }
        }
    }

    /* 架ll */
    Node *now = NULL, *prev = NULL, *tmp_head = NULL, *prev_tmp_head = NULL, *head = NULL;
    for (int r = 0; r < R; r++){
        for (int c = 0; c < C; c++){ // 每造完1 row更新vt
            now = genPairNode(map[r][c].Rank, map[r][c].Bread);
            rank_to_node[map[r][c].Rank] = now;
            if (r == 0 && c == 0){
                head = now;
            }
            if (c == 0){
                prev = now;
                prev_tmp_head = tmp_head;
                tmp_head = now;
                continue;
            }
            assert(prev != NULL);
            prev->hr = now;
            now->wormhole->hr = prev->wormhole;
            prev = now;
        }
        if (prev_tmp_head == NULL){
            continue;
        }
        for (Node *ptr1 = prev_tmp_head, *ptr2 = tmp_head; ptr1 != NULL; ptr1 = ptr1->hr, ptr2 = ptr2->hr){
            assert(ptr2 != NULL);
            ptr1->vt = ptr2;
            ptr2->wormhole->vt = ptr1->wormhole;
        }
    }

    for (; day <= T1 + TR; day++){
        scanf("%d%d",&rank, &len);
        if (len == 1){
            continue;
        }
        Node *center = rank_to_node[rank];
        int r = len / 2;
        Node *ptr1 = find_corner(center->wormhole, r)->wormhole; // 取上層的左上
        Node *ptr2 = find_corner(center, r)->wormhole; // 取下層的右下 
        if (head == ptr1){
            head = ptr2;
        }
        else if (head == ptr2){
            head = ptr1;
        }
        for (int i = 0; i < len; i++){
            Node *tmp_ptr1_hr = ptr1->wormhole->hr;
            Node *tmp_ptr2_hr = ptr2->wormhole->hr;
            if (tmp_ptr1_hr != NULL){
                tmp_ptr1_hr->wormhole->hr = ptr2;
                ptr2->wormhole->hr = tmp_ptr1_hr;
            }
            else{
                ptr2->wormhole->hr = NULL;

            }
            
            if (tmp_ptr2_hr != NULL){
                tmp_ptr2_hr->wormhole->hr = ptr1;
                ptr1->wormhole->hr = tmp_ptr2_hr;
            }
            else{
                ptr1->wormhole->hr = NULL;

            }
            if (i != len - 1){
                ptr1 = ptr1->vt;
                ptr2 = ptr2->vt;
            }
        }

        assert(ptr1 != NULL && ptr2 != NULL);
        for (int i = 0; i < len; i++){
            Node *tmp_ptr1_vt = ptr1->vt;
            Node *tmp_ptr2_vt = ptr2->vt;
            if (tmp_ptr1_vt != NULL){
                tmp_ptr1_vt->wormhole->vt = ptr2->wormhole;
                ptr2->vt = tmp_ptr1_vt;
            }
            else{
                ptr2->vt = NULL;
            }
            if (tmp_ptr2_vt != NULL){
                tmp_ptr2_vt->wormhole->vt = ptr1->wormhole;
                ptr1->vt = tmp_ptr2_vt;
            }
            else{
                ptr1->vt = NULL;
            }
            ptr1 = ptr1->hr;
            ptr2 = ptr2->hr;
        }
    }

    /* 將ll的東西放回array中 */
    put_to_array(rank_to_index, map, head, R, C, d_dis);
    init_ddis(map, d_dis, R, C);
    
    /* 改d_dis */
    for (; day <= T1 + TR + T2; day++){
        scanf("%d%d%d",&rank, &len, &size);
        int tmp_rest[DIR],tmp_visited[DIR],nowR = rank_to_index[rank] / C,nowC = rank_to_index[rank] % C;
        int whether_visited[MAXINDEX],visited_arr[MAXINDEX],visited_num = 0,start = 1,track_arr[MAXINDEX], track_num = 0;
        while (len >= 0){
            int ansIndex = find_valid(map, nowR, nowC, d_dis, tmp_rest, tmp_visited, R, C,visited_arr, whether_visited, &visited_num);
            if (!start){
                map[nowR][nowC].Bread -= size;
            }
            if (map[nowR][nowC].Bread > 0){
                visited(d_dis, nowR, nowC, tmp_rest, R, C);
            }
            else{
                closed(nowR, nowC, d_dis, tmp_rest, tmp_visited, R, C);
            }
            track_arr[track_num++] = nowR * C + nowC;


            if (ansIndex == -1){
                break;
            } 
            nowR = ansIndex / C;
            nowC = ansIndex % C;
            len--,start = 0;
        }

        
        for (int i = 0; i < visited_num; i++){
            for (int dir = 0; dir < DIR; dir++){
                d_dis[dir][VISITED][visited_arr[i]] = 0;
                whether_visited[visited_arr[i]] = 0;
            }
        }
    }
    printMap(map, R, C);
    return 0;
}