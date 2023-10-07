#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>

#define MAXN 100005

typedef long long ll;

void reverse(int T[],int l,int r)
{
    for (int i = l; i <= (r + l) / 2; i++){
        int temp = T[i];
        T[i] = T[r + l - i];
        T[r + l - i] = temp;
    }
    return;
}

void merge(int A[], int l, int m, int r)
{
    int T[MAXN];

    r--;
    for (int i = l; i <= r; i++){
        T[i] = A[i];
    }

    reverse(T,m,r);

    int Left_index = l;
    int Right_index = r;

    for (int k = l; k <= r; k++){
        if (T[Left_index] <= T[Right_index]){
            A[k] = T[Left_index];
            Left_index++;
        }
        else{
            A[k] = T[Right_index];
            Right_index--;

        }
    }
}


ll count(int l, int r, int A[])
{
    if (l + 1 == r) return 0;
    int m = (l + r) / 2;
    ll cnt = count(l, m, A) + count(m, r, A);

    int right_index = m;
    for (int left_index = l; left_index < m; left_index++){
        while (right_index < r && A[right_index] < A[left_index]){
            right_index++;
        }
        cnt += right_index - m;
    }
    merge(A, l, m, r);
    return cnt;
}

int main(void)
{

    int Case = 1;
    while (1){
        int n;
        scanf("%d", &n);

        if (n == 0) break;

        int A[MAXN];
        for (int i = 0; i < n; i++){
            scanf("%d", &A[i]);
        }
        printf("Case #%lld: %lld\n", Case, count(0, n, A));
        Case++;
    }
    return 0;
}
