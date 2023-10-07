#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>

#define MAXN 10000

typedef struct Node{
    int key;
    struct Node *l, *r;
} Node;

Node *genNode(int key)
{
    Node *node = malloc(sizeof(Node));
    node->key = key;
    node->l = NULL, node->r = NULL;
    return node;
}



Node *construct_BST(int arr[MAXN], int l, int r, int mode)
{
    int m = (mode == 0)? (int)((l + r) / 2): (int)ceil(((l + r) /(double) 2));
    Node *node = genNode(arr[m]);
    printf("%d", m);
    if (mode == 0 && m == l )
    if (m != l)
        node->l = construct_BST(arr, l, m, 0);
    if (m != r)
        node->r = construct_BST(arr, m, r, 1);
    return node;
}

void printInOrder(Node *root)
{
    if (root == NULL) return;
    printInOrder(root->l);
    printf("%d ", root->key);
    printInOrder(root->r);
    return ;
}


int main(void)
{
    int n;
    scanf("%d", &n);

    int arr[MAXN];
    for (int i = 0; i < n; i++){
        scanf("%d", &arr[i]);
    }

    Node *root = construct_BST(arr, 0, n - 1, 0);
    printInOrder(root);


    
    return 0;
}
