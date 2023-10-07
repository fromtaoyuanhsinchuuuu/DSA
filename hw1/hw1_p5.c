#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>

#define MAXM 1000

typedef struct Arena{
    int index,attack,dscore;
    struct Arena *next;
    struct Arena *prev;
    int rev_in;
} Arena;

typedef struct{
    Arena *arenaHead;
    Arena *arenaTail;
    Arena *King;
    int drev;
    int rev;
    int n,M;
} ArenaData; 

Arena *genArena(int index, int attack, int dscore, int rev_in,Arena* next){
    Arena *now = (Arena *)malloc(sizeof(Arena));
    now->next = next;
    if (next != NULL){
        next->prev = now;
    }
    now->dscore = dscore;
    now->index = index;
    now->attack = attack;
    now->rev_in = rev_in;
    now->prev = NULL;
    return now;
}

int Attack(int index, int attack, ArenaData *arenaData) // attack 小到大排
{
    Arena *head = arenaData->arenaHead;
    (arenaData->n)++;
    if (head == NULL){
        arenaData->arenaHead = genArena(index, attack, arenaData->M, 0, NULL);
        arenaData->King = arenaData->arenaHead;
        return 0;
    }

    Arena *now = arenaData->arenaHead;
    Arena *prev = NULL;

    int n = 0;
    bool cont = 1;

    /* 遇到第一個大於等於自己的就停 */
    if (arenaData->arenaHead->attack >= attack){
        int dscore = (arenaData->King->dscore - arenaData->n) + 1;
        arenaData->arenaHead = genArena(index, attack, dscore, arenaData->rev, arenaData->arenaHead);
        cont = 0;
    }

    while (cont){
        if (now == NULL){ // 沒遇到大於等於自己的
            assert(prev != NULL);
            arenaData->arenaHead = genArena(index, attack, arenaData->M - arenaData->drev, arenaData->rev, NULL);
            arenaData->King = arenaData->arenaHead;
            printf(" %d,%d",prev->index,((index - prev->index) * prev->dscore) + (arenaData->rev - prev->rev_in));
            free(prev);
            (arenaData->n)--;
            assert(arenaData->n == 1);
            break;
        }
        else if (prev != NULL){
            printf(" %d,%d",prev->index,((index - prev->index) * prev->dscore) + (arenaData->rev - prev->rev_in));
            free(prev);
            (arenaData->n)--;
        }
        if (now != NULL && now->attack >= attack){
            arenaData->arenaHead = genArena(index, attack, now->dscore - 1, arenaData->rev, now);
            break;
        }
        prev = now;
        now = now->next;
    }

    if (arenaData->n > arenaData->M){
        printf(" %d,%d",arenaData->King->index, (arenaData->King->dscore * (index - (arenaData->King->index))) + (arenaData->rev - arenaData->King->rev_in));
        Arena *temp = arenaData->King->prev;
        free(arenaData->King);
        arenaData->King = temp;
        arenaData->King->next = NULL;
        (arenaData->n)--;
        (arenaData->drev)++;
    }
    arenaData->rev += arenaData->drev;

}



int main(void)
{
    int N,M;
    scanf("%d%d",&N,&M);

    ArenaData arenaData;
    arenaData.King = NULL;
    arenaData.arenaHead = NULL;
    arenaData.arenaTail = NULL;
    arenaData.M = M;
    arenaData.n = 0;
    arenaData.rev = 0;
    arenaData.drev = 0;

    int attack;
    for (int index = 1; index <= N; index++){
        scanf("%d",&attack);
        printf("Round %d:",index);
        Attack(index, attack, &arenaData);
        printf("\n");
    }

    int live_arr[MAXM];
    int n = 0;

    printf("Final:");
    for (Arena *it = arenaData.arenaHead; it != NULL; it = it->next){
        printf(" %d,%d",it->index,(((N+1) - it->index) * it->dscore) + (arenaData.rev - it->rev_in));
    }

    return 0;
}
