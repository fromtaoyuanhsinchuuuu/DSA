#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>

#define MAXC 25
#define MAXR 25

typedef struct {
    int n;
    int color[MAXR];
} Clue;

typedef struct {
    int now_Color_num;
    int matrix[MAXR][MAXC];
} Board;

void print_board(int R, int C, Board *board)
{
    for (int r = 0; r < R; r++){
        // printf("r:%d\n",r);
        for (int c = 0; c < C; c++){
            if (board->matrix[r][c]){
                printf("o");
            }
            else{
                printf("_");
            }
        }
        printf("\n");
    }
}

void empty(Board *board, int R, int C)
{
    for (int i = 0; i < R; i++){
        for (int j = 0; j < C; j++){
            board->matrix[i][j] = 0;
        }
    }

    return;
}

bool col_valid(Board *board, int checkCol, Clue Col_clue[MAXR], int R)
{

        /* 先找第一個黑色出現的index */
        int start_r = 0;
        for (; start_r < R; start_r++){
            if (board->matrix[start_r][checkCol]){
                break;
            }
        }

        int black_block_num = 0;
        int black_appear_num = 0;
        int black_num_arr[MAXC]; 

        bool prev = 1;
        for (int r = start_r; r < R; r++){
            if (board->matrix[r][checkCol]){
                black_appear_num++;
                // board_color_num++;

                if (r == R - 1){
                    black_num_arr[black_block_num++] = black_appear_num;
                }
            }
            else{
                if (!prev){
                    continue;
                }
                if (black_appear_num != Col_clue[checkCol].color[black_block_num]){
                    return 0;
                }
                black_num_arr[black_block_num++] = black_appear_num;
                black_appear_num = 0;
            }

            prev = board->matrix[r][checkCol];

        }

        if (black_block_num != Col_clue[checkCol].n){
            return 0;
        }

        for (int i = 0; i < black_block_num; i++){
            if (black_num_arr[i] != Col_clue[checkCol].color[i]){
                return 0;
            }
        }

        return 1;
}
 
bool row_valid(Board *board, int checkRow, Clue Row_clue[MAXR], int C)
{

        /* 先找第一個黑色出現的index */
        int start_c = 0;
        for (; start_c < C; start_c++){
            if (board->matrix[checkRow][start_c]){
                break;
            }
        }

        int black_block_num = 0;
        int black_appear_num = 0;
        int black_num_arr[MAXC]; 


        bool prev = 1;
        for (int c = start_c; c < C; c++){
            if (board->matrix[checkRow][c]){
                black_appear_num++;
                // board_color_num++;

                if (c == C - 1){
                    black_num_arr[black_block_num++] = black_appear_num;
                }
            }
            else{
                if (!prev){
                    continue;
                }
                if (black_appear_num != Row_clue[checkRow].color[black_block_num]){
                    return 0;
                }
                black_num_arr[black_block_num++] = black_appear_num;
                black_appear_num = 0;
            }

            prev = board->matrix[checkRow][c];

        }

        if (black_block_num != Row_clue[checkRow].n){
            return 0;
        }

        for (int i = 0; i < black_block_num; i++){
            if (black_num_arr[i] != Row_clue[checkRow].color[i]){
                return 0;
            }
        }

        return 1;
}

bool sol(int total_color_num, int R, int C, Board *board, Clue Row_clue[MAXR], Clue Col_clue[MAXC],int nowstep)
{
    if (board->now_Color_num > total_color_num){
        return 0;
    }

    // printf("nowstep:%d\n",nowstep);

    if (nowstep == R * C && board->now_Color_num == total_color_num){
        if (!row_valid(board, R - 1, Row_clue, C)){
            return 0;
        }
        
        for (int c = 0; c < C; c++){
            if (!col_valid(board, c, Col_clue, R)){
                return 0;
            }
        }
        print_board(R, C, board);
        return 1;
    }

    if (nowstep == R * C){
        return 0;
    }

    int nowR,nowC;
    nowR = nowstep / C;
    nowC = nowstep % C;

    if (nowR >= 1 && nowC == 0){
        // printf("!\n");
        if (!row_valid(board, nowR - 1, Row_clue, C)){
            return 0;
        }
    }

    board->matrix[nowR][nowC] = 1;
    board->now_Color_num++;

    if (sol(total_color_num, R, C, board, Row_clue, Col_clue, nowstep + 1)){
        return 1;
    }

    board->matrix[nowR][nowC] = 0;
    board->now_Color_num--;

    if (sol(total_color_num, R, C, board, Row_clue, Col_clue, nowstep + 1)){
        return 1;
    }

    return 0;
}

int main(void)
{
    Board board = {0,{{0}}};

    int R,C;
    scanf("%d%d",&R,&C);

    int total_color_num = 0;

    Clue Row_clue[MAXR];
    Clue Col_clue[MAXC];

    /* read row clues */
    for (int i = 0; i < R; i++){
        scanf("%d",&Row_clue[i].n);
        for (int j = 0; j < Row_clue[i].n; j++){
            scanf("%d",&(Row_clue[i].color[j]));
            total_color_num += Row_clue[i].color[j];
        }
    }

    /* read col clues */
    int col_num = 0;
    for (int i = 0; i < C; i++){
        scanf("%d",&Col_clue[i].n);
        for (int j = 0; j < Col_clue[i].n; j++){
            scanf("%d",&(Col_clue[i].color[j]));
            col_num += Col_clue[i].color[j];
        }
    }

    // printf("%d\n",total_color_num);
    assert(col_num == total_color_num);
    sol(total_color_num, R, C, &board, Row_clue, Col_clue, 0);
    
    return 0;
}
