#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>

#define MAXSTR 10000000

char str[MAXSTR];



int main(void)
{
    const char *dir_name = "soin";
    // test_directory(dir_name);
    DIR *directory = opendir(dir_name);
    assert(directory != NULL);

    struct dirent *entry = NULL;

    char entry_path[256];

    while ((entry = readdir(directory)) != NULL) {
        printf("filaname:%s\n", entry->d_name);
        snprintf(entry_path, sizeof(entry_path), "%s/%s", dir_name, entry->d_name);
        printf("%s\n", entry_path);
        FILE *fp = fopen(entry_path, "r");

        int c;
        while ((c = fgetc(fp)) != EOF){
            // printf("adsdas");
            printf("%d", c);
            FILE *fp2 = fopen("test.in", "w");
            fputc(c, fp2);
            fclose(fp2);
        }
        break;
        /* copy fp to fp2 */


        // assert(fp != NULL);
    }

    closedir(directory);

    

    
    return 0;
}
