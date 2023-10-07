#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>

#define MAXN 2000

typedef struct Node{
    int index, d_damage, HP, rank;
    struct Node *left, *right;
} Node;

typedef struct knight_set{
    int rank;
    struct knight_set *parent;
    Node *heap_root;
    int total_attack; // for root
    int d_ans, index, attack;
} Knight_set;

Knight_set *Knight_arr[MAXN];
int ans_arr[MAXN] = {};

Node *merge(Node *root1, Node *root2) { // return root
    if (root1 == NULL) return root2;
    if (root2 == NULL) return root1;
    if (root1->HP - root1->d_damage > root2->HP - root2->d_damage) {
        Node* temp = root1;
        root1 = root2;
        root2 = temp;
    }

    /* root1會比較小 */
    root2->d_damage -= root1->d_damage;
    root1->right = merge(root1->right, root2);
    if (root1->left == NULL || root1->left->rank < root1->right->rank) {
        Node* temp = root1->left;
        root1->left = root1->right;
        root1->right = temp;
    }
    if (root1->right == NULL)
        root1->rank = 0;
    else
        root1->rank = root1->right->rank + 1;
    return root1;
}

Knight_set *Find_set(Knight_set *x, int *d_ans)
{
    assert(x != NULL);
    if (x != x->parent) x->parent = Find_set(x->parent, d_ans);

    assert(x->parent->parent = x->parent); // x.parent == root!
    if (x->parent != x){
        *d_ans += x->d_ans;
        x->d_ans = *d_ans;
    }
    return x->parent;
}

// Function to delete the minimum element from the Leftist Tree
Node *deleteMin(Node *root) {
    if (root == NULL) return NULL;

    Node *left = root->left;
    if (left != NULL) left->d_damage += root->d_damage;

    Node *right = root->right;
    if (right != NULL) right->d_damage += root->d_damage;

    int d_ans = 0;
    Find_set(Knight_arr[root->index], &d_ans)->total_attack -= Knight_arr[root->index]->attack;
    ans_arr[root->index] = (Knight_arr[root->index]->parent == Knight_arr[root->index])? Knight_arr[root->index]->d_ans: Knight_arr[root->index]->d_ans + Knight_arr[root->index]->parent->d_ans;
    free(root);
    return merge(left, right);
}

Knight_set *genKnight(int index)
{
    Knight_set *knight = malloc(sizeof(Knight_set));
    Node *heap = malloc(sizeof(Node));
    heap->index = index, heap->d_damage = 0, heap->HP = 0, heap->left = NULL, heap->right = NULL, heap->rank = 0;
    knight->index = index, knight->d_ans = 0, knight->total_attack = 0, knight->rank = 0, knight->parent = knight;
    knight->heap_root = heap;
    return knight;
}

void Link(Knight_set *knight1, Knight_set *knight2)
{
    if (knight1->rank > knight2->rank){ // knight2 link to knight1
        knight1->heap_root = merge(knight1->heap_root, knight2->heap_root);
        knight2->d_ans -= knight1->d_ans;
        knight2->parent = knight1;
        knight1->total_attack += knight2->total_attack;
    }
    else{
        knight2->heap_root = merge(knight1->heap_root, knight2->heap_root);
        knight1->d_ans -= knight2->d_ans;
        knight1->parent = knight2;
        knight2->total_attack += knight1->total_attack;
        if (knight1->rank == knight2->rank) knight2->rank++;
    }
    return;
}

int main(void)
{
    int n, m;
    scanf("%d%d", &n, &m);

    for (int i = 1; i <= n; i++){
        Knight_arr[i] = genKnight(i);
        ans_arr[i] = -1;
        scanf("%d", &(Knight_arr[i]->heap_root->HP));
    }

    int at;
    for (int i = 1; i <= n; i++){
        scanf("%d", &at);
        Knight_arr[i]->attack = at, Knight_arr[i]->total_attack = at;
    }

    int index1, index2;
    int d_ans = 0, d_damage = 0;
    for (int it = 0; it < m; it++){
        scanf("%d%d", &index1, &index2);
        Knight_set *knight1 = Knight_arr[index1], *knight2 = Knight_arr[index2];

        d_ans = 0, d_damage = 0;
        Knight_set *root1 = Find_set(knight1, &d_ans);
        d_ans = 0, d_damage = 0;
        Knight_set *root2 = Find_set(knight2, &d_ans);

        if (root1 == root2) continue;
        if (ans_arr[index1] != -1 || ans_arr[index2] != -1) continue;

        root1->d_ans++;
        root2->heap_root->d_damage += root1->total_attack;
        while (root2->heap_root != NULL && root2->heap_root->HP <= root2->heap_root->d_damage){
            root2->heap_root = deleteMin(root2->heap_root);
        }
        Link(root1, root2);
    }

    for (int i = 1; i <= n; i++){
        d_ans = 0, d_damage = 0;
        Knight_set *x = Knight_arr[i];
        if (ans_arr[i] != -1){
            printf("%d ",ans_arr[i]);
            continue;
        }
        Find_set(x, &d_ans);
        printf("%d ", (x->parent == x)? x->d_ans: x->d_ans + x->parent->d_ans);
    }
    return 0;
}
