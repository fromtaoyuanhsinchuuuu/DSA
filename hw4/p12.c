#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>

#define d 26
#define MAXM 2000 // 2000005
#define P 1000213
#define P1 1000211
#define P2 1000039
#define P3 1000037
#define INSERT 1
#define DELETE 2

int m;

typedef long long ll;

int d_power_arr[MAXM];

typedef struct Node{
    int num;
    char *str;
    struct Node *next;
} Node;

Node *hash_table[P];

void set_d_power_arr()
{
    int num = 1;
    for (int i = 0; i <= m; i++){
        d_power_arr[i] = num;
        num = (num * d) % P;
    }
    return;
}

void set_hash_table()
{
    for (int i = 0; i < P; i++) hash_table[i] = NULL;
    return;
}

int min_hash_value(char str[MAXM])
{
    int min = 0;
    for (int i = 0; i < m; i++){
        min += (str[i] - 'a') * d_power_arr[m - i - 1];
        min %= P;
    }

    char ans_str[MAXM];

    strcpy(ans_str, str);
    int tmp_hash = min;

    for (int i = m - 1, it = 1; it <= m - 1; it++, i++){
        str[i + 1] = str[i - m + 1], str[i + 2] = '\0';
        tmp_hash += P;
        tmp_hash -= (str[i - m + 1] - 'a') * d_power_arr[m - 1] % P;
        tmp_hash = (tmp_hash * d) % P;
        tmp_hash += str[i - m + 1] - 'a';

        // printf("tmp_hash:%d\n", tmp_hash);


        if (tmp_hash < min){
            min = tmp_hash;
            strcpy(ans_str, str + it);
        }
        else if (tmp_hash == min && strcmp(ans_str, str + it) > 0){
            strcpy(ans_str, str + it);
        }

    }
    strcpy(str, ans_str);
    return min;
}

Node *genNode(char *str)
{
    Node *node = malloc(sizeof(Node));
    node->next = NULL, node->num = 1, node->str = malloc((m + 1) * sizeof(char));
    strcpy(node->str, str);
    return node;
}

Node *find_Node(Node *node, char *str, ll *before_num, ll *after_num, int mode)
{
    if (node == NULL){
        *before_num = 0;
        *after_num = 1;
        return genNode(str);
    }

    // printf("tmp:%s\n", node->str);
    if (strcmp(node->str, str)) node->next = find_Node(node->next, str, before_num, after_num, mode);

    else{
        if (mode == INSERT){ 
            *before_num = node->num;
            node->num++;
            *after_num = node->num;
        }
        else if (mode == DELETE){ 
            *before_num = node->num;
            node->num--;
            *after_num = node->num;
        }
    }
    return node;
}

ll combination(ll n)
{
    if (n % 2 == 0) return (n / 2) * (n - 1);
    else return ((n - 1) / 2) * n;
}

int main(void)
{
    int N, Q;
    scanf("%d%d", &N, &Q);

    set_hash_table();

    char str[MAXM];
    ll ans_pair = 0, before_num = -1, after_num = -1;
    for (int i = 0; i < N; i++){
        scanf("%s", str);
        if (i == 0){
            m = strlen(str);
            set_d_power_arr();
        }

        int min_value = min_hash_value(str);
        // printf("min%d\n", min_value);

        before_num = -1, after_num = -1;
        hash_table[min_value] = find_Node(hash_table[min_value], str, &before_num, &after_num, INSERT);
        // printf("before:%d after:%d\n", before_num, after_num);
        ans_pair = ans_pair - combination(before_num) + combination(after_num);
    }
    printf("%lld\n", ans_pair);

    int mode = -1;
    for (int i = 0; i < Q; i++){
        scanf("%d %s", &mode, str);
        int min_value = min_hash_value(str);
        hash_table[min_value] = find_Node(hash_table[min_value], str, &before_num, &after_num, mode);
        ans_pair = ans_pair - combination(before_num) + combination(after_num);
        printf("%lld\n", ans_pair);
    }
    return 0;
}
