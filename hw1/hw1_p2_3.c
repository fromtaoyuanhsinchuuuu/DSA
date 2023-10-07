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

Node *min(Node *l1, Node *l2)
{
    return (l1->data <= l2->data)? l1: l2;
}

Node *mergeLinkedList(Node *list1, Node *list2)
{
    Node *now1 = list1;
    Node *now2 = list2;

    Node *prev = min(now1, now2);
    Node *head = prev;

    if (prev == now1){
        now1 = now1->next;
    }
    else{
        now2 = now2->next;
    }

    while (now1 != NULL && now2 != NULL){
        Node *now = min(now1, now2);
        prev->next = now;
        prev = now;

        if (now == now1){
            now1 = now1->next;
        }
        else{
            now2 = now2->next;
        }
    }

    if (now1 == NULL){
        prev->next = now2;
    }
    else{
        prev->next = now1;
    }
    
    return head;
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

Node *reverseLinkedList(Node *list)
{
    Node *ptr = list;
    Node *prev = NULL;

    Node *next;
    while (ptr != NULL){
        printf("ptr:%d\n",ptr->data);
        next = ptr->next;
        ptr->next = prev;
        prev = ptr;
        ptr = next;
    }

    return prev;
}

int main(void)
{
    /* 想法:由小到大串完再reverse */
    int n;
    scanf("%d", &n);

    int arr[MAXN];

    for (int i = 0; i < n; i++){
        scanf("%d", &arr[i]);
    }

    Node *l1 = genLinkedList(arr, n);

    scanf("%d", &n);

    for (int i = 0; i < n; i++){
        scanf("%d", &arr[i]);
    }

    Node *l2 = genLinkedList(arr, n);

    Node *mergell = mergeLinkedList(l1, l2);

    printLinledList(mergell);
    Node *new = reverseLinkedList(mergell);
    printLinledList(new);









    

   
    
    return 0;
}
