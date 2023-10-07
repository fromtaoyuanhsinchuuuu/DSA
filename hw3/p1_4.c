#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>

#define MAX 100

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

int main(void)
{
    char key[MAX], S[MAX];
    scanf("%s%s", key + 1, S + 1);

    int m = strlen(key + 1), n = strlen(S + 1);
    
    int pi[MAX];
    prefix_function(m, pi, key);

    printf("m:%d\n", m);
    for (int i = 1; i <= m; i++){
        printf("%d ", pi[i]);
    }

    int i = 1, j = 1;
    while (n - i >= m - j){
        if (key[j] > S[i]){
            printf("index:%d\n", i - j);
            return 0;
        }
        else if (key[j] == S[i]){
            j++, i++;
        }
        else if (i <= n && key[j] < S[i]){
            if (j != 1)
                j = pi[j - 1];
            else
                i++;
        }
    }

    
    return 0;
}
