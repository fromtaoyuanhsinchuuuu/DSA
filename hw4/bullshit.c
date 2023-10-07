#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#define MAX 1000005        //1000005
#define prime1 8688571
#define prime2 8688553
#define prime3 8688569
#define prime4 8688551

typedef long long ll;

int M;
int power[4];

struct node{
    int num;
    int hash[4];
    struct node *next;
};
typedef struct node Node;

Node *hash_table[prime1];

Node *genNode(){
    Node *node = (Node *)malloc(sizeof(Node));
    node->num = 0;
    node->hash[0] = 0;
    node->hash[1] = 0;
    node->hash[2] = 0;
    node->hash[3] = 0;
    node->next = NULL;
    return node;
}

int cal_power(int M, int prime){
    int power = 1;
    for(int i = 0;i < M - 1;i ++){
        power *= 26;
        power %= prime;
    }
    return power;
}

int hashing(char string[MAX], int prime){
    int hash_value = 0;
    for(int i = 0;i < M;i ++){
        hash_value = (hash_value * 26 + (string[i] - 'a')) % prime;
    }
    return hash_value;
}

int smallest_hash(char string[MAX], int power, int prime){
    int prev_hash = hashing(string, prime);
    int smallest = prev_hash;
    int loop_time = M;
    for(int i = 0;i < loop_time;i ++){
        prev_hash -= ((string[i] - 'a') * power) % prime;
        prev_hash += prime;
        prev_hash %= prime;
        prev_hash *= 26;
        prev_hash += (string[i] - 'a');
        prev_hash %= prime;

        if(prev_hash < smallest){
            smallest = prev_hash;
        }
    }
    return smallest;
}

Node *chain(Node *node, int hash, char string[MAX], int power[4], ll *ans){
    ll return_ans = *ans;
    if(node == NULL){
        node = genNode();
        node->num ++;
        node->hash[0] = hash;
        node->hash[1] = smallest_hash(string, power[1], prime2);
        node->hash[2] = smallest_hash(string, power[2], prime3);
        node->hash[3] = smallest_hash(string, power[3], prime4);
    }
    else if(node->hash[0] == hash && node->hash[1] == smallest_hash(string, power[1], prime2) && node->hash[2] == smallest_hash(string, power[2], prime3) && node->hash[3] == smallest_hash(string, power[3], prime4)){
        assert(node->num >= 0);
        node->num ++;
        if(node->num % 2 == 1){
            return_ans -= ((node->num - 1) / 2) * (node->num - 2);
            return_ans += (node->num * ((node->num - 1) / 2));
        }
        else{
            return_ans -= (node->num - 1) * ((node->num - 2) / 2);
            return_ans += (node->num / 2) * (node->num - 1);
        }
        *ans = return_ans;
    }
    else{
        node->next = chain(node->next, hash, string, power, ans);
    }
    return node;
}

void set_hash_table()
{
    for (int i = 0; i < prime1; i++) hash_table[i] = NULL;
}



int main(){
    int N, Q;
    scanf("%d%d", &N, &Q);

    char string[MAX];
    set_hash_table();

    ll ans = 0;
    for(int i = 0;i < N;i ++){
        scanf("%s", string);
        if(i == 0){
            M = strlen(string);
            power[0] = cal_power(M, prime1);
            power[1] = cal_power(M, prime2);
            power[2] = cal_power(M, prime3);
            power[3] = cal_power(M, prime4);
        }
        int hash = smallest_hash(string, power[0], prime1);
        hash_table[hash] = chain(hash_table[hash], hash, string, power, &ans);
        assert(hash_table[hash] != NULL);
    }
    printf("%lld\n", ans);
    for(int i = 0;i < Q;i ++){
        int step;
        scanf("%d", &step);
        scanf("%s", string);
        int hash = smallest_hash(string, power[0], prime1);
        if(step == 1){
            hash_table[hash] = chain(hash_table[hash], hash, string, power, &ans);
        }
        else{
            Node *node = hash_table[hash];
            while(!(node->hash[0] == smallest_hash(string, power[0], prime1) && node->hash[1] == smallest_hash(string, power[1], prime2) && node->hash[2] == smallest_hash(string, power[2], prime3) && node->hash[3] == smallest_hash(string, power[3], prime4))){
                node = node->next;
            }
            node->num --;
            if(node->num % 2 == 1){
                ans -= ((node->num + 1) / 2) * (node->num);
                ans += (node->num * ((node->num - 1) / 2));
            }
            else{
                ans -= (node->num + 1) * ((node->num) / 2);
                ans += (node->num / 2) * (node->num - 1);
            }
        }
        printf("%lld\n", ans);
    }
}