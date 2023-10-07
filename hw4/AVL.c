#include <stdio.h>
#include <stdlib.h>

// Structure for a node of the AVL tree
typedef struct Node {
    int key;
    struct Node* parent;
    struct Node* left;
    struct Node* right;
    int height;
} Node;

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

// Function to insert a key into the AVL tree
Node* insert(Node* root, int key) {
    if (root == NULL) {
        Node* newNode = (Node*)malloc(sizeof(Node));
        newNode->key = key;
        newNode->parent = NULL;
        newNode->left = NULL;
        newNode->right = NULL;
        newNode->height = 1;
        return newNode;
    }
    
    if (key < root->key) {
        root->left = insert(root->left, key);
        root->left->parent = root;
    } else if (key > root->key) {
        root->right = insert(root->right, key);
        root->right->parent = root;
    } else {
        // Duplicate keys not allowed
        return root;
    }
    
    return balance(root);
}

// Function to find the minimum key in the AVL tree


// Function to find a key in the AVL tree
Node* search(Node* root, int key) {
    if (root == NULL || root->key == key)
        return root;
    
    if (key < root->key)
        return search(root->left, key);
    else
        return search(root->right, key);
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
            root->key = minRight->key;
            root->right = delete(root->right, minRight->key);
        }
    }
    
    if (root != NULL)
        root = balance(root);
    
    return root;
}

// Function to print the AVL tree in in-order traversal
void printInOrder(Node* root) {
    if (root != NULL) {
        printInOrder(root->right);
        printf("%d ", root->key);
        printInOrder(root->left);

    }
}

// Function to free the memory allocated for the AVL tree
void freeTree(Node* root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

int main() {
    Node* root = NULL;

    // Insertion
    root = insert(root, 50);
    root = insert(root, 30);
    root = insert(root, 70);
    root = insert(root, 20);
    root = insert(root, 40);
    root = insert(root, 60);
    root = insert(root, 80);

    printf("In-order traversal of the AVL tree: ");
    printInOrder(root);
    printf("\n");

    // Search
    int searchKey = 40;
    Node* searchResult = search(root, searchKey);
    if (searchResult != NULL)
        printf("Key %d found in the AVL tree.\n", searchKey);
    else
        printf("Key %d not found in the AVL tree.\n", searchKey);

    // Deletion
    int deleteKey = 30;
    root = delete(root, deleteKey);
    printf("In-order traversal after deleting key %d: ", deleteKey);
    printInOrder(root);
    printf("\n");

    // Free the memory allocated for the AVL tree
    freeTree(root);

    return 0;
}
