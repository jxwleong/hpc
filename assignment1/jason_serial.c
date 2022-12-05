#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct {
     int x, y;
     unsigned char *data;
} PPMImage;

#define CREATOR "JASONLEONG"
#define RGB_COMPONENT_COLOR 255

static PPMImage *readPPM(const char *filename) {
         char buff[16];
         PPMImage *img;
         FILE *fp;
         int c, rgb_comp_color;
         //open PPM file for reading
         fp = fopen(filename, "rb");
         if (!fp) {
              fprintf(stderr, "Unable to open file '%s'\n", filename);
              return(NULL);
         }

         //read image format
         if (!fgets(buff, sizeof(buff), fp)) {
              perror(filename);
              return(NULL);
         }

    //check the image format
    if (buff[0] != 'P' || buff[1] != '6') {
         fprintf(stderr, "Invalid image format (must be 'P6')\n");
         return(NULL);
    }

    //alloc memory form image
    img = (PPMImage *)malloc(sizeof(PPMImage));
    if (!img) {
         fprintf(stderr, "Unable to allocate memory\n");
         return(NULL);
    }

    //check for comments
    c = getc(fp);
    while (c == '#') {
    while (getc(fp) != '\n') ;
         c = getc(fp);
    }

    ungetc(c, fp);
    //read image size information
    if (fscanf(fp, "%d %d", &img->x, &img->y) != 2) {
         fprintf(stderr, "Invalid image size (error loading '%s')\n", filename);
         return(NULL);
    }

    //read rgb component
    if (fscanf(fp, "%d", &rgb_comp_color) != 1) {
         fprintf(stderr, "Invalid rgb component (error loading '%s')\n", filename);
         return(NULL);
    }

    //check rgb component depth
    if (rgb_comp_color!= RGB_COMPONENT_COLOR) {
         fprintf(stderr, "'%s' does not have 8-bits components\n", filename);
         return(NULL);
    }

    while (fgetc(fp) != '\n') ;
    //memory allocation for pixel data
    img->data = (unsigned char*)malloc(img->x * img->y * sizeof(unsigned char) * 3);

    if (!img) {
         fprintf(stderr, "Unable to allocate memory\n");
         return(NULL);
    }

    //read pixel data from file
    if (fread(img->data, 3 * img->x, img->y, fp) != img->y) {
         fprintf(stderr, "Error loading image '%s'\n", filename);
         return(NULL);
    }

    fclose(fp);
    return img;
}
int writePPM(const char *filename, PPMImage *img) {
    FILE *fp;
    //open file for output
    fp = fopen(filename, "wb");
    if (!fp) {
         fprintf(stderr, "Unable to open file '%s'\n", filename);
         return(1);
    }

    //write the header file
    //image format
    fprintf(fp, "P6\n");

    //comments
    fprintf(fp, "# Created by %s\n",CREATOR);

    //image size
    fprintf(fp, "%d %d\n",img->x,img->y);

    // rgb component depth
    fprintf(fp, "%d\n",RGB_COMPONENT_COLOR);

    // pixel data
    fwrite(img->data, 3 * img->x, img->y, fp);
    fclose(fp);
	return(0);
}

#define SIZE 256

PPMImage invert(PPMImage *img){
     int size = img->x * img->y * 3;
     for (int x=0; x<=size; x++){
          img->data[x] = 255 - img->data[x];
     }
}

int main(){
	char filename[SIZE];
    PPMImage *image;
	
	printf("What is the input file name? ");
	fgets(filename,SIZE,stdin); 
	filename[strlen(filename)-1] = '\0'; // Get rid of the carriage return
    image = readPPM(filename);
	if (image) {
    	printf("What is the output file name? ");
		fgets(filename,SIZE,stdin); 
		filename[strlen(filename)-1] = '\0'; // Get rid of the carriage return
		invert(image);
		writePPM(filename,image);
	}
}

