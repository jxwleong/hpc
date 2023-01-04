// C program to perform histogram equalisation to adjust contrast levels

// All the needed library functions for this program
#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
     int x, y;
     unsigned char *data;
} PPMImage;

#define CREATOR "JASON_LEONG_XIE_WEI"
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

// Function to perform histogram equalisation on an image
void histogramEqualisation(PPMImage *image) {
	// Declaring 2 arrays for storing histogram values (frequencies) and
	// new gray level values (newly mapped pixel values as per algorithm)
	int hist[256] = { 0 };
	int new_gray_level[256] = { 0 };

	// Declaring other important variables
	int total, curr, i, grey, numData;
	numData = (image->x)*(image->y)*3;	// Number of data items in the RGB picture

	// Calculating frequency of occurrence for all pixel values
	for (i = 0; i<numData; i+=3) {	// increment by 3 because each RGB pixel is 3 values.
		grey = ((image->data[i])+(image->data[i+1])+(image->data[i+2]))/3.0 + 0.5; // The average brightness of the RGB pixel
		hist[grey]++;
	}

	// calculating total number of pixels
	total = (image->x)*(image->y);

	curr = 0;

	// calculating cumulative frequency and new gray levels
	for (i = 0; i < 256; i++)
	{	// cumulative frequency
		curr += hist[i];
		// calculating new gray level after multiplying by
		// maximum gray count which is 255 and dividing by
		// total number of pixels
		new_gray_level[i] = round((((float)curr) * 255) / total);
	}

	// performing histogram equalisation by mapping new gray levels
	for (i = 0; i<numData; i++) {
		image->data[i] = (unsigned char)new_gray_level[image->data[i]];
	}
}


#define SIZE 256

int main(){
	char filename[SIZE];
    PPMImage *image;
	
	printf("What is the image file name? ");
	fgets(filename,SIZE,stdin); 
	filename[strlen(filename)-1] = '\0'; // Get rid of the carriage return
    image = readPPM(filename);
	if (image) {
    	histogramEqualisation(image);

		printf("What is the output file name? ");
		fgets(filename,SIZE,stdin); 
		filename[strlen(filename)-1] = '\0'; // Get rid of the carriage return
		
		writePPM(filename,image);
	}
}