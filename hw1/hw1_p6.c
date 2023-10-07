#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>

#define MAXR 1000
#define MAXC 1000

typedef struct {
    int Rank;
    int Bread;
    int visited_day;
} Rank_and_Bread;

bool in_map(int R, int C, int nowR, int nowC)
{
    if (nowR < R && nowR >= 0 && nowC < C && nowC >= 0){
        return 1;
    }
    return 0;
}

int max(int R, int C)
{
    return (R >= C)? R: C;
}

int find_valid(Rank_and_Bread map[MAXR][MAXC], int nowR, int nowC, int R, int C, int day) // 回傳值為
{
    int dr[4] = {-1,0,0,1};
    int dc[4] = {0,-1,1,0};

    bool find = 0;

    int ansIndex = -1, ansrank = -1;

    int final = max(R,C);
    for (int mul = 1; mul <= final && !find; mul++){
        for (int i = 0; i < 4; i++){
            for (int j = 0; j < 4; j++){
                if ((dr[i] != 0 && dc[j] != 0)){
                    continue;
                }
                int drow = mul * dr[i];
                int dcol = mul * dc[j];
                if (!(drow == 0 && dcol == 0) && in_map(R, C, nowR + drow, nowC + dcol) && map[nowR + drow][nowC + dcol].Bread > 0 && map[nowR + drow][nowC + dcol].visited_day != day){
                    find = 1; // 已找到候選點;
                    if (map[nowR + drow][nowC + dcol].Rank < ansrank || ansrank == -1){
                        ansIndex = (nowR + drow) * C + (nowC + dcol);
                        ansrank = map[nowR + drow][nowC + dcol].Rank;
                    }
                }
            }
        }
    }

    return ansIndex; // 若ansIndex == -1, 則未找到
}

void printMap(Rank_and_Bread map[MAXR][MAXC], int R, int C)
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
    static Rank_and_Bread map[MAXR][MAXC];

    int rank_to_index[MAXR * MAXC + 1] = {-1}; // rank是從1開始的

    /* 讀rank */
    // printf("R:%d C:%d\n",R,C);
    for (int r = 0; r < R; r++){
        for (int c = 0; c < C; c++){
            scanf("%d",&map[r][c].Rank);
            rank_to_index[map[r][c].Rank] = r * C + c;
            map[r][c].visited_day = 0;
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

    /* Training period 1 */
    int rank,len,size,day = 1;
    for (; day <= T1; day++){
        scanf("%d%d%d",&rank, &len, &size);

        int nowR = rank_to_index[rank] / C;
        int nowC = rank_to_index[rank] % C;

        map[nowR][nowC].visited_day = day;

        // printf("nowR:%d nowC:%d\n",nowR, nowC);
        // exit(0);

        while (len > 0){
            int ans_index = find_valid(map, nowR, nowC, R, C, day);
            // printf("ans_index:%d\n",ans_index);

            if (ans_index == -1){
                break;
            }

            nowR = ans_index / C;
            nowC = ans_index % C;
            // printf("nowR:%d nowC:%d\n",nowR, nowC);

            map[nowR][nowC].Bread -= size;
            map[nowR][nowC].visited_day = day;
            len--;
        }
        /* find valid,拜訪過的使map.day = day */
    }

    /* racing period */
    assert(day == T1 + 1);
    for (; day <= T1 + TR; day++){
        scanf("%d%d", &rank, &len);

        int nowR = rank_to_index[rank] / C;
        int nowC = rank_to_index[rank] % C;

        int dl = len / 2;
        bool cont = 1;
        for (int r1 = nowR - dl; r1 <= nowR + dl && cont; r1++){
            for (int c1 = nowR - dl; c1 <= nowC + dl; c1++){

                int r2 = 2 * nowR - r1;
                int c2 = 2 * nowC - c1;
                if (r1 == r2 && c1 == c2){
                    cont = 0;
                    break;
                }

                assert(r1 < nowR || c1 < nowC);

                /* swap map */
                Rank_and_Bread temp = map[r1][c1];
                map[r1][c1] = map[r2][c2];
                map[r2][c2] = temp;

                /* swap rank_to_index */
                int arrtemp = rank_to_index[map[r1][c1].Rank];
                rank_to_index[map[r1][c1].Rank] = rank_to_index[map[r2][c2].Rank];
                rank_to_index[map[r2][c2].Rank] = arrtemp;
                // printf("r1:%d c1:%d r2:%d c2:%d\n",r1,c1,r2,c2);
                // printf("rk:map[%d][%d].rank:%d index:%d\n",r1,c1,map[r1][c1].Rank, rank_to_index[map[r1][c1].Rank]);
                // printf("rk:map[%d][%d].rank:%d index:%d\n",r2,c2,map[r2][c2].Rank, rank_to_index[map[r2][c2].Rank]);
            }
        }
    }

    assert(day == T1 + TR + 1);
    for (; day <= T1 + T2 + TR; day++){
        scanf("%d%d%d", &rank, &len, &size);

        int nowR = rank_to_index[rank] / C;
        int nowC = rank_to_index[rank] % C;

        map[nowR][nowC].visited_day = day;

        while (len > 0){
            int ans_index = find_valid(map, nowR, nowC, R, C, day);

            if (ans_index == -1){
                break;
            }

            nowR = ans_index / C;
            nowC = ans_index % C;
            map[nowR][nowC].Bread -= size;
            map[nowR][nowC].visited_day = day;
            len--;
        }
    }

    // printf("\n");
    printMap(map, R, C);

    return 0;
}
