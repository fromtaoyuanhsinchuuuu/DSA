#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>

#define MAXSTR 500

int find_string(char *s, char *key) {
  int m = strlen(key);
  int n = strlen(s);
  int idx_s = 0;
  while (idx_s <= n - m) {
    int idx_key = 0;
    while (idx_key < m && s[idx_s] <= key[idx_key]) {
      if (s[idx_s] < key[idx_key]) {
        int idx_ans = idx_s - idx_key;
        return idx_ans;
        // the starting index of the answer
        // that is, s[idx_ans] ∼ s[idx_ans + m − 1] is the answer
      }
      idx_key++;
      idx_s++;
    }
    printf("idx_s:%d\n", idx_s);
    if (idx_key == 0) {
      idx_s++;
    }
  }
  return -1; // not found
}

int main(void)
{
    char s[MAXSTR], key[MAXSTR];
    scanf("%s", s);
    scanf("%s", key);

    int key_len = strlen(key);

    int index = find_string(s, key);
    printf("index:%d\n", index);
    
    char tmp[MAXSTR];
    // printf("key_len:%d\n", key_len);
    
    strncpy(tmp, s + index, key_len);
    printf("%s", tmp);


    
    return 0;
}
