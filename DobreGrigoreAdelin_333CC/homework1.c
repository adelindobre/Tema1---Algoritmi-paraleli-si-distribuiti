#include "homework1.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>

int num_threads;
int resolution;

void initialize(image *im) 
{
	int i,j;

	im->image_type = (char*)malloc(2*sizeof(char));
	strcpy(im->image_type, "P5");
	im->width = resolution;
	im->height = resolution;
	im->maxval = 255;

	im->matrix = (unsigned char**)malloc(im->height * sizeof(unsigned char*));
	for(i = 0; i < im->height; i++)
		im->matrix[i] = (unsigned char*)malloc(im->width * sizeof(unsigned char));


}

double calculateDistance(int x, int y) {
    return abs(-1 * x + 2 * y + 0)/sqrt(1 + 4);
}


void render(image *im) 
{
	int i,j;
	int dist;

	omp_set_num_threads(num_threads);
	#pragma omp parallel for \
	shared(resolution, im) \
	private(j)

	for(i = 0; i < im->height; i++)
		for(j = 0; j < im->width; j++)
		{
			if (calculateDistance(j * (100.0/resolution) + (100.0/resolution/2), i * (100.0/resolution) + (100.0/resolution/2)) < 3)
				im->matrix[i][j] = 0;
			else
				im->matrix[i][j] = 255;
		}		
}

void writeData(const char * fileName, image *img) 
{
	FILE *f;
	int i,j;

	f = fopen(fileName, "w");
	fseek(f,SEEK_SET, 0);

	fprintf(f, "%s\n", img->image_type);
	fprintf(f, "%d %d\n", img->width, img->height);
	fprintf(f, "%d\n", img->maxval);

	for(i = img->height - 1; i >= 0; i--)
		for(j = 0; j < img->width; j++)
			fwrite(&img->matrix[i][j], sizeof(unsigned char), 1, f);

	fclose(f);
}

