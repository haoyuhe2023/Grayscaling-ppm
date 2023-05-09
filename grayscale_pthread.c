#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>



#ifndef _TIMER_H_
#define _TIMER_H_

#include <sys/time.h>
/* time function to record the running time of grayscale*/
#define GET_TIME(now) { \
struct timeval t; \
gettimeofday(&t, NULL); \
now = t.tv_sec + t.tv_usec/1000000.0; \
}

#endif
/*shared variable,thread number*/
int total_thread;


typedef struct
{
    int maxVal;          // Image maximum threshold
    int height;           //   record the height of the image
    int width;          //   record the width of the image
    int ** data;          // Store original  values array
}PPMImage;


/*read the ppm file*/
void getPPMFile(char fileName[],PPMImage* img);
/*grayscale function*/
void* gray(void*param);
/*save the ppm image*/
void save(PPMImage* image);


PPMImage* im;

int main(int argc,char *argv[])
{   /*allocate memory for ppm image*/
    im = malloc(sizeof(PPMImage));
/*time variables*/
    double start,finish,elapsed;
/*long is used in case of 64 bit*/
    long thread;
/*each pthread memory pthread_handles*/
    pthread_t* thread_handles;
/*Get number of threads from command line */

    total_thread = strtol(argv[1],NULL,10);
/*allocate each memory to thread*/
    thread_handles = malloc(total_thread*sizeof(pthread_t));

/*change the route of the file according to situation*/
    getPPMFile("im.ppm",&(*im));
/*get time before grayscale*/
    GET_TIME(start);
    for (thread= 0; thread < total_thread; thread++) {
        pthread_create(&thread_handles[thread],NULL, gray,(void*) thread);
    }
    for (thread = 0; thread < total_thread; thread++) {
        pthread_join(thread_handles[thread],NULL);
    }
/*get the finished time*/
    GET_TIME(finish);
/*save images*/
    save(im);
/*get elapsed time*/
    elapsed=finish-start;
    printf("The running time is:%.6f\n",elapsed);

/*free memory */
    free(thread_handles);
    return 0;
}


void getPPMFile(char fileName[],PPMImage* img){
    FILE* in_file;

    char version[100];
    int row,col;
/*Open the file*/
    in_file = fopen(fileName,"rb");

/*File none error*/
    if (in_file == NULL)
    {
        fprintf(stderr, "Error: Unable to open file %s\n\n", fileName);
        exit(1);
    }
    printf("\nReading image file: %s\n", fileName);



/*Read ppm image */
    fscanf(in_file,"%s",version);
    fscanf(in_file,"%d",&(img->width));
    fscanf(in_file,"%d",&(img->height));
    fscanf(in_file,"%d",&(img->maxVal));

/*Show ppm image */
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
/*grayscale image*/
void *gray(void* param) {
    int row,col;
    int h=im->height;
    int w=im->width;
    long x=(long)param;
/*grayscale: get the avg of RGB values*/
    for ( row = x+1; row <h; row +=total_thread) {
        for ( col = 0; col < 3 * w; col++) {
            if ((col + 1) % 3 == 0) {
                int record = (im->data[row][col] + im->data[row][col - 1] + im->data[row][col - 2]) / 3;
                im->data[row][col] = record;
                im->data[row][col - 1] = record;
                im->data[row][col - 2] = record;
            } else {
                continue;
            }
        }
    }
    return 0;
}

/*save ppm image*/
void save(PPMImage* image){
    FILE* output_File;
    int row, col;
    int h = image->height;
    int w = image->width;
    int maxV = image->maxVal;
/*change the path according to the specific location of your file*/
    output_File = fopen("im-gray.ppm", "wb");

/*Print header information of ppm file*/
    fprintf(output_File,"P3\n");
    fprintf(output_File,"%d %d\n",w,h);
    fprintf(output_File,"%d \n",maxV);
/* write the data of the file*/
    for (row = 0; row < h; row++) {
        for (col = 0; col < 3*w; col++) {
            fprintf(output_File,"%d ",image->data[row][col]);
        }
        fprintf(output_File,"\n");
    }
/* close the file*/
    fclose(output_File);
}


