#include <stdio.h>
#include <stdlib.h>

// Structure for Leftist Tree node
typedef struct node {
    int data;
    struct node* left;
    struct node* right;
    int rank;
} Node;

// Function to create a new node
Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->rank = 0;
    return newNode;
}

// Function to merge two Leftist Trees
Node* merge(Node* root1, Node* root2) {
    if (root1 == NULL)
        return root2;
    if (root2 == NULL)
        return root1;
    if (root1->data > root2->data) {
        Node* temp = root1;
        root1 = root2;
        root2 = temp;
    }

    /* root1會比較小 */
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

// Function to insert a new element into the Leftist Tree
Node* insert(Node* root, int data) {
    Node* newNode = createNode(data);
    return merge(root, newNode);
}

// Function to delete the minimum element from the Leftist Tree
Node* deleteMin(Node* root) {
    if (root == NULL)
        return NULL;
    Node* left = root->left;
    Node* right = root->right;
    free(root);
    return merge(left, right);
}

// Function to display the elements of the Leftist Tree in pre-order traversal
void preOrder(Node* root) {
    if (root != NULL) {
        printf("%d ", root->data);
        preOrder(root->left);
        preOrder(root->right);
    }
}

void inOrder(Node* root) {
    if (root != NULL) {
        inOrder(root->left);
        printf("%d ", root->data);
        inOrder(root->right);
    }
}



// Function to test the implementation
int main() {
    Node* root = NULL;

    // Insert elements into the Leftist Tree
    root = insert(root, 5);
    root = insert(root, 10);
    root = insert(root, 3);
    root = insert(root, 8);
    root = insert(root, 2);
    root = insert(root, 3);
    root = insert(root, 0);

    printf("Leftist Tree elements: ");
    preOrder(root);
    printf("\n");

    // Delete minimum element from the Leftist Tree
    root = deleteMin(root);

    printf("Leftist Tree elements after deletion: ");
    preOrder(root);
    printf("\n");
    inOrder(root);
    printf("\n");

    return 0;
}
