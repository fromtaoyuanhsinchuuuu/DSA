#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

void process_file(const char* input_file, const char* output_dir){
    // Open the input file
    FILE* input_fp = fopen(input_file, "r");
    if (input_fp == NULL) {
        printf("Failed to open input file: %s\n", input_file);
        return;
    }

    // Extract the input file name
    char* input_file_name = strrchr(input_file, '/');
    if (input_file_name == NULL) {
        input_file_name = (char*)input_file;
    } else {
        input_file_name++;  // Skip the '/'
    }

    // Construct the output file path
    char output_file[256];
    snprintf(output_file, sizeof(output_file), "%s/%s", output_dir, input_file_name);

    // printf("output:%s\n", output_file);

    // Open the output file
    FILE* output_fp = fopen(output_file, "w");
    if (output_fp == NULL) {
        printf("Failed to open output file: %s\n", output_file);
        fclose(input_fp);
        return;
    }

    // Execute the program and redirect input/output
    char command[256];
    snprintf(command, sizeof(command), "main.exe < %s > %s", input_file, output_file);

    // printf("")
    printf("command:%s\n", command);
    // Execute the program
    system(command);

    // Close the file pointers
    fclose(input_fp);
    fclose(output_fp);
}

void test_files(const char* input_dir, const char* output_dir) {
    DIR* input_directory = opendir(input_dir);
    if (input_directory == NULL) {
        printf("Failed to open the input directory.\n");
        return;
    }

    // Create the output directory if it doesn't exist
    struct stat st;
    if (stat(output_dir, &st) == -1) {
        if (mkdir(output_dir) == -1) {
            printf("Failed to create output directory: %s\n", output_dir);
            return;
        }
    }

    struct dirent *input_entry;
    while ((input_entry = readdir(input_directory)) != NULL) {
        // Skip directories and files starting with '.'
        if (input_entry->d_name[0] == '.') continue;

        // Construct the input file path
        char input_file[256];
        snprintf(input_file, sizeof(input_file), "%s/%s", input_dir, input_entry->d_name);
        // printf("infile:%s\n", input_file);

        char* input_file_name = strrchr(input_file, '/');
        if (input_file_name == NULL) {
            input_file_name = (char*)input_file;
        } else {
            input_file_name++;  // Skip the '/'
        }

        char output_file[256];
        snprintf(output_file, sizeof(output_file), "%s/%s", output_dir, input_file_name);
        FILE* output_fp = fopen(output_file, "w");



        char command[256];

        char AnsOutFile[256];
        snprintf(AnsOutFile, sizeof(AnsOutFile), "sout/%s", input_file_name);
        snprintf(command, sizeof(command), "main.exe < %s > %s", input_file, output_file);

        FILE *fp = fopen("filename", "w");
        fputs(AnsOutFile, fp);
        fputs(" ", fp);
        fputs(output_file, fp);
        fclose(fp);

        // printf("%s\n", command);
        fclose(output_fp);
        system(command);

        snprintf(command, sizeof(command), "Ans_cmp.exe < %s", "filename");
        printf("%s:", input_file_name);
        system(command);
        


        // sleep(3);


    // printf("output:%s\n", output_file);

    // Open the output file

        // Check if the entry is a regular file
        // struct stat entry_stat;
        // if (stat(input_file, &entry_stat) == 0 && S_ISREG(entry_stat.st_mode)) {
            // Process the files
            // process_file(input_file, output_dir);
        // }
    }

    closedir(input_directory);
}

int main() {
    const char *input_directory = "sin";
    const char *output_directory = "testout";

    system("gcc bullshit.c -o main.exe");
    system("gcc Ans_cmp.c -o Ans_cmp.exe");

    // if (chdir(input_directory) == -1) {
    //     printf("Failed to change directory to input directory: %s\n", input_directory);
    //     return 1;
    // }

    // system("gcc bullshit.c -o bullshit.exe");


    test_files(input_directory, output_directory);

    return 0;
}
