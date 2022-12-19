#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

#define ARRAYSIZE ('Z'-'A'+1)

int main(int argc, char *argv[])
{ 
    FILE *infile;
    int i, count[ARRAYSIZE]={0};
    long int size;
    char *buffer;
    clock_t start, end;
    infile = fopen(argv[1],"r");
    if (infile)
    { 
        fseek(infile,0L, SEEK_END); // Find out the
        size = ftell(infile); // size of the file.
        fseek(infile,0L, SEEK_SET); // Read the entire
        buffer = malloc(size); // file into the
        fread(buffer,1,size,infile); // buffer.
        start = clock();
        for (i=0; i<size; i++)
            if (isalpha(buffer[i]))
                count[tolower(buffer[i])-'a']++;

        end = clock();
        printf("CPU time used: %f seconds\n",
        ((double) (end - start)) / CLOCKS_PER_SEC);
        for (i=0; i<ARRAYSIZE; i++)
            printf("%c %d\n",i+'A', count[i]);

    }
    else
        printf("Could not open %s for reading.\n",argv[1]);
    }