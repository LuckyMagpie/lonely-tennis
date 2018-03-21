#include <stdio.h>
#include <stdlib.h>

#include "vector.h"
#include "files.h"

char* file_to_str(const char* path)
{
   FILE* file = fopen(path, "rb");

    if (!file) {
       fprintf(stderr, "%s:File not found %s\n", __func__, path);
       char* error = malloc(1);
       error[0] = '\0';
       return error;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    char* string = malloc(file_size + 1);
    fread(string, file_size, 1, file);

    fclose(file);

    string[file_size] = '\0';

    return string;
}
