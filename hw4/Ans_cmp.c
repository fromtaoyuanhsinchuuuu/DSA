#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>

#define MAXSTRING 200000

int main(void)
{
    char FileName1[MAXSTRING], FileName2[MAXSTRING];
    // printf("dick!\n");
    scanf("%s %s",FileName1,FileName2);

    FILE *fp;
    fp = fopen(FileName1,"r");
    assert(fp != NULL);

    FILE *fp2;
    fp2 = fopen(FileName2,"r");
    assert(fp2 != NULL);

    char str1[MAXSTRING],str2[MAXSTRING];
    
    int line = 0;

    bool ac = 1;
    while (fgets(str1,MAXSTRING,fp) != NULL && fgets(str2,MAXSTRING,fp2) != NULL) {
        line++;              
        if (strcmp(str1,str2)){
            ac = 0;
            printf("WA | ");
            printf("line:%d\n",line);
            break;
        }
    }

    if (ac) printf("AC!\n");
    return 0;
}
