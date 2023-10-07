#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>

#define Q 1000000007
#define MAXN 100000
#define BIG 1
#define SMALL 0

typedef long long ll;

int odd_or_even = 0;

int abs(int a) {return (a >= 0)? a: -a;}

// Structure for a node of the AVL tree
typedef struct Node {
    int price[2]; /* 1:bigger 0:smaller */
    int key, type, height;
    struct Node *parent, *left, *right;
} Node;

Node *TreeNode[MAXN];

// Function to calculate the height of a node
int getHeight(Node* node) {
    if (node == NULL)
        return 0;
    return node->height;
}

// Function to update the height of a node
void updateHeight(Node* node) {
    int leftHeight = getHeight(node->left);
    int rightHeight = getHeight(node->right);
    node->height = (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
}

// Function to rotate a subtree to the left
Node* rotateLeft(Node* node) {
    Node* newNode = node->right;
    node->right = newNode->left;
    if (newNode->left != NULL)
        newNode->left->parent = node;
    newNode->left = node;
    newNode->parent = node->parent;
    node->parent = newNode;
    updateHeight(node);
    updateHeight(newNode);
    return newNode;
}

// Function to rotate a subtree to the right
Node* rotateRight(Node* node) {
    Node* newNode = node->left;
    node->left = newNode->right;
    if (newNode->right != NULL)
        newNode->right->parent = node;
    newNode->right = node;
    newNode->parent = node->parent;
    node->parent = newNode;
    updateHeight(node);
    updateHeight(newNode);
    return newNode;
}

// Function to balance the AVL tree
Node* balance(Node* node) {
    updateHeight(node);
    int balanceFactor = getHeight(node->left) - getHeight(node->right);
    if (balanceFactor > 1) {
        if (getHeight(node->left->left) >= getHeight(node->left->right))
            node = rotateRight(node);
        else {
            node->left = rotateLeft(node->left);
            node = rotateRight(node);
        }
    } else if (balanceFactor < -1) {
        if (getHeight(node->right->right) >= getHeight(node->right->left))
            node = rotateLeft(node);
        else {
            node->right = rotateRight(node->right);
            node = rotateLeft(node);
        }
    }
    
    return node;
}

Node *genNode(int priceA, int priceB, int type)
{
    if (priceA < priceB){
        int tmp = priceA;
        priceA = priceB;
        priceB = tmp;
    }
    Node *node = (Node *)malloc(sizeof(Node));
    node->height = 1, node->key = abs(priceA - priceB), node->price[1] = priceA, node->price[0] = priceB, node->type = type;
    node->left = NULL, node->right = NULL, node->parent = NULL;
    return node;
}

// Function to insert a key into the AVL tree
Node* insert(Node* root, int priceA, int priceB, int type) {
    int key = abs(priceA - priceB);
    if (root == NULL) {
        Node *node = genNode(priceA, priceB, type);
        TreeNode[type] = node;
        return node;
    }
    if (key < root->key || key == root->key && type > root->type) {
        root->left = insert(root->left, priceA, priceB, type);
        root->left->parent = root;
    } 
    else if (key > root->key || key == root->key && type > root->type) {
        root->right = insert(root->right, priceA, priceB, type);
        root->right->parent = root;
    } 
    return balance(root);
}


// Function to find a key in the AVL tree
Node* search(Node* root, int key) {
    if (root == NULL || root->key == key)
        return root;
    
    if (key < root->key)
        return search(root->left, key);
    else
        return search(root->right, key);
}

Node* findMin(Node* root) {
    if (root == NULL)
        return NULL;
    
    while (root->left != NULL)
        root = root->left;
    
    return root;
}

// Function to delete a key from the AVL tree
Node* delete(Node* root, int key) {
    if (root == NULL)
        return root;
    
    if (key < root->key)
        root->left = delete(root->left, key);
    else if (key > root->key)
        root->right = delete(root->right, key);
    else {
        // Node to be deleted found
        if (root->left == NULL && root->right == NULL) {
            // Case 1: Node has no children
            free(root);
            root = NULL;
        } else if (root->left == NULL) {
            // Case 2: Node has only right child
            Node* temp = root;
            root = root->right;
            root->parent = temp->parent;
            free(temp);
        } else if (root->right == NULL) {
            // Case 2: Node has only left child
            Node* temp = root;
            root = root->left;
            root->parent = temp->parent;
            free(temp);
        } else {
            // Case 3: Node has both left and right children
            Node* minRight = findMin(root->right);
            root->key = minRight->key, root->price[0] = minRight->price[0], root->price[1] = minRight->price[1], root->type = minRight->type, TreeNode[minRight->type] = root;
            root->right = delete(root->right, minRight->key);
        }
    }
    
    if (root != NULL)
        root = balance(root);
    
    return root;
}

ll getANS(Node *root)
{
    return (root == NULL)? 0: getANS(root->right) + root->price[(odd_or_even++) % 2] + getANS(root->left);
}

// void modify_price(Node *node, int c, int d, int e, int f, int P)
// {
//     int priceA = (c * P + d) % Q, priceB = (e * P + f) % Q;
//     if (priceA < priceB){
//         int tmp = priceA;
//         priceA = priceB;
//         priceA = tmp;
//     }
//     node->price[0] = priceB, node->price[1] = priceA;
//     return;
// }

int main(void)
{
    int N, M;
    scanf("%d%d", &N, &M);

    Node *root = NULL;
    int priceA, priceB;
    for (int i = 1; i <= N; i++){
        scanf("%d%d", &priceA, &priceB);
        root = insert(root, priceA, priceB, i);
    }

    ll ans = getANS(root);
    odd_or_even = 0;
    printf("%lld\n", ans);

    int type, c, d, e, f;
    for (int i = 0; i < M - 1; i++){
        // scanf("%d%d%d%d%d", &type, &c, &d, &e, &f);
        // root = delete(root, TreeNode[type]->key);
        // root = insert(root, (c * (ans % Q) + d) % Q, (e * (ans % Q) + f) % Q, type);
        // ans = getANS(root);
        // odd_or_even = 0;
        // printf("%lld\n", ans);
    }
    return 0;
}
