#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>

#define MAXM 1000

typedef struct Arena{
    int index,attack;
    struct Arena *next;
    struct Arena *prev;
} Arena;

typedef struct{
    Arena *arenaHead;
    Arena *arenaTail;
    Arena *King;
    int n,M;
} ArenaData;

int cmp(const void *a, const void *b)
{
    int *ptr1 = (int *)a;
    int *ptr2 = (int *)b;

    return (*ptr2 - *ptr1);

}

Arena *genArena(int index, int attack, Arena* next){
    Arena *now = (Arena *)malloc(sizeof(Arena));
    now->next = next;
    if (next != NULL){
        next->prev = now;
    }
    now->index = index;
    now->attack = attack;
    now->prev = NULL;
    return now;
}

int Attack(int index, int attack, ArenaData *arenaData) // attack 小到大排
{
    Arena *head = arenaData->arenaHead;
    (arenaData->n)++;
    if (head == NULL){
        arenaData->arenaHead = genArena(index, attack, NULL);
        arenaData->King = arenaData->arenaHead;
        return 0;
    }

    Arena *now = arenaData->arenaHead;
    Arena *prev = NULL;

    int n = 0;
    bool cont = 1;

    /* 遇到第一個大於等於自己的就停 */
    if (arenaData->arenaHead->attack >= attack){
        arenaData->arenaHead = genArena(index, attack, arenaData->arenaHead);
        cont = 0;
    }

    int kill_arr[MAXM];
    while (cont){
        if (now == NULL){ // 沒遇到大於等於自己的
            assert(prev != NULL);
            arenaData->arenaHead = genArena(index, attack, NULL);
            arenaData->King = arenaData->arenaHead;
            printf(" %d",prev->index);
            kill_arr[n++] = prev->index;
            free(prev);
            (arenaData->n)--;
            assert(arenaData->n == 1);
            break;
        }
        else if (prev != NULL){
            kill_arr[n++] = prev->index;
            printf(" %d",prev->index);
            free(prev);
            (arenaData->n)--;
        }
        if (now != NULL && now->attack >= attack){
            arenaData->arenaHead = genArena(index, attack, now);
            break;
        }
        prev = now;
        now = now->next;
    }

    if (arenaData->n > arenaData->M){
        kill_arr[n++] = arenaData->King->index;
        printf(" %d",arenaData->King->index);
        Arena *temp = arenaData->King->prev;
        free(arenaData->King);
        arenaData->King = temp;
        arenaData->King->next = NULL;
        (arenaData->n)--;
    }

    // for (int i = 0; i < n; i++){
    // }
}



int main(void)
{
    int N,M;
    scanf("%d%d",&N,&M);

    ArenaData arenaData;
    // ArenaData dick[1983201082309123];

    arenaData.King = NULL;
    arenaData.arenaHead = NULL;
    arenaData.arenaTail = NULL;
    arenaData.M = M;
    arenaData.n = 0;

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
        printf(" %d",it->index);
    }

    return 0;
}
