#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>

#define MAXN 100
#define MAXDIGIT 30

typedef long long ll;

typedef struct PriceNode {
    int index;
    ll price;
    int due_day;
    struct PriceNode *LeftNode, *RightNode, *ParentNode;
} PriceNode;

typedef struct CompanyNode{
    ll total_price;
    int sweet_melon_num;
    int Index;
    int ParentIndex;
    int priceNum;
    int child_num;
    PriceNode *PriceTail, *PriceTop;    
    struct CompanyNode *Parent;
} CompanyNode;

PriceNode* genPriceNode(int index, int price, int due_day)
{
    PriceNode* Node = malloc(sizeof(PriceNode));
    Node->due_day = due_day;
    Node->index = index;
    Node->LeftNode = NULL;
    Node->RightNode = NULL;
    Node->ParentNode = NULL;
    Node->price = price;
    return Node;
}

CompanyNode* genCompanyNode(int index, int ParentIndex)
{
    CompanyNode* head = (CompanyNode*)malloc(sizeof(CompanyNode));
    head->Index = index;
    head->ParentIndex = ParentIndex;
    head->sweet_melon_num = 1;
    head->total_price = 0; // 包含所有子公司的加總
    head->priceNum = 0;
    head->PriceTop = NULL;
    head->PriceTail = NULL;
    head->Parent = NULL;
    return head;
}

void setSweetMelon(CompanyNode* Company_arr[MAXN], int nowIndex)
{
    if (Company_arr[Company_arr[nowIndex]->ParentIndex] == NULL){
        return;
    }
    (Company_arr[Company_arr[nowIndex]->ParentIndex]->sweet_melon_num)++;
    setSweetMelon(Company_arr, Company_arr[nowIndex]->ParentIndex);
    return;
}

int setRelation(CompanyNode* Company_arr[MAXN], int N, int parent_arr[MAXN], CompanyNode* leaf_arr[MAXN])
{
    int leaf_num = 0;
    for (int i = 1; i <= N; i++){
        if (!parent_arr[i]){
            leaf_arr[leaf_num++] = Company_arr[i];
        }
        // setSweetMelon(Company_arr, i);
        Company_arr[i]->Parent = Company_arr[Company_arr[i]->ParentIndex];
    }
    return leaf_num;
}

int set_Binary_arr(int binary_arr[MAXDIGIT], int index)
{
    int digit = (int)log2(index) + 1;
    for (int i = digit - 1; i >= 0; i--){
        binary_arr[i] = (index & 1);
        index >>= 1;
    }
    return digit;
}

void swapPriceNode(PriceNode *a, PriceNode *b)
{
    int index = a->index, due_day = a->due_day, price = a->price;
    a->index = b->index, a->due_day = b->due_day, a->price = b->price;
    b->index = index, b->due_day = due_day, b->price = price;
    return;
}

PriceNode *findTail(PriceNode *priceTop, int price_num)
{
    int binary_arr[MAXDIGIT] = {};
    int digit = set_Binary_arr(binary_arr, price_num);

    assert(binary_arr[0] == 1);

    PriceNode *ptr = priceTop;
    for (int i = 1; i < digit; i++){
        if (binary_arr[i] == 1){ // 向右
            ptr = ptr->RightNode;
        }
        else{ // 向左
            ptr = ptr->LeftNode;
        }
    }
    assert(ptr != NULL);
    return ptr;
}


void update_company(CompanyNode *nowCompany, int price, int period, int day)
{
    int binary_arr[MAXDIGIT] = {};
    PriceNode *priceNode = genPriceNode(++(nowCompany->priceNum), price, day + period);
    if (nowCompany->PriceTop == NULL){
        assert(nowCompany->PriceTail == NULL && nowCompany->priceNum == 1);
        nowCompany->PriceTop = priceNode;
        nowCompany->PriceTail = priceNode;
        nowCompany->total_price = 0;
        return;
    }

    int digit = set_Binary_arr(binary_arr, nowCompany->priceNum);
    PriceNode *ptr = nowCompany->PriceTop;

    assert(binary_arr[0] == 1);

    for (int i = 1; i < digit - 1; i++){
        if (binary_arr[i] == 1){ // 向右
            ptr = ptr->RightNode;
        }
        else{ // 向左
            ptr = ptr->LeftNode;
        }
    }
    assert(ptr != NULL);
    if (binary_arr[digit - 1] == 1){
        ptr->RightNode = priceNode;
    }
    else{
        ptr->LeftNode = priceNode;
    }
    priceNode->ParentNode = ptr;
    nowCompany->PriceTail = priceNode;

    /* 使之浮上來 */
    ptr = priceNode;
    // printf("ptrPrice:%d\n", ptr->price);
    while (ptr->ParentNode != NULL && ptr->ParentNode->price > ptr->price){
        // printf("swap!\n");
        swapPriceNode(ptr, ptr->ParentNode); // 只有換內部資料
        ptr = ptr->ParentNode;
    }
    nowCompany->total_price = 0;
    // nowCompany->PriceTail = findTail(nowCompany->PriceTop, nowCompany->priceNum);
    // printf("company:%d totalPrice:%d\n",nowCompany->Index, nowCompany->total_price);
    // printf("index:%d pricetop:%d\n", nowCompany->Index, nowCompany->PriceTop->price);
    return;
}


PriceNode *min(PriceNode *ptr)
{
    PriceNode *ans = ptr;
    if (ptr->LeftNode != NULL && ptr->LeftNode->price < ans->price){
        ans = ptr->LeftNode;
    }
    if (ptr->RightNode != NULL && ptr->RightNode->price < ans->price){
        ans = ptr->RightNode;
    }
    assert(ans != NULL);
    return ans;
}

void heapify_priceTree(CompanyNode *now, int day)
{
    PriceNode *top = now->PriceTop, *tail = now->PriceTail;
    if (!(day > top->due_day)){ // 沒有過期
        return;
    }
    assert(top != tail);

    while (day > top->due_day){ // 已過期
        swapPriceNode(top, tail);
        if (tail->ParentNode->RightNode == tail){
            tail->ParentNode->RightNode = NULL;
        }
        else{
            assert(tail->ParentNode->LeftNode == tail);
            tail->ParentNode->LeftNode = NULL;
        }
        free(tail);
        tail = findTail(top, --(now->priceNum));

        /* 沉到下面 */
        PriceNode *ptr = top;
        while (1){
            PriceNode *minPrice = min(ptr);
            if (minPrice == ptr){
                break;
            } 
            swapPriceNode(minPrice, ptr);
            ptr = minPrice;
        }
    }
    return;
}

void init_sweetMelon(int child_num_arr[MAXN], CompanyNode *company_arr[MAXN], int N, CompanyNode *leaf_arr[MAXN], int leaf_num)
{
    /* init child_num */
    for (int i = 1; i <= N; i++){
        company_arr[i]->child_num = child_num_arr[i];
    }
    for (int i = 0; i < leaf_num; i++){
        CompanyNode *ptr = leaf_arr[i];
        while (ptr->child_num == 0 && ptr->Parent != NULL){
            ptr->Parent->sweet_melon_num += ptr->sweet_melon_num;
            ptr->Parent->child_num--;
            ptr = ptr->Parent;
        }
    }
    return;
}

void printPiceTree(PriceNode *ptr)
{
    if (ptr == NULL){
        return;
    }
    printf("%d ",ptr->price);
    printPiceTree(ptr->LeftNode);
    printPiceTree(ptr->RightNode);
    return;
}

int main(void)
{
    int N, M, C;
    scanf("%d%d%d", &N, &M, &C);

    int parent_arr[MAXN] = {-1};
    int parent_index_arr[MAXN] = {-1};
    int child_num_arr[MAXN] = {-1};
    CompanyNode* Company_arr[MAXN];
    CompanyNode* Leaf_arr[MAXN];

    Company_arr[0] = NULL;
    Company_arr[1] = genCompanyNode(1, 0);
    parent_index_arr[1] = 0;
    parent_arr[1] = 1;

    int parent_index = -1;
    for (int i = 2; i <= N; i++){
        scanf("%d", &parent_index);
        child_num_arr[parent_index]++;
        parent_index_arr[i] = parent_index;
        parent_arr[parent_index] = 1;
        Company_arr[i] = genCompanyNode(i, parent_index);
    }

    // for (int i = 1; i <= N; i++){
    //     printf("%d ",child_num_arr[i]);
    // }

    int leaf_num = setRelation(Company_arr, N, parent_arr, Leaf_arr);
    init_sweetMelon(child_num_arr, Company_arr, N, Leaf_arr, leaf_num);

    // for (int i = 1; i <= N; i++){
    //     printf("%d ",Company_arr[i]->sweet_melon_num);
    // }
    // printf("\n");

    for (int nowday = 1; nowday <= M; nowday++){
        int ans_num = -1,price = -1, period = -1;
        // printf("nowday:%d\n", nowday);
        for (int i = 1; i <= N; i++){
            scanf("%d%d", &price, &period);
            update_company(Company_arr[i], price, period, nowday);
            Company_arr[i]->child_num = child_num_arr[i];
        }
        // printf("leaf_num:%d\n",leaf_num);
        for (int i = 0; i < leaf_num; i++){
            CompanyNode *ptr = Leaf_arr[i];
            // printf("ajskdasd\n");

            /* 更新完往上送 */
            while (ptr->child_num == 0){

                /* 更新每間公司的totalprice跟price_head */
                heapify_priceTree(ptr, nowday);
                ptr->total_price += ptr->PriceTop->price;
                if (ptr->total_price <= C && ptr->sweet_melon_num > ans_num){
                    // printf("contract with:%d\n", ptr->Index);
                    // printf("totalprice:%d\n", ptr->total_price);
                    ans_num = ptr->sweet_melon_num;
                }
                // printf("ptr:%d\n",ptr->Index);
                if (ptr->Parent == NULL){
                    assert(ptr == Company_arr[1]);
                    // printf("ans_num:%d",ans_num);
                    break;
                }
                (ptr->Parent->child_num)--;
                (ptr->Parent->total_price) += (ptr->total_price);
                ptr = ptr->Parent;
            }
        }
        // printf("dick");
        printf("%d\n",ans_num);
    }

    // for (int i = 1; i <= N; i++){
    //     printf("i:%d|",i);
    //     printf("top:%d ",Company_arr[i]->PriceTop->price);
    //     printPiceTree(Company_arr[i]->PriceTop);
    //     printf("totalPrice:%d",Company_arr[i]->total_price);
    //     printf("\n");
    // }
    return 0;
}
