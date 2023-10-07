#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main(void) {
    if (system("gcc add.c -o add.exe") != 0) {
        printf("Failed to compile add.c\n");
        return 1;
    }

    printf("Executing add.exe...\n");

    FILE* add_output = popen("add.exe 1 1", "r");
    if (add_output == NULL) {
        printf("Failed to execute add.exe\n");
        return 1;
    }

    char result[100];
    if (fgets(result, sizeof(result), add_output) != NULL) {
        printf("Output: %s", result);
    } else {
        printf("No output received\n");
    }

    pclose(add_output);

    return 0;
}
