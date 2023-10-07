#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>

#define MAXDIGIT 260

typedef struct  
{
    int digit;
    int num[MAXDIGIT];
} Number;

void print_number(Number number)
{
    if (number.digit == 0){
        printf("0");
    }
    for (int i = 0; i < number.digit; i++){
        printf("%d",number.num[i]);
    }
    printf("\n");
    return;
}

Number modify(Number number) // 處理開頭為0的情況
{
    while(number.num[0] == 0 && number.digit > 0){
        for (int i = 0; i < number.digit; i++){
            number.num[i] = number.num[i + 1];
        }
        number.digit--;
    }
    
    return number;
}

Number divided_by_two(Number number)
{
    assert(number.num[number.digit - 1] % 2 == 0);

    for (int i = 0; i < number.digit; i++){
        int now_digit = number.num[i];
        number.num[i] = now_digit / 2;
        number.num[i + 1] += (now_digit % 2) * 10;
    }

    /* modify */
    return modify(number);
}



int min(Number num1, Number num2)
{
    if (num1.digit - num2.digit > 0){
        return 1;
    }
    else if (num1.digit - num2.digit < 0){
        return 0;
    }

    assert(num1.num[0] != 0 && num2.num[0] != 0);

    for (int i = 0; i < num1.digit; i++){
        if (num1.num[i] < num2.num[i]){
            return 0;
        }
        else if (num1.num[i] > num2.num[i]){
            return 1;
        }
    }
    /* the same !*/
    return 1;
}

Number reverse(Number number)
{
    for (int i = 0; i <= (number.digit / 2) - 1; i++){
        int temp = number.num[i];
        number.num[i] = number.num[number.digit - i - 1];
        number.num[number.digit - i - 1] = temp;
    }
    return number;
}

Number mul_by_two(Number number)
{

    // print_number(number);

    for (int i = 0; i <= number.digit - 1; i++){
        number.num[i] *= 2;
    }
    // print_number(number);


    for (int i = 0; i <= number.digit - 1; i++){
        int now_num = number.num[i];
        number.num[i] = (now_num % 10);
        number.num[i + 1] += (now_num / 10);
    }
    // print_number(number);


    if (number.num[number.digit] != 0){
        number.digit++;
    }
    // print_number(number);

    while (number.num[number.digit] >= 10){
        // printf("dick%d\n",number.num[number.digit]);
        number.num[number.digit + 1] += number.num[number.digit] / 10;
        number.num[number.digit] %= 10;
        (number.digit)++;
    }

    return reverse(number);
}

Number subtract(Number num1, Number num2)
{
    /* return num1 - num2 */
    assert(num1.digit >= num2.digit);

    // print_number((num1));
    // print_number((num2));

    // printf("num1_digit:%d\n",num1.digit);
    // printf("num2_digit:%d\n",num2.digit);

    Number ret_num;

    ret_num.digit = num1.digit;
    for (int i = 0; i < num1.digit; i++){
        if (i < num2.digit){
            ret_num.num[i] = num1.num[i] - num2.num[i];
        }
        else{
            ret_num.num[i] = num1.num[i];
        }
    }

    // printf("last_digit:%d\n",ret_num.num[ret_num.digit - 1]);
    assert(ret_num.num[ret_num.digit - 1] >= 0);

    for (int i = ret_num.digit - 2; i >= 0; i--){
        if (ret_num.num[i] < 0){
            int index = i + 1; // 找到大於0的那一位跟他借
            while (!(ret_num.num[index] > 0)){
                index++;
            }
            ret_num.num[index]--;

            for (int j = index - 1; j > i; j--){
                ret_num.num[j] = 9;
            }

            ret_num.num[i] += 10;
            assert(ret_num.num[i] >= 1);
        }
    }

    return modify(reverse(ret_num));
}

Number multiply(Number num1, Number num2)
{

    Number ret = {0,{0}};
    for (int i = 0; i < num1.digit; i++){
        for (int j = 0; j < num2.digit; j++){
            ret.num[i + j] += (num1.num[i] * num2.num[j]);

        }
    }

    for (int i = 0; i < num1.digit + num2.digit - 1; i++){
        ret.num[i + 1] += ret.num[i] / 10;
        ret.num[i] %= 10;
    }

    int check_index = num1.digit + num2.digit - 1;
    while (ret.num[check_index] >= 10){
        ret.num[check_index + 1] += ret.num[check_index] / 10;
        ret.num[check_index] %= 10;
        check_index++;
    }

    ret.digit = check_index + 1;

    return reverse(ret);


}

Number gcd(Number num1, Number num2)
{
    Number Ans = {0,{0}};
    Ans.digit = 1;
    Ans.num[0] = 1;

    while (num1.digit != 0 && num2.digit != 0){
        // printf("num1:");
        // print_number(num1);
        // printf("num2:");
        // print_number(num2);
        // printf("Ans:");
        // print_number(Ans);
        
        if (num1.num[num1.digit - 1] % 2 == 0 && num2.num[num2.digit - 1] % 2 == 0){
            Ans = mul_by_two(reverse(Ans));
            // printf("Ans:");
            // print_number(Ans);
            num1 = divided_by_two(num1);
            num2 = divided_by_two(num2);
        }
        else if (num1.num[num1.digit - 1] % 2 == 0){// num1 is even
            num1 = divided_by_two(num1);
        }
        else if (num2.num[num2.digit - 1] % 2 == 0){ // num2 is even
            num2 = divided_by_two(num2);
        }
        

        if (min(num1,num2) == 1){ // num1 > num2
            // printf("swap!\n");
            Number temp = num1;
            num1 = num2;
            num2 = temp;
        }

        // printf("num1:");
        // print_number(num1);
        // printf("num2:");
        // print_number(num2);
        // printf("Ans:");
        // print_number(Ans);

        num2 = subtract(reverse(num2), reverse(num1));
    }

    return modify(multiply(reverse(num1), reverse(Ans)));
}


int main(void)
{
    Number num1 = {0,{0}};
    Number num2 = {0,{0}};

    num1.digit = 0, num2.digit = 0;
    char digit;

    /* read num1 */
    while (1){
        scanf("%c",&digit);
        if (!isdigit(digit)){
            break;
        }
        num1.num[(num1.digit)++] = digit - '0';
    }

    while (1){
        scanf("%c",&digit);
        if (!isdigit(digit)){
            break;
        }
        num2.num[(num2.digit)++] = digit - '0';
    }

    print_number(gcd(num1,num2));

    return 0;
}
