// File operations

#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <cassert>
#include <sys/stat.h>

// According to https://github.com/david-slatinek/c-read-vs.-mmap
// a buffer of 8192 bytes is good enough up to a 8MB file
// then mmap is up to 20% faster for 1GB file
#define BUFF_SIZE 64

size_t get_file_size(FILE* fptr) {
    clock_t start_t, end_t;
    long bufsize = -1;

    start_t = clock();
    long saved_offset = ftell(fptr);
    if (fseek(fptr, 0L, SEEK_END) == 0) {
         bufsize = ftell(fptr);
        if (bufsize == -1) {
            // Error
        }
    }
    fseek(fptr, saved_offset, SEEK_SET); // reset offset
    end_t = clock();
    printf("Measure file size using feek: %ld cycles\n", (long)(end_t-start_t));
    printf("Value: %ld bytes\n", bufsize);

    // Alternative
    struct stat stat;

    start_t = clock();
    fstat(fileno(fptr), &stat);
    off_t size=stat.st_size; // in bytes
    end_t = clock();
    printf("Measure file size using stat: %ld cycles\n", (long)(end_t-start_t));
    printf("Value: %ld bytes\n", size);

    assert(bufsize == size);

    return size;
}

int main(int argc, char** argv) {

    FILE *fptr;
    char buffer[BUFF_SIZE];
    clock_t start_t, end_t;
    fptr = fopen("tests/test.txt", "r");
    if (fptr == NULL) {
        printf("Error: Cannot read file\n");
        return 1;
    }

    get_file_size(fptr);

    start_t = clock();
    if (fgets(buffer, BUFF_SIZE, fptr) == NULL) {
        printf("Error: Cannot read from file\n");
    }
    end_t = clock();
    printf("Read a line from file: %ld cycles\n", (long)(end_t-start_t));

    start_t = clock();
    size_t len = strlen(buffer);
    end_t = clock();
    printf("Count number of chars in the buffer: %ld cycles\n", (long)(end_t-start_t));
    printf("Size of the line: %ld bytes\n", len);
    
    // Alternative:
    // size_t newlen = fread(buffer, sizeof(char), BUFF_SIZE-1, fp);
    // if (ferror(fp) != 0) {
    //     fputs("Error reading the file", stderr);
    // } else {
    //     buffer[newlen++] = '\0';
    // }


    // Go to the beginning of the file
    // if (fseek(fp, 0L, SEEK_SET) != 0) { /* Error */ }

    // mmap(); the entire file in order to parse it and get context aware syntax + search...

    // Write large files with mmap(): ftruncate() to expand the file, memmove() the content

    fclose(fptr);
    return 0;
}
