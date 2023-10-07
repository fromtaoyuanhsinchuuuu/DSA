#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>

#define MAXN 1000

int main(void)
{
    int n;
    scanf("%d",&n);

    int arr[MAXN];
    for (int i = 1; i <= n; i++){
        scanf("%d",&arr[i]);
    }

    int i = 1,j = 2;

    int pair_num = 0;
    while (j <= n){
        if (arr[i] == -1){
            i++;
            continue;
        }
        if (arr[j] == arr[i] * 2){
            printf("%d %d\n",arr[i],arr[j]);
            arr[i] = -1;
            arr[j] = -1;
            i++;
            pair_num++;
        }
        j++;
    }

    if (pair_num == n / 2){
        printf("yes\n");
    }
    else{
        printf("no!\n");
    }

   
    
    return 0;
}
