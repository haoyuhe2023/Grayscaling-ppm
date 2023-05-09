#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>


typedef struct
{
    int maxVal;          // Image maximum threshold
    int height;           //  to record the height of the image
    int width;          //  to record the width of the image
    int **data          // Store original  values array
}PPMImage;


void getPPMFile(char fileName[],PPMImage* img);    // Read the ppm file
void gray(PPMImage* image);
//void gray(PPMImage* image,int thread_count);
void save(PPMImage* image);
PPMImage* im;

int main(int argc,char* argv[])
{
    int thread_count;
    thread_count = strtol(argv[1], NULL, 10);
    omp_set_num_threads(thread_count);
    double start;

    im = (PPMImage *)malloc(sizeof(PPMImage));// allocate memory for ppm image*/
    getPPMFile("im.ppm",&(*im));// get ppm file
    save(im);    // Save the image */

    start = omp_get_wtime();
    gray(im);
    printf("\n The elapsed time is %f sec\n", omp_get_wtime()-start);
    save(im);    // Save the image */
}


void getPPMFile(char fileName[],PPMImage* img){
    FILE* in_file;
    char version[100];
    int row,col;
    // Open a binary file, only allow reading and writing data
    in_file = fopen(fileName,"rb");

    /*File none error*/
    if (in_file == NULL)
    {
        fprintf(stderr, "Error: Unable to open file %s\n\n", fileName);
        exit(1);
    }
    printf("\nReading image file: %s\n", fileName);

    /*Read image information*/
    fscanf(in_file,"%s",version);
    fscanf(in_file,"%d",&(img->width));
    fscanf(in_file,"%d",&(img->height));
    fscanf(in_file,"%d",&(img->maxVal));

    /*Show image information*/
    printf("\n%s", version);
    printf("\n width  = %d",img->width);
    printf("\n height = %d",img->height);
    printf("\n maxVal = %d",img->maxVal);
    printf("\n");

    /*make dynamic array memory*/
    img->data = (int**) malloc(sizeof(int*)* img->height);
    for (int k = 0; k < img->height; k++) {
        img->data[k] = (int*) malloc(sizeof (int)* 3*(img->width));
    }

    /*Read image data*/
    for (row = 0; row < img-> height ; row++) {
        for (col = 0; col < 3*(img->width); col++) {
            fscanf(in_file,"%d",&(img->data[row][col]));
        }
    }
}

void gray(PPMImage* image){
    // Define temporary variables to store processed data
    int h = image->height;
    int w = image->width;
    int thread_count = omp_get_num_threads();
#pragma omp parallel for collapse(2)
//#  pragma omp parallel for num_threads(thread_count) \
      reduction(+: sum) private(factor)
    for (int row = 0; row < h ; row++) {
        for (int col = 0; col < 3*w ; col++) {
            if((col+1) %3==0){
                int record =(im->data[row][col]+im->data[row][col-1]+im->data[row][col-2])/3;
                im->data[row][col]=record;
                im->data[row][col-1]=record;
                im->data[row][col-2]=record;
            }else{
                continue;
            }
        }
    }
}



// Save the ppm file
void save(PPMImage* image){
    FILE* output_File;
    int row, col;
    int h = image->height;
    int w = image->width;
    int maxV = image->maxVal;
    // Open and name the file
    // Please change the path according to the specific location of your file
    output_File = fopen("im-gray.ppm", "wb");

    // Print header information of ppm file
    fprintf(output_File,"P3\n");
    fprintf(output_File,"%d %d\n",w,h);
    fprintf(output_File,"%d \n",maxV);
    // Write gray-scaled image data
    for (row = 0; row < h; row++) {
        for (col = 0; col < 3*w; col++) {
            fprintf(output_File,"%d ",image->data[row][col]);
        }
        fprintf(output_File,"\n");
    }
    // Close the ppm file
    fclose(output_File);
}
