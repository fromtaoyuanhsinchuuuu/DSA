#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>

#define MAXN 10000

typedef struct Node{
    int data;
    struct Node *next;

} Node;

Node *genNode(int data,Node *remain)
{
    Node *now = (Node *)malloc(sizeof(Node));
    now->next = remain;
    now->data = data;
    return now;
}

void printLinledList(Node *linklist)
{
    for (Node *ptr = linklist; ptr != NULL; ptr = ptr->next){
        printf("%d ",ptr->data);
    }
    printf("\n");
}

Node *genLinkedList(int arr[MAXN], int n)
{
    Node *head;
    Node *prev = NULL;
    for (int i = n - 1; i >= 0; i--){
        head = genNode(arr[i], prev);
        prev = head;
    }
    return head;
}

Node *sortL(Node *L)
{
    Node *head = L;

    Node *ptr = L;
    Node *prev = NULL;

    int neg_num = 0;

    Node *neg_head = NULL;
    while (ptr != NULL){
        if (ptr->data < 0){
            Node *next = ptr->next;
            if (neg_num == 0){
                neg_head = genNode(ptr->data, neg_head);
                // printf("%d",neg_head->data);
            }
            else{
                neg_head = genNode(ptr->data, neg_head);
            }
            neg_num++;
            if (ptr == head){
                head = next;
            }
            free(ptr);
            ptr = next;
            if (prev != NULL){
                prev->next = ptr;
            }
        }
        else{
            prev = ptr;
            ptr = ptr->next;
        }
    }

    if (prev != NULL){
        // printf("!");
        prev->next = neg_head;
    }
    else{
        head = neg_head;
    }

    return head;
}


int main(void)
{
    int n;
    scanf("%d", &n);

    int arr[MAXN];

    for (int i = 0; i < n; i++){
        scanf("%d", &arr[i]);
    }

    Node *l1 = genLinkedList(arr, n);
    // printLinledList(l1);
    l1 = sortL(l1);
    printLinledList(l1);


   
    
    return 0;
}
