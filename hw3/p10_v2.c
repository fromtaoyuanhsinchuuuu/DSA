#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#define MAX 1000

struct node{
    int health;
    int damage;
    int rank;
    int index;
    struct node *left;
    struct node *right;
};
typedef struct node Node;

Node *genNode(int health) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->health = health;
    node->damage = 0;
    node->rank = 0;
    node->index = 0;
    node->left = NULL;
    node->right = NULL;
    return node;
}

struct knight{
    int health;
    int attack;
    int totaldamage;
    int position;
    int indexInPlace;
    int rank;
    int ans;
    struct knight *parent;
    Node *root;
};
typedef struct knight Knight;

Knight *genKnight(int health, int i){
    Knight *node = (Knight *)malloc(sizeof(Knight));
    node->totaldamage = 0;
    node->health = health;
    node->attack = 0;
    node->position = i;
    node->indexInPlace = 1;
    node->rank = 0;
    node->parent = NULL;
    node->root = genNode(health);
    node->ans = 0;
    return node;
}

void Make_set(Knight *knight){
    knight->parent = knight;
    knight->rank = 0;
}

int part_ans = 0;
Knight *Find_set(Knight *x){
    if(x != x->parent){
        x->parent = Find_set(x->parent);
    }
    else{
        part_ans = 0;
    }
    if(x->parent != x){
        part_ans += x->ans;
        x->ans = part_ans;
    }
    return (x->parent);
}


Node* merge(Node* h1, Node* h2) {
    if (h1 == NULL)
        return h2;
    if (h2 == NULL)
        return h1;

    if (h1->health - h1->damage > h2->health - h2->damage) {
        Node* temp = h1;
        h1 = h2;
        h2 = temp;
    }
    h2->damage -= h1->damage;
    h1->right = merge(h1->right, h2);

    if (h1->left == NULL || h1->left->rank < h1->right->rank) {
        Node* temp = h1->left;
        h1->left = h1->right;
        h1->right = temp;
    }

    if (h1->right == NULL)
        h1->rank = 0;
    else
        h1->rank = h1->right->rank + 1;

    return h1;
}

void Union(Knight *knight1, Knight *knight2){
    Knight *x = knight1->parent;
    Knight *y = knight2->parent;
    if(x->rank > y->rank){
        y->parent = x;
        //knight1->root->damage += knight2->root->damage;
        x->totaldamage += y->totaldamage;
        y->ans -= x->ans;
        x->root = merge(x->root, y->root);
    }
    else{
        x->parent = y;
        //knight2->root->damage += knight1->root->damage;
        y->totaldamage += x->totaldamage;
        x->ans -= y->ans;
        y->root = merge(x->root, y->root);
        if(x->rank == y->rank){
            y->rank = y->rank + 1;
        }
    }
}
Node* deleteMin(Node* root, int ans[MAX], Knight *knight[MAX]) {
    if (root == NULL){
        return NULL;
    }
    Node* leftChild = root->left;
    Node* rightChild = root->right;
    if(leftChild != NULL){              
        leftChild->damage += root->damage;
    }
    if(rightChild != NULL){
        rightChild->damage += root->damage;
    }
    Knight *add = Find_set(knight[root->index]);
    if(knight[root->index] == add){
        assert(knight[root->index]->parent == knight[root->index]);
        ans[root->index] = knight[root->index]->ans;
    }
    else{
        int to_print = knight[root->index]->ans + add->ans;
        ans[root->index] = to_print;
    }
    add->totaldamage -= knight[root->index]->attack;
    free(root);
    return merge(leftChild, rightChild);
}

int main(){
    int N, M;
    scanf("%d%d", &N, &M);
    int health[MAX], damage[MAX];
    Knight *knight[MAX];
    int ans[MAX];
    for(int i = 1;i <= N;i ++){
        scanf("%d", &health[i]);
        knight[i] = genKnight(health[i], i);
        Make_set(knight[i]);
        knight[i]->root->index = i;
        ans[i] = -1;
    }
    for(int i = 1;i <= N;i ++){
        scanf("%d", &damage[i]);
        knight[i]->totaldamage = damage[i];
        knight[i]->attack = damage[i];
        knight[i]->root->damage = 0;
    }
    for(int i = 1;i <= M;i ++){
        int start, end;
        scanf("%d%d", &start, &end);
        Knight *x = Find_set(knight[start]);
        Knight *y = Find_set(knight[end]);
        if((ans[start] == -1 && ans[end] == -1) && (x != y)){
            x->ans ++;      //only for subtask 2
            y->root->damage += x->totaldamage;
            while((y->root != NULL) && (y->root->health - y->root->damage <= 0)){
                y->root = deleteMin(y->root, ans, knight);
            }
            Union(x, y);
        }
    }
    for(int i = 1;i <= N;i ++){
        if(ans[i] == -1){
            Knight *add = Find_set(knight[i]);
            if(knight[i] == add){
                printf("%d ", knight[i]->ans);
            }
            else{
                int to_print = knight[i]->ans + add->ans;
                printf("%d ", to_print);
            }
        }
        else{
            printf("%d ", ans[i]);
        }
    }
    return 0;
}