#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>

#define MAXN 3000
#define MAXM 3000

typedef struct {
    int appetite, color, index;
} Cat;

typedef struct Node {
  Cat *cat;
  struct Node *left;
  struct Node *right;
  struct Node *parent;
  int height;
} Node;

typedef struct {
    int color, head, num, size;
    Cat *cat_deque;
} Color_data;

Cat *genCat(int ap, int c, int index)
{
    Cat *cat = malloc(sizeof(Cat));
    cat->appetite = ap, cat->color = c, cat->index = index;
    return cat;
}

int cmp_color(const void *cat1, const void *cat2)
{
    Cat *cat_1 = (Cat *)cat1;
    Cat *cat_2 = (Cat *)cat2;
    return (cat_1->color - cat_2->color);
}

void push_back(Cat key_cat, Color_data *now_colorData)
{
    now_colorData->cat_deque[(now_colorData->head + now_colorData->size - 1) % now_colorData->size] = key_cat;
    return;
}

void push_front(Cat key_cat, Color_data *now_colorData)
{
    now_colorData->head = (now_colorData->head + now_colorData->size - 1) % now_colorData->size;
    now_colorData->cat_deque[(now_colorData->head % now_colorData->size)] = key_cat;
    return;
}

Cat pop_front(Color_data *now_colorData)
{
    Cat pop_cat = now_colorData->cat_deque[now_colorData->head % now_colorData->size];
    now_colorData->head = (now_colorData->head + now_colorData->size + 1) % now_colorData->size;
    return pop_cat;
}

Cat pop_back(Color_data *now_colorData)
{
    Cat pop_cat = now_colorData->cat_deque[(now_colorData->head + now_colorData->size - 1) % now_colorData->size];
    return pop_cat;
}

int cmp_appetite(const void *cat1, const void *cat2)
{
    Cat *cat_1 = (Cat *)cat1;
    Cat *cat_2 = (Cat *)cat2;
    return (cat_1->appetite - cat_2->appetite);
}

int cmp_Color_data(const void *Color_data1, const void *Color_data2)
{
    Color_data *Color_data_1 = (Color_data *)Color_data1;
    Color_data *Color_data_2 = (Color_data *)Color_data2;
    return (Color_data_1->color - Color_data_2->color);
}

int bsearch_appetite(int ap_key, Color_data color_data, int mode)
{
    int l = color_data.head, r = color_data.head + color_data.size - 1, m = -1, size = color_data.size;
    Cat *cat_arr = color_data.cat_deque;
    while (1){ // 有取值再模即可
        m = (mode == 0)? (int)((l + r) / 2): (int)ceil(((l + r) /(double) 2));
        if (m == l || m == r){
            if (cat_arr[l % size].appetite == ap_key)
                return l;
            else if (cat_arr[r % size].appetite == ap_key)
                return r;
            break;
        }
        if (cat_arr[m % size].appetite < ap_key) l = m;
        else if (cat_arr[m % size].appetite > ap_key) r = m;
        else return m;
    }

    assert(mode != 3);
    if (mode == 0){
        while (cat_arr[m % size].appetite < ap_key && m < color_data.head + color_data.size){
            m++;
        }
    }
    else{
        while (cat_arr[m % size].appetite > ap_key && m >= color_data.head){
            m--;
        }
    }
    return m;
}

Node *genNode(Cat *cat){
  Node *node = (Node *)malloc(sizeof(Node));
  node->cat = cat;
  node->left = NULL, node->right = NULL, node->parent = NULL;
  node->height = 1;
  return node;
}

int height(Node *N) 
{
    if (N == NULL) return 0;
    return N->height;
}

int max(int a, int b) 
{
    return (a > b)? a: b;
}

int getBalance(Node *Node) 
{
    if (Node == NULL) return 0;
    return height(Node->left) - height(Node->right);
}

Node *rightRotate(Node *y)
{
    Node *x = y->left;
    assert(x != NULL);
    Node *T2 = x->right; // 接到y的left,可能為NULL;

    x->right = y;
    y->left = T2;

    if (y->parent != NULL){
        if (y->parent->cat->appetite > x->cat->appetite){
            y->parent->left = x;
        }
        else{
            assert(y->parent->cat->appetite < x->cat->appetite);
            y->parent->right = x;
        }
    }

    if (T2 != NULL)
        T2->parent = y;

    x->parent = y->parent;
    y->parent = x;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

Node *leftRotate(Node *x)
{
    Node *y = x->right;
    assert(y != NULL);
    Node *T2 = y->left; // 等等接到x的right

    y->left = x;
    x->right = T2;
    if (T2 != NULL)
        T2->parent = x;

    if (x->parent != NULL){
        if (x->parent->cat->appetite > y->cat->appetite){
            x->parent->left = y;
        }
        else{
            assert(x->parent->cat->appetite < y->cat->appetite);
            x->parent->right = y;
        }
    }
    y->parent = x->parent;
    x->parent = y;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

Node *insertNode(Node *node, Cat *cat) 
{   

    if (node == NULL) return genNode(cat);

    if (cat->appetite < node->cat->appetite){
        node->left = insertNode(node->left, cat);
        node->left->parent = node;
    }
    else if (cat->appetite > node->cat->appetite){
        node->right = insertNode(node->right, cat);
        node->right->parent = node;
    }
    else // 有一樣的appetite
        assert(0);

    node->height = 1 + max(height(node->left),height(node->right));

    // Balance the tree
    int balance = getBalance(node); // 正數代表左邊高,負數代表右邊高
    if (balance > 1 && cat->appetite < node->left->cat->appetite) // LL型，node為插入的那個node.parent.parent
        return rightRotate(node); // 順時針

    if (balance > 1 && cat->appetite > node->left->cat->appetite){
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    
    if (balance < -1 && cat->appetite > node->right->cat->appetite)
        return leftRotate(node);

    if (balance < -1 && cat->appetite < node->right->cat->appetite){
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

void in_order_print(Node *root)
{
    if (root == NULL) return;

    in_order_print(root->left);
    printf("i:%d|%d ",root->cat->index,root->cat->appetite);
    in_order_print(root->right);
    return;
}

Node *findNodeByappetite(int appetite, Node *root)
{
    if (root != NULL && appetite == root->cat->appetite) return root;
    else if (root != NULL && appetite > root->cat->appetite) return findNodeByappetite(appetite, root->right);
    else if (root != NULL && appetite < root->cat->appetite) return findNodeByappetite(appetite, root->left);
    assert(0);
    return NULL; // 不應該找不到
}

Node *findNextBiggerAppetiteNode(Node *node)
{
    if (node == NULL) 
        assert(0);
    if (node->right != NULL){
        Node *ptr = node->right;
        while(ptr->left != NULL)
            ptr = ptr->left;
        return ptr;
    }

    Node *ptr = node;
    while (ptr->parent->cat->appetite < ptr->cat->appetite)
        ptr = ptr->parent;
    return ptr->parent;
}

void swapCatInTree(Cat *a, Cat *b)
{
    int tmp = a->color;
    a->color = b->color;
    b->color = tmp;

    tmp = a->index;
    a->index = b->index;
    b->index = tmp;
    return;
}

void swapCatInDeque(Cat *cat_1, Cat *cat_2, Color_data Color_data_arr[], int color_runNum, bool same_color)
{
    Color_data *nowColorData_1 = (Color_data *)bsearch(&(cat_1->color), Color_data_arr, color_runNum, sizeof(Color_data), cmp_Color_data);
    assert(nowColorData_1 != NULL);

    Cat *cat1_in_deque = &(nowColorData_1->cat_deque[bsearch_appetite(cat_1->appetite, *nowColorData_1, 3) % nowColorData_1->size]);
    assert(cat1_in_deque != NULL);

    Color_data *nowColorData_2 = (Color_data *)bsearch(&(cat_2->color), Color_data_arr, color_runNum, sizeof(Color_data), cmp_Color_data);
    assert(nowColorData_2 != NULL);

    Cat *cat2_in_deque = &(nowColorData_2->cat_deque[bsearch_appetite(cat_2->appetite, *nowColorData_2, 3) % nowColorData_2->size]);
    assert(cat2_in_deque != NULL);

    if (!same_color){
        int tmp = cat1_in_deque->appetite;
        cat1_in_deque->appetite = cat2_in_deque->appetite;
        cat2_in_deque->appetite = tmp;
    }
    else{
        int tmp = cat1_in_deque->index;
        cat1_in_deque->index = cat2_in_deque->index;
        cat2_in_deque->index = tmp;
    }
    return;
}

void printappetite(int N, Cat cat_arr[MAXN])
{
    for (int i = 0; i < N; i++){
        printf("i:%d|%d ",i, cat_arr[i].appetite);
    }
    return;
}

void printDeque(Color_data colordata)
{
    for (int i = 0; i < colordata.size; i++){
        printf("i:%d|%d ", colordata.cat_deque[(colordata.head + i) % colordata.size].index ,colordata.cat_deque[(colordata.head + i) % colordata.size].appetite);
    }
}

Node *getMinAppetite(Node *root)
{
    if (root->left == NULL) return root;
    return getMinAppetite(root->left);
}

Node *getMaxAppetite(Node *root)
{
    if (root->right == NULL) return root;
    return getMaxAppetite(root->right);
}

Node *deleteNode(Node *root, int appetite) 
{
  // Find the node and delete it
    if (root == NULL) return root;
    if (appetite < root->cat->appetite){
        root->left = deleteNode(root->left, appetite);
        if (root->left != NULL) 
            root->left->parent = root;
    }

    else if (appetite > root->cat->appetite){
        root->right = deleteNode(root->right, appetite);
        if (root->right != NULL)
            root->right->parent = root;
    }
    else{
        assert(root->cat->appetite == appetite);
        if ((root->left == NULL) || (root->right == NULL)){
            Node *temp = (root->left != NULL)? root->left : root->right;
            if (temp == NULL){ /* Both are NULL */
                temp = root;
                root = NULL;
            }
            else{
                Node *tmp_parent = root->parent;
                *root = *temp;
                root->parent = tmp_parent;
            }
            free(temp);
        } 
        else{
            Node *temp = getMinAppetite(root->right);
            root->cat = temp->cat;
            root->right = deleteNode(root->right, temp->cat->appetite);
            if (root->right != NULL)
                root->right->parent = root;
        }
    }

    
    if (root == NULL) return root;

    root->height = 1 + max(height(root->left), height(root->right));

    int balance = getBalance(root);
    if (balance > 1 && getBalance(root->left) >= 0){
        return rightRotate(root);
    }

    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

int main(void)
{
    int N, M;
    scanf("%d%d", &N, &M);
    Cat cat_arr[MAXN], cat_arr_sortedByIndex[MAXN];
    for (int i = 0; i < N; i++){
        cat_arr[i].index = i; // 在deque裡面index用不到
        scanf("%d",&(cat_arr[i].appetite));
        cat_arr_sortedByIndex[i].index = i;
        cat_arr_sortedByIndex[i].appetite = cat_arr[i].appetite;
    }
    for (int i = 0; i < N; i++){
        scanf("%d", &(cat_arr[i].color));
        cat_arr_sortedByIndex[i].color = cat_arr[i].color;
    }

    /* sort cat by color */
    qsort(cat_arr, N, sizeof(Cat), cmp_color);

    /* set deque */
    int color_num_arr[MAXN] = {};
    int prev_color = -1, now_color = -1, color_runNum = 0;
    for (int i = 0; i < N; i++){
        now_color = cat_arr[i].color;
        if (prev_color != -1 && now_color != prev_color){
            color_num_arr[++color_runNum]++;
        }
        else{
            color_num_arr[color_runNum]++;
        }
        prev_color = now_color;
    }
    color_runNum++;

    Color_data *Color_data_arr = (Color_data *)malloc(sizeof(Color_data) * color_runNum);

    int nowIndex = 0;
    for (int i = 0; i < color_runNum; i++){
        Color_data_arr[i].num = color_num_arr[i];
        Color_data_arr[i].size = color_num_arr[i];
        Cat *tmp_cat_deque = (Cat *)malloc(sizeof(Cat) * color_num_arr[i]);
        for (int j = 0; j < Color_data_arr[i].num; j++){
            tmp_cat_deque[j] = cat_arr[nowIndex++];
        }
        Color_data_arr[i].cat_deque = tmp_cat_deque;
        Color_data_arr[i].head = 0;
        Color_data_arr[i].color = cat_arr[nowIndex - 1].color;
    }

    for (int i = 0; i < color_runNum; i++){
        qsort(Color_data_arr[i].cat_deque, Color_data_arr[i].num, sizeof(Cat), cmp_appetite);
        printf("c:%d|",Color_data_arr[i].color);
        printDeque(Color_data_arr[i]);
        printf("\n");
    }

    /* set AVL */
    Node *root = NULL;
    for (int i = 0; i < N; i++){
        root = insertNode(root, &(cat_arr[i]));
    }

    int cmd;
    for (int it = 1; it <= M; it++){
        scanf("%d", &cmd);
        if (cmd == 1){ // questioning
            int c,l,r;
            scanf("%d%d%d", &c, &l, &r);

            Color_data *nowColorData = NULL;
            nowColorData = (Color_data *)bsearch(&c, Color_data_arr, color_runNum, sizeof(Color_data), cmp_Color_data);
            if (nowColorData == NULL){
                printf("0\n");
                continue;
            }
            assert(nowColorData->color == c);
            int left_index = bsearch_appetite(l, *nowColorData, 0), right_index = bsearch_appetite(r, *nowColorData, 1);
            printf("%d\n",right_index - left_index + 1);
        }
        if (cmd == 2){ //swapping
            int index;
            scanf("%d", &index);

            int nowappetie = cat_arr_sortedByIndex[index].appetite;
            assert(cat_arr_sortedByIndex[index].index == index);
            
            Node *greedyNode_cat_Node = findNodeByappetite(nowappetie, root);
            assert(greedyNode_cat_Node->cat->index == index);

            Node *biggerCat_Node = findNextBiggerAppetiteNode(greedyNode_cat_Node);
            assert(greedyNode_cat_Node->cat->appetite < biggerCat_Node->cat->appetite);

            /* swapCatInArr */
            cat_arr_sortedByIndex[index].appetite = biggerCat_Node->cat->appetite;
            cat_arr_sortedByIndex[biggerCat_Node->cat->index].appetite = nowappetie;

            swapCatInDeque(greedyNode_cat_Node->cat, biggerCat_Node->cat, Color_data_arr, color_runNum, greedyNode_cat_Node->cat->color == biggerCat_Node->cat->color);
            swapCatInTree(greedyNode_cat_Node->cat, biggerCat_Node->cat);
        }
        if (cmd == 3){
            int c, select, success;
            scanf("%d%d%d", &c, &select, &success);
            Color_data *nowColorData = NULL;
            nowColorData = (Color_data *)bsearch(&c, Color_data_arr, color_runNum, sizeof(Color_data), cmp_Color_data);
            if (success == 1){ 
                int Max_ap = getMaxAppetite(root)->cat->appetite;
                if (select == 1){
                    Cat nowCat = nowColorData->cat_deque[(nowColorData->head + nowColorData->size - 1) % nowColorData->size];

                    root = deleteNode(root, nowCat.appetite);
                    root = insertNode(root, genCat(Max_ap + 1, nowCat.color, nowCat.index));

                    nowColorData->cat_deque[(nowColorData->head + nowColorData->size - 1) % nowColorData->size].appetite = Max_ap + 1;

                    cat_arr_sortedByIndex[nowCat.index].appetite = Max_ap + 1;
                }
                else{
                    Cat nowCat = pop_front(nowColorData);

                    root = deleteNode(root, nowCat.appetite);
                    root = insertNode(root, genCat(Max_ap + 1, nowCat.color, nowCat.index));

                    nowCat.appetite = Max_ap + 1;
                    push_back(nowCat, nowColorData);

                    cat_arr_sortedByIndex[nowCat.index].appetite = Max_ap + 1;
                }
            }
            else{
                int Min_ap = getMinAppetite(root)->cat->appetite;
                if (select == 0){
                    Cat nowCat = nowColorData->cat_deque[(nowColorData->head) % nowColorData->size];

                    root = deleteNode(root, nowCat.appetite);
                    root = insertNode(root, genCat(Min_ap - 1, nowCat.color, nowCat.index));

                    nowColorData->cat_deque[(nowColorData->head) % nowColorData->size].appetite = Min_ap - 1;

                    cat_arr_sortedByIndex[nowCat.index].appetite = Min_ap - 1;
                }
                else{
                    Cat nowCat = pop_back(nowColorData);

                    root = deleteNode(root, nowCat.appetite);
                    root = insertNode(root, genCat(Min_ap - 1, nowCat.color, nowCat.index));

                    nowCat.appetite = Min_ap - 1;
                    push_front(nowCat, nowColorData);
                    
                    cat_arr_sortedByIndex[nowCat.index].appetite = Min_ap - 1;
                }
            }
        }
    }
    return 0;
}