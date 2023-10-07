#include <stdio.h>
#include <stdlib.h>

int djs[16];
int cnt = 0;

void djs_make_set(int x)
{
    djs[x] = x;
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
    if (x == y) return;
    if (rand() % 2 == 0)
        djs[x] = y;
    else
        djs[y] = x;
    printf("cnt:%d\n", cnt);
}

int main()
{
    srand(1126);
    for(int i = 0; i < 16; i++) djs_make_set(i);

    // Modify the following sample test case
    djs_union(1, 0);
    djs_union(2, 0);
    djs_union(3, 0);
    djs_union(0, 4);
    djs_union(0, 0);
    djs_union(0, 0);
    djs_union(0, 0);
    djs_union(0, 0);
    djs_union(0, 0);
    djs_union(0, 0);
    djs_union(0, 0);
    djs_union(0, 0);
    djs_union(0, 0);
    djs_union(0, 0);
    djs_union(0, 0);
    // ---------------------------

    printf("cnt = %d\n", cnt);
}