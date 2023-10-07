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

typedef struct {
    int rank,Bread;
    struct Node *vt, *hr, *wormhole;
} Node;

typedef struct {
    Bakery bakery;
    Node *Upper;
    Node *Lower;


    
}







int abs(int a)
{
    return (a >= 0)? a: -a;
}

bool in_map(int R, int C, int nowR, int nowC)
{
    if (nowR < R && nowR >= 0 && nowC < C && nowC >= 0){
        return 1;
    }
    return 0;
}

void visited(int d_dis[DIR][STATUS][MAXINDEX], int nowR, int nowC, int tmp_rest[DIR],int R, int C)
{
    int nowIndex = nowR * C + nowC;

    // printf("nowR:%d nowC:%d nowIndex:%d\n",nowR, nowC, nowIndex);
    for (int dir = 0; dir < DIR; dir++){
        if (!in_map(R, C, tmp_rest[(dir + 2) % DIR] / C, tmp_rest[(dir + 2) % DIR] % C)){
            continue;
        }
        // printf("%d,%d\n",d_dis[dir][VISITED][tmp_rest[(dir + 2) % DIR]],1 + d_dis[dir][VISITED][nowIndex]);
        d_dis[dir][VISITED][tmp_rest[(dir + 2) % DIR]] += (1 + d_dis[dir][VISITED][nowIndex] + d_dis[dir][CLOSED][nowIndex]);
        // printf("pos:(%d,%d) dir:%d total_dis:%d vis_dis:%d close_dis:%d\n",tmp_rest[(dir + 2) % DIR] / C, tmp_rest[(dir + 2) % DIR] % C, dir, d_dis[dir][VISITED][tmp_rest[(dir + 2) % DIR]] + d_dis[dir][CLOSED][tmp_rest[(dir + 2) % DIR]] + 1,d_dis[dir][VISITED][tmp_rest[(dir + 2) % DIR]],d_dis[dir][CLOSED][tmp_rest[(dir + 2) % DIR]]);
    }
    
    // assert(d_dis[LEFT][VISITED][nowR * C] >= MAXINDEX);

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
            // printf("dir:%d dick\n",dir);
            d_dis[dir][VISITED][tmp_visited[(dir + 2) % DIR]] += d_dis[dir][VISITED][nowIndex];
            d_dis[dir][CLOSED][tmp_visited[(dir + 2) % DIR]] += (1 + d_dis[dir][CLOSED][nowIndex]);
            if (in_map(R, C, tmp_rest[(dir + 2) % DIR] / C, tmp_rest[(dir + 2) % DIR] % C)){
                d_dis[dir][VISITED][tmp_rest[(dir + 2) % DIR]] += (1 + d_dis[dir][VISITED][nowIndex] + d_dis[dir][CLOSED][nowIndex]);
            }
        }
        // printf("pos:(%d,%d) dir:%d total_dis:%d vis_dis:%d close_dis:%d\n",tmp_rest[(dir + 2) % DIR] / C, tmp_rest[(dir + 2) % DIR] % C, dir, d_dis[dir][VISITED][tmp_rest[(dir + 2) % DIR]] + d_dis[dir][CLOSED][tmp_rest[(dir + 2) % DIR]] + 1,d_dis[dir][VISITED][tmp_rest[(dir + 2) % DIR]],d_dis[dir][CLOSED][tmp_rest[(dir + 2) % DIR]]);

    }

    return;
}

int find_valid(Bakery map[MAXR][MAXC], int nowR, int nowC, int d_dis[DIR][STATUS][MAXINDEX], int tmp_rest[DIR], int tmp_visited[DIR], int R, int C, int visited_arr[MAXINDEX], int whether_visited[MAXINDEX], int *visited_num) // 回傳ansIndex，並更新候選點
{
    /* 更新tmp_rest與tmp_visted */

    // printf("nowpos:(%d,%d)\n",nowR, nowC);
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
        // printf("dir:%d total_dis:%d vis_dis:%d close_dis:%d\n",dir, d_dis[dir][VISITED][nowIndex] + d_dis[dir][CLOSED][nowIndex] + 1,d_dis[dir][VISITED][nowIndex],d_dis[dir][CLOSED][nowIndex]);
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

        // printf("nowpos:(%d,%d), tmp_pos:(%d,%d), nowrank:%d, tmp_rank:%d\n",nowR, nowC, tmp_r, tmp_c,map[nowR][nowC].Rank, map[tmp_r][tmp_c].Rank);

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
    // printf("R:%d C:%d\n",R,C);
    for (int r = 0; r < R; r++){
        for (int c = 0; c < C; c++){
            // printf("r:%d c:%d\n",r,c);
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
            // printf("T2:%d\n",T2);
            // if (d_dis[LEFT][])
            int ansIndex = find_valid(map, nowR, nowC, d_dis, tmp_rest, tmp_visited, R, C,visited_arr, whether_visited, &visited_num);
            if (!start){
                map[nowR][nowC].Bread -= size;
            }
            if (map[nowR][nowC].Bread > 0){
                visited(d_dis, nowR, nowC, tmp_rest, R, C);
            }
            else{
                // printf("rank:%d closed!\n",map[nowR][nowC].Rank);
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

        for (int i = 0; i < track_num; i++){
            // printf("rank:%d pos:(%d,%d)\n", map[track_arr[i] / C][track_arr[i] % C].Rank ,track_arr[i] / C, track_arr[i] % C);
        }
        for (int i = 0; i < visited_num; i++){
            for (int dir = 0; dir < DIR; dir++){
                d_dis[dir][VISITED][visited_arr[i]] = 0;
                whether_visited[visited_arr[i]] = 0;
            }
        }
    }

    /* 架ll */


   

    

    assert(day == T1 + TR + 1);
    for (; day <= T1 + TR + T2; day++){
        scanf("%d%d%d",&rank, &len, &size);
        int tmp_rest[DIR],tmp_visited[DIR],nowR = rank_to_index[rank] / C,nowC = rank_to_index[rank] % C;
        int whether_visited[MAXINDEX],visited_arr[MAXINDEX],visited_num = 0,start = 1,track_arr[MAXINDEX], track_num = 0;
        while (len >= 0){
            // printf("T2:%d\n",T2);
            // if (d_dis[LEFT][])
            
            int ansIndex = find_valid(map, nowR, nowC, d_dis, tmp_rest, tmp_visited, R, C,visited_arr, whether_visited, &visited_num);
            if (!start){
                map[nowR][nowC].Bread -= size;
            }
            if (map[nowR][nowC].Bread > 0){
                visited(d_dis, nowR, nowC, tmp_rest, R, C);
            }
            else{
                // printf("rank:%d closed!\n",map[nowR][nowC].Rank);

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

        for (int i = 0; i < track_num; i++){
            // printf("rank:%d pos:(%d,%d)\n", map[track_arr[i] / C][track_arr[i] % C].Rank ,track_arr[i] / C, track_arr[i] % C);
        }
        for (int i = 0; i < visited_num; i++){
            // printf("visited_rank:%d\n", map[visited_arr[i] / C][visited_arr[i] % C].Rank);
            for (int dir = 0; dir < DIR; dir++){
                d_dis[dir][VISITED][visited_arr[i]] = 0;
                whether_visited[visited_arr[i]] = 0;
            }
        }
    }
    printMap(map, R, C);



    return 0;
}
