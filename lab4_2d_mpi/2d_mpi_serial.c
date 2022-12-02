#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAXPIXEL 256
#define TEMPFILE "2d_serial.pgm"

int xsize, ysize;

int f(int x, int y)
{ 
    double divisor = (M_PI*xsize/60);
    double zx = (double)x/divisor;
    double zy = (double)y/divisor;
    return (sin(zx)+cos(zy))*(MAXPIXEL/4)+(MAXPIXEL/2);
}
int main(int argc, char*argv[])
{ 
    int x, y;
    int **picture;
    FILE *outfile;
    printf("How wide and tall should the picture be? ");fflush(stdout);
    scanf("%d %d", &xsize, &ysize);
    picture =(int**)malloc(sizeof(int*)*ysize);
    for (y=0; y<ysize; y++)
    { 
        picture[y] = (int*)malloc(sizeof(int)*xsize);
        for (x=0; x<xsize; x++)
            picture[y][x] = f(x,y);
    }
    outfile=fopen(TEMPFILE,"w");
    fprintf(outfile,"P2\n%d %d %d\n",xsize, ysize, MAXPIXEL);
    for(y=0; y<ysize; y++)
    for (x=0; x<xsize; x++)
    fprintf(outfile,"%d\n", picture[y][x]);
    fclose(outfile);
    system("display "TEMPFILE);
}