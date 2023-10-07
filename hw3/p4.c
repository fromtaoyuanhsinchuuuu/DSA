#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>

#define MAX 1000

void prefix_function(int len, int pi[MAX], char str[MAX])
{
    pi[1] = 0;
    int k = 0;
    for (int q = 2; q <= len; q++){
        while (k > 0 && str[k + 1] != str[q]){
            k = pi[k];
        }
        if (str[k + 1] == str[q]){
            k++;
        }
        pi[q] = k;
    }
    return;
}

void KMP_matching(int N, int M, char strS[MAX], char strP[MAX], int pi[MAX], int KMP_arr[MAX])
{
    int q = 0;
    for (int i = 1; i <= N; i++){
        while (q > 0 && strP[q + 1] != strS[i]){
            q = pi[q];
        }
        if (strP[q + 1] == strS[i]){
            q++;
        }
        KMP_arr[i] = q;
        if (q == M){
            q = pi[q];
        }
    }
    return;
}

void reverse_str(int N, char str[MAX])
{
    for (int i = 1; i <= N / 2; i++){
        char c = str[i];
        str[i] = str[N + 1 - i];
        str[N + 1 - i] = c;
    }
    return;
}

void reverse_arr(int N, int arr[MAX])
{
    for (int i = 1; i <= N / 2; i++){
        int tmp = arr[i];
        arr[i] = arr[N + 1 - i];
        arr[N + 1 - i] = tmp;
    }
    return;
}

int main(void)
{
    int N, M;
    scanf("%d%d", &N, &M);

    char strS[MAX], strP[MAX];
    scanf("%s", strS + 1),scanf("%s", strP + 1);

    int pi_P[MAX],pi_P_reverse[MAX], KMP_arr[MAX], KMP_reverse_arr[MAX];
    prefix_function(M, pi_P, strP);
    KMP_matching(N, M, strS, strP, pi_P, KMP_arr);

    for (int i = 1; i <= N; i++){
        printf("%d ", KMP_arr[i]);
    }

    printf("\n");
    reverse_str(M, strP), reverse_str(N, strS);
    prefix_function(M, pi_P_reverse, strP);
    KMP_matching(N, M, strS, strP, pi_P_reverse, KMP_reverse_arr);
    for (int i = 1; i <= N; i++){
        printf("%d ", KMP_reverse_arr[i]);
    }
    reverse_arr(N, KMP_reverse_arr);

    int ans = 0;
    printf("\n");
    for (int i = 1; i <= N - M + 1; i++){
        printf("%d %d\n", KMP_reverse_arr[i], KMP_arr[i + M - 1]);
        int key = KMP_reverse_arr[i] + KMP_arr[i + M - 1];
        if (key == M || key == M * 2)
            ans++;
    }
    printf("%d\n", ans);
    return 0;
}
