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
} Arena;

typedef struct{
    Arena *arenaHead;
    // Arena *arenaTail;
    Arena *King;
    int n,M;
} ArenaData;

int cmp(const void *a, const void *b)
{
    int *ptr1 = (int *)a;
    int *ptr2 = (int *)b;

    return (*ptr2 - *ptr1);

}

Arena *genArena(int index, int attack, Arena* remain){
    Arena *now = (Arena *)malloc(sizeof(Arena));
    now->next = remain;
    now->index = index;
    now->attack = attack;
    return now;
}

Arena* insertArena(int index, int attack, Arena *arena, int n)
{
    /* 以降冪做排序 */
    if (n == 0){ // 尚未有元素
        return genArena(index, attack, NULL);
    }
    else if (arena->index < index){
        return genArena(index, attack, arena);
    }

    Arena *head = arena;

    Arena *cur = arena;
    Arena *prev = NULL;

    while (1){
        if (cur == NULL){ // 已到最後的元素
            assert(prev != NULL);
            Arena *cur = genArena(index, attack, NULL);
            prev->next = cur;
            return head;
        }
        else if (cur->index < index){
            assert(prev != NULL);
            prev->next = genArena(index, attack, cur);
            return head;
        }

        prev = cur;
        cur = cur->next;
    }

    assert(0);
    return head;
}

int Attack(int index, int attack, ArenaData *arenaData) // return 1表有revolution, 0代表沒有
{
    Arena *head = arenaData->arenaHead;
    (arenaData->n)++;
    if (head == NULL){
        arenaData->arenaHead = genArena(index, attack, NULL);
        arenaData->King = arenaData->arenaHead;
        return 0;
    }

    assert(head == arenaData->King);
    Arena *now = arenaData->arenaHead;
    Arena *prev = NULL;

    Arena *first_less = NULL;

    int n = 0;

    bool cont = 1;
    if (attack > head->attack){
        arenaData->arenaHead = genArena(index, attack, NULL);
        first_less = head;
        arenaData->King = arenaData->arenaHead;
        cont = 0;
    }

    while (cont){ // 找到第一個比自己小的就停
        if (now == NULL){ // 沒有人比自己小
            assert(prev != NULL);
            prev->next = genArena(index, attack, NULL);
            first_less = NULL;
            break;
        }
        else if (now->attack < attack){
            first_less = now;
            prev->next = genArena(index, attack, NULL);
            break;
        }

        prev = now;
        now = now->next;
    }

    int kill_num = 0;
    int kill_arr[MAXM] = {};

    for (Arena *it = first_less; it != NULL; (arenaData->n)--){
        kill_arr[kill_num++] = it->index;
        Arena *prev = it;
        it = it->next;
        free(prev);
    }

    bool yes = 0;
    if (arenaData->n > arenaData->M){
        kill_arr[kill_num++] = head->index;
        arenaData->arenaHead = arenaData->arenaHead->next;
        free(head);
        arenaData->King = arenaData->arenaHead;
        (arenaData->n)--;
        yes = 1;
    }

    qsort(kill_arr, kill_num, sizeof(int), cmp);

    for (int i = 0; i < kill_num; i++){
        printf(" %d",kill_arr[i]);
    }
    return yes;
}



int main(void)
{
    int N,M;
    scanf("%d%d",&N,&M);

    ArenaData arenaData;
    // ArenaData dick[1983201082309123];

    arenaData.King = NULL;
    arenaData.arenaHead = NULL;
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
    for (Arena *it = arenaData.arenaHead; it != NULL; it = it->next){
        live_arr[n++] = it->index;
    }

    qsort(live_arr, n, sizeof(int), cmp);
    printf("Final: ");
    for (int i = 0; i < n; i++){
        printf("%d%c",live_arr[i]," \n"[i == n - 1]);
    }


    
    
    return 0;
}
