#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>

#define MAXN 100

typedef long long ll;

typedef struct PriceData{
    int price,due_day; 
} PriceData;

typedef struct Company{
    int child_num;
    struct Company **child_arr, *Parent;
    int sweet_melon_num;
    PriceData *PriceHeap;
    ll total_price;
    int price_num;
    int index,ParentIndex;
} Company;

Company *genCompany(int index, int parentIndex, int M)
{
    Company *head = malloc(sizeof(Company));
    head->index = index;
    head->ParentIndex = parentIndex;
    head->child_num = 0;
    head->price_num = 0;
    head->total_price = 0;
    head->sweet_melon_num = 1;
    head->Parent = NULL;
    head->PriceHeap = (PriceData *)malloc((M + 1) * sizeof(PriceData)); // 記得從1開始
    head->PriceHeap[0].due_day = -1,head->PriceHeap[0].price = -1;
    return head;
}

void setRelation(int parent_index_arr[MAXN], int child_num_arr[MAXN], int N, Company *company_arr[MAXN])
{
    for (int i = 1; i <= N; i++){
        company_arr[i]->child_arr = (Company **)malloc(sizeof(Company *) * child_num_arr[i]);
        company_arr[i]->ParentIndex = parent_index_arr[i];
    }
    for (int i = 1; i <= N; i++){
        if (i == 1){
            continue;
        }
        company_arr[i]->Parent = company_arr[company_arr[i]->ParentIndex];
        company_arr[i]->Parent->child_arr[(company_arr[i]->Parent->child_num)++] = company_arr[i];
    }
    return;
}

int init_SweetMelonNum(Company *ptr)
{
    if (ptr == NULL){
        return 0;
    }
    for (int i = 0; i < ptr->child_num; i++){
        ptr->sweet_melon_num += init_SweetMelonNum(ptr->child_arr[i]);
    }
    return ptr->sweet_melon_num;
}

void insert_priceData(PriceData tmp_priceData, Company *nowCompany)
{
    nowCompany->PriceHeap[++(nowCompany->price_num)] = tmp_priceData;
    /* 浮上來 */
    int nowIndex = nowCompany->price_num;
    assert(nowCompany->PriceHeap[nowIndex].price == tmp_priceData.price);
    while (nowIndex != 1 && nowCompany->PriceHeap[nowIndex / 2].price > nowCompany->PriceHeap[nowIndex].price){
        PriceData tmp = nowCompany->PriceHeap[nowIndex / 2];
        nowCompany->PriceHeap[nowIndex / 2] = nowCompany->PriceHeap[nowIndex];
        nowCompany->PriceHeap[nowIndex] = tmp;
        nowIndex /= 2;
    }
    return;
}

int min(PriceData *priceHeap, int nowIndex, int priceNum)
{
    int left = nowIndex * 2, right = nowIndex * 2 + 1, ansIndex = nowIndex;
    if (left <= priceNum){
        if (priceHeap[left].price < priceHeap[ansIndex].price){
            ansIndex = left;
        }
    }
    if (right <= priceNum){
        if (priceHeap[right].price < priceHeap[ansIndex].price){
            ansIndex = right;
        }
    }

    return ansIndex;

}

int heapify(PriceData *priceHeap, int price_num, int nowday) // return priceNum
{
    while (priceHeap[1].due_day < nowday){ // 過期
        priceHeap[1] = priceHeap[(price_num)--];
        int nowIndex = 1;
        while (nowIndex <= price_num){
            int min_index = min(priceHeap, nowIndex, price_num);
            if (min_index == nowIndex){
                break;
            }
            PriceData tmp = priceHeap[min_index];
            priceHeap[min_index] = priceHeap[nowIndex];
            priceHeap[nowIndex] = tmp;
            nowIndex = min_index;
        }
    }
    return price_num;
}

ll run_Company(int *ansNum, int C, Company *ptr, int nowday)
{
    if (ptr == NULL){
        return 0;
    }
    ptr->price_num = heapify(ptr->PriceHeap, ptr->price_num, nowday);
    ptr->total_price = ptr->PriceHeap[1].price;
    for (int i = 0; i < ptr->child_num; i++){
        ll d_price = run_Company(ansNum, C, ptr->child_arr[i], nowday);
        ptr->total_price += d_price;
    }
    if (ptr->total_price <= C){
        if (ptr->sweet_melon_num > *ansNum){
            *ansNum = ptr->sweet_melon_num;
        }
    }
    return ptr->total_price;
}





int main(void)
{
    int N, M, C;
    scanf("%d%d%d", &N, &M, &C);

    Company *CompanyArr[MAXN];
    CompanyArr[0] = NULL;
    CompanyArr[1] = genCompany(1, 0, M);

    int parent_index_arr[MAXN] = {-1};
    int child_num_arr[MAXN] = {-1};
    parent_index_arr[1] = 0;

    int parent_index;
    for (int i = 2; i <= N; i++){
        scanf("%d", &parent_index);
        child_num_arr[parent_index]++;
        parent_index_arr[i] = parent_index;
        CompanyArr[i] = genCompany(i, parent_index, M);
    }

    setRelation(parent_index_arr, child_num_arr, N, CompanyArr);
    init_SweetMelonNum(CompanyArr[1]);

    for (int nowday = 1; nowday <= M; nowday++){
        int ans_num = 0,price = -1, period = -1;
        PriceData tmp_priceData;
        for (int i = 1; i <= N; i++){
            scanf("%d%d", &price, &period);
            tmp_priceData.due_day = nowday + period, tmp_priceData.price = price;
            insert_priceData(tmp_priceData, CompanyArr[i]);
        }
        run_Company(&ans_num, C, CompanyArr[1], nowday);
        assert(ans_num > 0);
        printf("%d\n", ans_num);

    }

    return 0;
}
