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

bool valid(Board board, Clue Row_clue[MAXR], Clue Col_clue[MAXC], int R, int C, int total_color_num)
{
    /* use row_clue but check col */
    assert(board.now_Color_num == total_color_num);

    int board_color_num = 0;
    for (int r = 0; r < R; r++){

        /* 先找第一個黑色出現的index */
        int start_c = 0;
        for (; start_c < C; start_c++){
            if (board.matrix[r][start_c]){
                break;
            }
        }

        int black_block_num = 0;
        int black_appear_num = 0;
        int black_num_arr[MAXC]; 

        for (int c = start_c; c < C; c++){
            if (board.matrix[r][c]){
                black_appear_num++;
                board_color_num++;

                if (c == C - 1){
                    black_num_arr[black_block_num++] = black_appear_num;
                }
            }
            else{
                if (black_appear_num != Row_clue[r].color[black_block_num]){
                    return 0;
                }
                black_num_arr[black_block_num++] = black_appear_num;
                black_appear_num = 0;
            }
        }

        if (black_block_num != Row_clue[r].n){
            return 0;
        }

        for (int i = 0; i < black_block_num; i++){
            if (black_num_arr[i] != Row_clue[r].color[i]){
                return 0;
            }
        }
    }

    /* use col_clue but check row */

    for (int c = 0; c < C; c++){

        /* 先找第一個黑色出現的index */
        int start_r = 0;
        for (; start_r < R; start_r++){
            if (board.matrix[start_r][c]){
                break;
            }
        }

        int black_block_num = 0;
        int black_appear_num = 0;
        int black_num_arr[MAXR]; 

        for (int r = start_r; r < R; r++){
            if (board.matrix[r][c]){
                board_color_num++;
                black_appear_num++;

                if (r == R - 1){
                    black_num_arr[black_block_num++] = black_appear_num;
                }
            }
            else{
                if (black_appear_num != Col_clue[c].color[black_block_num]){
                    return 0;
                }
                black_num_arr[black_block_num++] = black_appear_num;
                black_appear_num = 0;
            }
        }

        if (black_block_num != Col_clue[c].n){
            return 0;
        }

        for (int i = 0; i < black_block_num; i++){
            if (black_num_arr[i] != Col_clue[c].color[i]){
                return 0;
            }
        }
    }

    assert(board_color_num / 2 == board.now_Color_num);

    return 1;
}

void print_board(int R, int C, Board board)
{
    for (int r = 0; r < R; r++){
        for (int c = 0; c < C; c++){
            if (board.matrix[r][c]){
                printf("o");
            }
            else{
                printf("_");
            }
        }
        printf("\n");
    }
}

void empty(Board board, int R, int C)
{
    for (int i = 0; i < R; i++){
        for (int j = 0; j < C; j++){
            board.matrix[i][j] = 0;
        }
    }

    return;
}

bool whether_valid(Board board, int nowR, int nowC, Clue Row_clue[MAXR], Clue Col_clue[MAXC],int R, int C)
{
    int nowRow_num = 0;
    for (int c = 0; c < C; c++){
        nowRow_num += board.matrix[nowR][c];
    }

    int AnsRow_num = 0;
    for (int i = 0; i < Row_clue[nowR].n; i++){
        AnsRow_num += Row_clue[nowR].color[i];
    }

    if (nowRow_num > AnsRow_num){
        return 0;
    }

    int nowCol_num = 0;
    for (int r = 0; r < R; r++){
        nowCol_num += board.matrix[r][nowC];
    }

    int AnsCol_num = 0;
    for (int i = 0; i < Col_clue[nowC].n; i++){
        AnsCol_num += Col_clue[nowC].color[i];
    }

    if (nowCol_num > AnsCol_num){
        return 0;
    }

    return 1;
    

}

bool sol(int total_color_num, int R, int C, Board board, Clue Row_clue[MAXR], Clue Col_clue[MAXC],int nowstep)
{
    int nowR = nowstep / R;
    int nowC = nowstep % R;

    if (board.now_Color_num > total_color_num /*|| !whether_valid(board, nowR, nowC, Row_clue, Col_clue, R, C)*/){
        return 0;
    }

    if (nowstep == R * C && board.now_Color_num == total_color_num){
        if (valid(board, Row_clue, Col_clue, R, C, total_color_num)){
            // printf("xd");
            print_board(R, C, board);
            return 1;
        }
        return 0;
    }
    else if (nowstep == R * C){
        return 0;
    }
    else{
        assert(!board.matrix[nowR][nowC]);
        /* paint black */
        board.matrix[nowR][nowC] = 1;
        board.now_Color_num++;

        if (sol(total_color_num, R, C, board, Row_clue, Col_clue, nowstep + 1)){
            return 1;
        }

        /* try fail, paint to white */
        board.matrix[nowR][nowC] = 0;
        board.now_Color_num--;
        if (sol(total_color_num, R, C, board, Row_clue, Col_clue, nowstep + 1)){
            return 1;
        }
        // return 0;
    }

    empty(board, R, C);

    
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
    sol(total_color_num, R, C, board, Row_clue, Col_clue, 0);
    

    
    return 0;
}
