#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

void process_file(char *filename) {
    // Open the file
    FILE *fp = fopen(filename, "r");
    // assert(file != NULL);

    if (fp == NULL) {

        // printf("Failed to open file: %s\n", filename);
        return;
    }

    int c;
    FILE *fp2 = fopen("test.in", "w");
    if (fp2 == NULL) {
        printf("Failed to open file: test.in\n");
        fclose(fp);
        return;
    }

    while ((c = fgetc(fp)) != EOF) {
        fputc(c, fp2);
    }

    system("main.exe < test.in > test.out");
    

    // Close the files
    fclose(fp);
    fclose(fp2);

    // Process the file
    printf("File name: %s\n", filename);
}

void test_directory(const char* directory_path) {
    DIR* directory = opendir(directory_path);

    if (directory == NULL) {
        printf("Failed to open the directory.\n");
        return;
    }

    struct dirent* entry;

    char entry_path[256];
    while ((entry = readdir(directory)) != NULL) {
        snprintf(entry_path, sizeof(entry_path), "%s/%s", directory_path, entry->d_name);
        FILE *fp = fopen(entry_path, "r");

        // printf("%s\n", entry_path);

        // Check if the entry is a regular file
        // struct stat entry_stat;
        // if (stat(entry_path, &entry_stat) == 0 && S_ISREG(entry_stat.st_mode)) {
        process_file(entry_path);
        // }
        // FILE* file = fopen(entry_path, "r");
        // assert(file != NULL);
    }

    closedir(directory);
}

int main() {
    const char* directory_path = "sin";
    
    system("gcc bullshit.c -o main.exe");

    test_directory(directory_path);

    return 0;
}
