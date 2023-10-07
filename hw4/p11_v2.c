#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>

#define Q 1000000007
#define MAXN 100000

#define LEFT 0
#define RIGHT 1

#define BIG 1
#define SMALL 0

typedef long long ll;

bool parity = 0, final_parity = 0, root_right_null = 0;
ll ans = 0, ans2 = 0;


int abs(int a)
{
    return (a >= 0)? a: -a;
}

// Structure for a node of the AVL tree
typedef struct Node {
    ll price[2]; /* 0:smaller 1:bigger */
    int child_num[2];
    ll total_price[2][2];
    int key, type, height;
    struct Node *parent, *left, *right;
} Node;

Node *TreeNode[MAXN];

// Function to calculate the height of a node
int getHeight(Node *node)
{
    return (node == NULL)? 0: node->height;
}

int getCHILD(Node *node, int mode)
{
    return (node == NULL)? 0: node->child_num[mode];
}

// Function to update the height of a node
void updateHeight(Node *node) 
{
    int leftHeight = getHeight(node->left);
    int rightHeight = getHeight(node->right);
    node->height = (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
}

void add_total_price(Node *nowNode, Node *insertedNode, bool this_node_parity, bool final_node_parity, int dir)
{
    if (this_node_parity == final_node_parity){
        nowNode->total_price[dir][0] += insertedNode->price[0];
        nowNode->total_price[dir][1] += insertedNode->price[1];
    }
    else{
        nowNode->total_price[dir][0] += insertedNode->price[1];
        nowNode->total_price[dir][1] += insertedNode->price[0];
    }
    return;
}

// Function to rotate a subtree to the left
Node *rotateLeft(Node *x) 
{
    // return x;

    // printf("leftRotate:%d\n", x->key);
    Node *y = x->right;

    // printf("y_left_kid:%d\n", y->child_num[LEFT]);
    assert(y != NULL);

    bool x_and_y_sameparity = (((1 + getCHILD(y, LEFT)) % 2) == 0)? 1: 0;
    if (y->left != NULL) y->left->parent = x;

    x->child_num[RIGHT] = y->child_num[LEFT];
    y->child_num[LEFT] = 1 + x->child_num[RIGHT] + x->child_num[LEFT];

    if (x_and_y_sameparity){
        // printf("the same!\n");
        x->total_price[RIGHT][0] = y->total_price[LEFT][0];
        x->total_price[RIGHT][1] = y->total_price[LEFT][1];
        y->total_price[LEFT][0] = x->price[0] + x->total_price[LEFT][0] + x->total_price[RIGHT][0];
        y->total_price[LEFT][1] = x->price[1] + x->total_price[LEFT][1] + x->total_price[RIGHT][1]; 
    }
    else{
        // printf(" not the same!\n");

        x->total_price[RIGHT][1] = y->total_price[LEFT][0];
        x->total_price[RIGHT][0] = y->total_price[LEFT][1];
        y->total_price[LEFT][1] = x->price[0] + x->total_price[LEFT][0] + x->total_price[RIGHT][0];
        y->total_price[LEFT][0] = x->price[1] + x->total_price[LEFT][1] + x->total_price[RIGHT][1];
    }
    x->right = y->left;
    y->left = x;
    y->parent = x->parent;
    x->parent = y;
    updateHeight(x);
    updateHeight(y);
    return y;
}

// Function to rotate a subtree to the right
Node *rotateRight(Node *x) 
{
    // printf("rightRotate:%d\n", x->key);
    Node *y = x->left;
    assert(y != NULL);

    bool x_and_y_sameparity =  ((1 + getCHILD(y, RIGHT)) % 2 == 0)? 1: 0;
    if (y->right != NULL) y->right->parent = x;
    
    x->child_num[LEFT] = y->child_num[RIGHT];
    y->child_num[RIGHT] = 1 + x->child_num[RIGHT] + x->child_num[LEFT];

    if (x_and_y_sameparity){
        x->total_price[LEFT][0] = y->total_price[RIGHT][0];
        x->total_price[LEFT][1] = y->total_price[RIGHT][1];
        y->total_price[RIGHT][0] = x->price[0] + x->total_price[LEFT][0] + x->total_price[RIGHT][0];
        y->total_price[RIGHT][1] = x->price[1] + x->total_price[LEFT][1] + x->total_price[RIGHT][1]; 
    }
    else{
        x->total_price[LEFT][1] = y->total_price[RIGHT][0];
        x->total_price[LEFT][0] = y->total_price[RIGHT][1];
        y->total_price[RIGHT][1] = x->price[0] + x->total_price[LEFT][0] + x->total_price[RIGHT][0];
        y->total_price[RIGHT][0] = x->price[1] + x->total_price[LEFT][1] + x->total_price[RIGHT][1];
    }

    x->left = y->right;
    y->right = x;
    y->parent = x->parent;
    x->parent = y;
    updateHeight(x);
    updateHeight(y);

    return y;
}

// Function to balance the AVL tree
Node *balance(Node* node) {
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
    node->height = 1, node->key = abs(priceA - priceB), node->type = type;
    node->price[BIG] = priceA, node->price[SMALL] = priceB;
    node->child_num[LEFT] = 0, node->child_num[RIGHT] = 0;
    node->total_price[LEFT][0] = 0, node->total_price[LEFT][1] = 0;
    node->total_price[RIGHT][0] = 0, node->total_price[RIGHT][1] = 0;
    node->left = NULL, node->right = NULL, node->parent = NULL;
    return node;
}

void modify_price_insert(Node *ptr2)
{
    Node *ptr1 = ptr2->parent;
    // while (ptr1 != NULL){
        if (ptr1->right == ptr2){ // ptr2在右邊
            if (!(ptr2->child_num[LEFT] % 2)){ // not same
                ptr1->total_price[RIGHT][0] = ptr2->total_price[LEFT][1] + ptr2->total_price[RIGHT][1] + ptr2->price[1];
                ptr1->total_price[RIGHT][1] = ptr2->total_price[LEFT][0] + ptr2->total_price[RIGHT][0] + ptr2->price[0];

            }
            else{
                ptr1->total_price[RIGHT][0] = ptr2->total_price[LEFT][0] + ptr2->total_price[RIGHT][0] + ptr2->price[0];
                ptr1->total_price[RIGHT][1] = ptr2->total_price[LEFT][1] + ptr2->total_price[RIGHT][1] + ptr2->price[1];
            }
        }
        else{ // ptr2在左邊
            if (!(ptr2->child_num[RIGHT] % 2)){ // not same
                ptr1->total_price[LEFT][0] = ptr2->total_price[LEFT][1] + ptr2->total_price[RIGHT][1] + ptr2->price[1];
                ptr1->total_price[LEFT][1] = ptr2->total_price[LEFT][0] + ptr2->total_price[RIGHT][0] + ptr2->price[0];

            }
            else{
                ptr1->total_price[LEFT][0] = ptr2->total_price[LEFT][0] + ptr2->total_price[RIGHT][0] + ptr2->price[0];
                ptr1->total_price[LEFT][1] = ptr2->total_price[LEFT][1] + ptr2->total_price[RIGHT][1] + ptr2->price[1];
            }
        }
        // ptr2 = ptr1;
        // ptr1 = ptr1->parent;
    // }
    return;
}



// Function to insert a key into the AVL tree
Node *insert(Node *root, int priceA, int priceB, int type) 
{
    int key = abs(priceA - priceB);

    // if (root != NULL) printf("now_node:%d\n", root->key);
    // printf("ans:%d\n", ans);
    bool this_node_parity = parity;
    if (root == NULL) {
        Node *node = genNode(priceA, priceB, type);
        final_parity = this_node_parity;
        // printf("root_right_null:%d\n", root_right_null);
        // ans += root_right_null? node->price[!final_parity]: node->price[final_parity];
        // printf("added price:%d\n", node->price[final_parity]);
        // printf("this_node:%d\n", final_parity);
        TreeNode[type] = node;
        // printf("type:%d\n", TreeNode[type]->type);
        return node;
    }
    if (key < root->key || key == root->key && type < root->type){ // go left
        parity = ((1 + getCHILD(root->left, RIGHT)) % 2 == 0)? this_node_parity: !this_node_parity;
        root->left = insert(root->left, priceA, priceB, type);
        root->child_num[LEFT]++;
        root->left->parent = root;
        modify_price_insert(root->left);

        // printf("root:%d insertedNode:%d\n", this_node_parity, final_parity);
        // add_total_price(root, TreeNode[type], this_node_parity, final_parity, LEFT);
    } 
    else if (key > root->key || key == root->key && type > root->type){ // go right
        // ans = ans - root->total_price[LEFT][this_node_parity] + root->total_price[LEFT][!this_node_parity] + root->price[!this_node_parity] - root->price[this_node_parity];
        // printf("after moify ans:%d\n", ans);
        parity = ((1 + getCHILD(root->right, LEFT)) % 2 == 0)? this_node_parity: !this_node_parity;

        // bool root_right_null = 0;
        // if (root->right == NULL) 
        
        root->right = insert(root->right, priceA, priceB, type);



        root_right_null = 0;
        root->child_num[RIGHT]++;
        root->right->parent = root;
        modify_price_insert(root->right);
        // if (root->right->child_num[LEFT] + root->right->child_num[RIGHT] == 0) add_total_price(root, TreeNode[type], this_node_parity, !final_parity, RIGHT);
        // add_total_price(root, TreeNode[type], this_node_parity, final_parity, RIGHT);
    } 

    
    // return root;
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

void printTreeData(Node *root)
{
    if (root == NULL) return;

    bool this_node_parity = parity;

    parity = ((1 + getCHILD(root->right, LEFT)) % 2 == 0)? this_node_parity: !this_node_parity;
    printTreeData(root->right);

    // if (root->type == 5) exit(0);
    printf("parity:%d price:%d,%d left_num:%d left_total_price:%d,%d right_num:%d right_total_price:%d,%d\n", this_node_parity, root->price[0], root->price[1], root->child_num[LEFT], root->total_price[LEFT][0], root->total_price[LEFT][1], root->child_num[RIGHT], root->total_price[RIGHT][0], root->total_price[RIGHT][1]);

    parity = ((1 + getCHILD(root->left, RIGHT)) % 2 == 0)? this_node_parity: !this_node_parity;
    printTreeData(root->left);
    return;
}

void getANS(Node *root)
{
    Node *ptr = root;
    while (ptr != NULL){
        ans += ptr->total_price[LEFT][parity] + ptr->price[parity];
        ans2 += ptr->total_price[LEFT][!parity] + ptr->price[!parity];
        // printf("ptr:%d parity:%d\n", ptr->key, parity);
        parity = ((1 + getCHILD(ptr->right, LEFT)) % 2 == 0)? parity: !parity;
        ptr = ptr->right;
    }
    return;
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
        // scanf("%c", &cmd);



        parity = getCHILD(root, RIGHT) % 2;
        root = insert(root, priceA, priceB, i);

        parity = (getCHILD(root, RIGHT)) % 2;
        // printTreeData(root);
        // printf("key:%d", TreeNode[i]->key);
        // printf("root_parity:%d\n", parity);

        // parity = (getCHILD(root, RIGHT)) % 2;
        // printf("root_key:%d\n", root->key);
    }
        ans = 0, ans2 = 0;
        parity = getCHILD(root, RIGHT) % 2;
        getANS(root);
        printf("%lld\n", ans);


    // ll ans = getANS(root);
    // parity = 0;
    // printf("%lld\n", ans);

    int type, c, d, e, f;
    for (int i = 0; i < M - 1; i++){
        scanf("%d%d%d%d%d", &type, &c, &d, &e, &f);
        root = delete(root, TreeNode[type]->key);
        // root = insert(root, (c * (ans % Q) + d) % Q, (e * (ans % Q) + f) % Q, type);

        // // ans = getANS(root);
        // parity = 0;
        // printf("%lld\n", ans);
    }
    return 0;
}
