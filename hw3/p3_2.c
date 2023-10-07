#include <stdio.h>
#include <stdlib.h>

int djs[16], ind[16];
int cnt = 0;

void djs_make_set(int x)
{
    djs[x] = x;
    ind[x] = rand();
}

int djs_find_set(int x)
{
    cnt++;
    if (djs[x] == x) return x;
    return djs_find_set(djs[x]);
}

void djs_union(int x, int y)
{
    x = djs_find_set(x), y = djs_find_set(y);
    // printf("x:%d y:%d\n",x, y);
    // printf("cnt:%d\n", cnt);
    if (x == y) return;
    if (ind[x] < ind[y])
        djs[x] = y;
    else
        djs[y] = x;
}

int main()
{
    srand(1126);
    for(int i = 0; i < 16; i++) djs_make_set(i);

    for (int i = 0; i < 16; i++) printf("%d:%d\n", i, ind[i]);

    // Modify the following sample test case
    djs_union(10, 0);
    djs_union(10, 6);
    djs_union(10, 14);
    djs_union(10, 15);
    djs_union(10, 10);
    djs_union(10, 10);
    djs_union(10, 10);
    djs_union(10, 10);
    djs_union(10, 10);
    djs_union(10, 10);
    djs_union(10, 10);
    djs_union(10, 10);
    djs_union(10, 10);
    djs_union(10, 10);
    djs_union(10, 10);

    // ---------------------------

    printf("cnt = %d\n", cnt);
}