#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <mpi.h>

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


int main(int argc, char *argv[]){
	char filename[SIZE];
    PPMImage *image;
	
	unsigned char *local_image;
    //unsigned char *global_data;
    int numprocs, rank, namelen, dest=0, tag=0, source;
    int size, start, end, x, y;
    int xsplit_per_procs;
    char processor_name[MPI_MAX_PROCESSOR_NAME];

    // Init MPI
    MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Get_processor_name(processor_name, &namelen);

    image = (PPMImage *)malloc(sizeof(PPMImage));
    

    // Rank0, handle the user input
    // hard code filename for "quicker"
    if (!rank){
        //printf("What is the input file name? ");
        //fgets(filename,SIZE,stdin); 
        //filename[strlen(filename)-1] = '\0'; // Get rid of the carriage return
        strncpy(filename, "input.ppm", 100);
        image = readPPM(filename);

        //printf("\n\n%d", );
    }
    
    // Broadcast the neccessary infomration 
    MPI_Bcast(&(image->x), 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&(image->y), 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&(image->data), 1, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
    MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    printf("\n X: %d, Y: %d", image->x, image->y);
    //fprintf(stderr,"%d A\n", rank);
    xsplit_per_procs = (image->x)/numprocs;

    // Divide them evenly for now
    start = (xsplit_per_procs*rank);
    end = (xsplit_per_procs*rank) + xsplit_per_procs -1;
    //fprintf(stderr,"%d A\n", rank);
    printf("Rank: %d, start: %d end: %d\n", rank, start, end);
    int i=0;

    // idea
    // problem: might be something wrong with gather, end result not showing invert
    // loop through y
    // then split x by the numprocs
    // after invert then gather them
    for(int y=0; y<image->y; y++){  
        //fprintf(stderr,"%d B\n", rank);
        local_image = (unsigned char *)malloc(sizeof(unsigned char *) * sizeof(image->x) * 3);
        //fprintf(stderr,"%d B\n", rank);
        int i=0;
        for(int x=start; x<end * 3; x++){
            local_image[i] = 255 - image->data[x];
            if (!rank)
                printf("RANK[%d]: local_image[%d]=%u, global_image->data[%d]=%u\n", rank, i, local_image[i], x, image->data[x]);
            i++;
        }
    }
    MPI_Gather(local_image, xsplit_per_procs, MPI_UNSIGNED_CHAR, image->data, xsplit_per_procs, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
	if (!rank) {
        // Get the modulus after the split above,
        // if modulus !=0 , invert the leftover part

    	//printf("What is the output file name? ");
		//fgets(filename,SIZE,stdin); 
		//filename[strlen(filename)-1] = '\0'; // Get rid of the carriage return
        printf("IM HERE!\n");
		strncpy(filename, "output.ppm", 100);
        
		writePPM(filename,image);
	}
    MPI_Finalize();  
}

