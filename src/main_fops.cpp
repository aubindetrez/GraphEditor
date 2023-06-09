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
#define MAX_WIN_COL 750
#define MAX_WIN_ROW 500 // Maximum number of lines on screen

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

class LineTableEntry
{
    public:
        // How this entry maps to the file
        long fileoffset_start;
        long fileoffset_end;

        // Line number, need for display and for folds/multi-line conceals to make sense
        size_t linenr;

        // Offset of within line, needed for horizontal scrolling through very long lines
        // 0 if data[0] is the beginning of the line
        size_t charoffset;

        // Lenght of the string, avoid recomputing it
        size_t len; 

        bool ro; // Read only, example: if fully/partially concealed 

        bool dirty; // Need write-back

        char *data;

    private:
};

// Loads portions of a file into LineBuffer(s) and store information into a
// LineTable in order to write back changes to file.
// Chooses the most performant way to read data from a file depending on the
// file of the size...
// Also checks the file is not altered by an other program.
class FileLoader
{
    public:
        FileLoader(const char* filename) {
            // mmap(); the entire file in order to parse it and get context aware syntax + search...
            fptr = fopen(filename, "r");
            if (fptr == NULL) {
                printf("Error: Cannot read file\n");
            }
            filesize = get_file_size(fptr);
        }
        /*! \fn BufferSection(size_t topline, size_t leftcol, size_t wincol, size_t winrow, bool wrap)
            \brief Add a section to the buffer
            \param topline Line number to be printed at the top of the window
            \param leftcol Column numbber to be printed on the left of the screen
            \param wincol Maximum number of character that fits on the screen (horizontally)
            \param winrow Maximum number of character that fits on the screen (vertically)
            \param wrap Wrap / No-Wrap
         */
        int BufferSection(size_t topline, size_t leftcol, size_t wincol, size_t winrow, bool wrap) {
            clock_t start_t, end_t;
            start_t = clock();
            // if (fgets(buffer, BUFF_SIZE, fptr) == NULL) {
            //     printf("Error: Cannot read from file\n");
            // }
            end_t = clock();
            // printf("Read a line from file: %ld cycles\n", (long)(end_t-start_t));

            // Alternative:
            // size_t newlen = fread(buffer, sizeof(char), BUFF_SIZE-1, fp);
            // if (ferror(fp) != 0) {
            //     fputs("Error reading the file", stderr);
            // } else {
            //     buffer[newlen++] = '\0';
            // }
        }
        //! Returns true if not all changes have been written back to memory
        bool changed() {
            for (size_t i=0; i < sizeof(this->lineTable); i++) {
                if (this->lineTable[i].dirty) return true;
            }
            return false;
        }
        int writeBack() {
            // Write large files with mmap(): ftruncate() to expand the file, memmove() the content
        }
        long getFileOffsetOfLine(size_t linenr) {
            // TODO

            // Go to the beginning of the file
            // if (fseek(fp, 0L, SEEK_SET) != 0) { /* Error */ }
        }
        ~FileLoader() {
            if (fptr != NULL) {
                fclose(fptr);
                fptr = NULL;
            }
        }

    private:
        FILE* fptr = NULL;
        char buffer[BUFF_SIZE];
        size_t filesize = 0;
        LineTableEntry lineTable[2*MAX_WIN_ROW] = {0};
};

int main(int argc, char** argv) {

    return 0;
}
