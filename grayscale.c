#include <stdio.h>
#include <stdlib.h>
#include <string.h>



typedef struct
{
    int maxVal;          // Image maximum threshold
    int height;           //  to record the height of the image
    int width;          //  to record the width of the image
    int data[12000][12000];          // Store original  values array
    int ** new_data;      // Store updated  values array
}PPMImage;



void getPPMFile(char fileName[],PPMImage* img);    // Read the ppm file
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
int main()
{
    PPMImage* im;
    im = (PPMImage *)malloc(sizeof(PPMImage));/*allocate memory for ppm image*/
    getPPMFile("im.ppm",&(*im));// get ppm file
    int h = im->height;
    int w = im->width;
    // grayscale: get the avg of RGB values
    for (int row = 0; row < h ; row++) {
        for (int col = 0; col <3*w ; col++) {
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
    printf("%d",im->data[0][1]);

    save(im);    // Save the image
    return 0;
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


    /*Read image data*/
    for (row = 0; row < img-> height ; row++) {
        for (col = 0; col < 3*(img->width); col++) {
            fscanf(in_file,"%d",&(img->data[row][col]));
        }
    }


}




