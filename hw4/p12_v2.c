#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>

#define d 26
#define MAXM 2000005 // 2000005
#define P 10017193
#define P1 10017191
#define P2 10017109
#define P3 10017107
#define INSERT 1
#define DELETE 2

int m;

typedef long long ll;

int d_power_arr[4][MAXM];

int prime_arr[4] = {P, P1, P2, P3};

typedef struct Node{
    int num;
    int hash_num[4];
    struct Node *next;
} Node;

Node *hash_table[P];

void set_d_power_arr()
{
    int num[4] = {1, 1, 1, 1};
    for (int i = 0; i <= m; i++){
        for (int j = 0; j < 4; j++){
            d_power_arr[j][i] = num[j];
            num[j] *= d;
            num[j] %= prime_arr[j];
        }
    }
    return;
}

void print_prime(int tmp_hash[4])
{
    for (int i = 0; i < 4; i++){
        printf("%d%c", tmp_hash[i], " \n"[i == 3]);
    }
}

void set_hash_table()
{
    for (int i = 0; i < P; i++) hash_table[i] = NULL;
    return;
}

void min_hash_value(char str[MAXM], int min[4], int tmp_hash[4])
{
    
    for (int i = 0; i < m; i++){
        for (int j = 0; j < 4; j++){
            if (i == 0) min[j] = 0;
            min[j] += (str[i] - 'a') * d_power_arr[j][m - i - 1];
            min[j] %= prime_arr[j];
            if (i == m - 1) tmp_hash[j] = min[j];
        }
    }
    for (int i = m - 1, it = 1; it <= m - 1; it++, i++){
        str[i + 1] = str[i - m + 1], str[i + 2] = '\0';
        for (int j = 0; j < 4; j++){
            tmp_hash[j] += prime_arr[j];
            tmp_hash[j] -= ((str[i - m + 1] - 'a') * d_power_arr[j][m - 1]) % prime_arr[j];
            tmp_hash[j] = (tmp_hash[j] * d) % prime_arr[j];
            tmp_hash[j] += (str[i - m + 1] - 'a');
        }
        for (int j = 0; j < 4; j++){
            if (tmp_hash[j] < min[j]){
                for (int k = 0; k < 4; k++){
                // printf("tmphash:%d min:%d\n", tmp_hash[k], min[k]);
                    min[k] = tmp_hash[k];
                }
                break;
            }
            else if (tmp_hash[j] > min[j]){
                break;
            }
        }
    }
    return;
}

Node *genNode(int hash_num[4])
{
    Node *node = malloc(sizeof(Node));
    node->next = NULL, node->num = 1;
    for (int i = 0; i < 4; i++) node->hash_num[i] = hash_num[i];
    return node;
}

bool same_str(Node *node, int hash_num[4])
{
    for (int i = 0; i < 4; i++){
        if (node->hash_num[i] != hash_num[i]) return 0;
    }
    return 1;
}

Node *find_Node(Node *node, ll *before_num, ll *after_num, int mode, int hash_num[4])
{
    if (node == NULL){
        *before_num = 0;
        *after_num = 1;
        return genNode(hash_num);
    }

    // printf("tmp:%s\n", node->str);
    if (!same_str(node, hash_num)) node->next = find_Node(node->next, before_num, after_num, mode, hash_num);
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
    int min[4] = {};
    int tmp_hash[4] = {};
    ll ans_pair = 0, before_num = -1, after_num = -1;
    for (int i = 0; i < N; i++){
        scanf("%s", str);
        if (i == 0){
            m = strlen(str);
            set_d_power_arr();
        }
        min_hash_value(str, min, tmp_hash);
        before_num = -1, after_num = -1;
        hash_table[min[0]] = find_Node(hash_table[min[0]], &before_num, &after_num, INSERT, min);
        ans_pair = ans_pair - combination(before_num) + combination(after_num);
    }
    printf("%lld\n", ans_pair);

    int mode = -1;
    for (int i = 0; i < Q; i++){
        scanf("%d %s", &mode, str);
        min_hash_value(str, min, tmp_hash);
        hash_table[min[0]] = find_Node(hash_table[min[0]], &before_num, &after_num, mode, min);
        ans_pair = ans_pair - combination(before_num) + combination(after_num);
        printf("%lld\n", ans_pair);
    }
    return 0;
}