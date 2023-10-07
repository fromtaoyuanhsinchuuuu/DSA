#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>

#define MAXM 10000

int main(void)
{
    int n;
    scanf("%d", &n);

    int arr[MAXM];
    for (int i = 1; i <= n - 2; i++){
        scanf("%d",&arr[i]);
    }

    /* 先找左邊 */
    int l = 1, r = n - 2,find = 0;

    /* 處理edge case:有東西在最右邊, */
    if (arr[r] == r){
        printf("%d %d",r + 1, r + 2);
        find += 2;
    }
    else if (arr[r] == r + 1){
        printf("%d ", r + 2);
        find += 1;
    }

    int m = (l + r) / 2;
    while (m > l){
        if (arr[m] == m){ // 在右邊
            l = m;
        }
        else{
            r = m;
        }
        m = (l + r) / 2;
    }

    if (arr[l] == l){
        printf("%d ",arr[l] + 1);
        find += 1;
    }
    else{ // 不見的在最左邊
        assert(l == 1);

        if (arr[l] == 2){ 
            printf("1 ");
            find += 1;
        }
        else if (arr[l] == 3){
            printf("1 2");
            find += 2;
        }
        else{
            assert(0);
        }
    }

    if (find == 2){
        exit(1);
    }

    l = 1,r = n - 2;
    m = ((l + r) % 2 == 0)? (l + r) / 2: (l + r) / 2 + 1;

    /* 再找右邊 */
    while (m < r){
        if (arr[m] == m + 2){ // 在左邊
            r = m;
        }
        else{
            l = m;
        }
        m = ((l + r) % 2 == 0)? (l + r) / 2: (l + r) / 2 + 1;
    }

    assert(arr[r] == r + 2);
    printf("%d",arr[r] - 1);
    
    return 0;
}
