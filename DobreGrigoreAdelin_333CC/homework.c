#include "homework.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

int num_threads;
int resize_factor;

void readInput(const char * fileName, image *img) 
{
	FILE *f;
	int i; 

	f = fopen(fileName, "rb");
	fseek(f, SEEK_SET, 0);

	img->image_type = (char*)malloc(2*sizeof(char));

	fscanf(f, "%s", img->image_type);
	fscanf(f, "%d", &img->width);
	fscanf(f, "%d", &img->height);
	fscanf(f, "%d\n", &img->maxval);	

	if(strcmp(img->image_type, "P6") == 0)
	{
		img->matrix = (unsigned char**)malloc(img->height * sizeof(unsigned char*));
		for(i = 0; i < img->height; i++)
			img->matrix[i] = (unsigned char*)malloc(3 * img->width * sizeof(unsigned char));
	
		for(i = 0; i < img->height; i++)	
			fread(img->matrix[i], sizeof(unsigned char), 3 * img->width, f);
	}
	else
	{
		img->matrix = (unsigned char**)malloc(img->height * sizeof(unsigned char*));
		for(i = 0; i < img->height; i++)
			img->matrix[i] = (unsigned char*)malloc(img->width * sizeof(unsigned char));	
	
		for(i = 0; i < img->height; i++)
			fread(img->matrix[i], sizeof(unsigned char), img->width, f);						
	}
	fclose(f);
}

void writeData(const char * fileName, image *img)
{
	FILE *f;
	int i;

	f = fopen(fileName, "w");
	fseek(f, SEEK_SET, 0);

	fprintf(f, "%s\n", img->image_type);
	fprintf(f, "%d %d\n", img->width, img->height);
	fprintf(f, "%d\n", img->maxval);

	if(strcmp(img->image_type, "P6") == 0)
	{
		for(i = 0; i < img->height; i++)
			fwrite(img->matrix[i], sizeof(unsigned char), 3 * img->width, f);	
	}	
	else
	{
		for(i = 0; i < img->height; i++)
			fwrite(img->matrix[i], sizeof(unsigned char), img->width, f);	
	}
	fclose(f);	
}

void resize(image *in, image *out) 
{ 
	int i, j, l, c, lin, col, suma_gr, suma_r, suma_g, suma_b;
	int **kernel, nr;

	out->image_type = (char*)malloc(2 * sizeof(char));
	strcpy(out->image_type, in->image_type);
	
	out->height = in->height/resize_factor;
	out->width = in->width/resize_factor;
	out->maxval = in->maxval;


	if (strcmp(out->image_type, "P6") == 0)
	{
		out->matrix = (unsigned char**)malloc(out->height * sizeof(unsigned char*));
		for(i = 0; i < out->height; i++)
			out->matrix[i] = (unsigned char*)malloc(3 * out->width * sizeof(unsigned char));
	}
	else
	{
		out->matrix = (unsigned char**)malloc(out->height * sizeof(unsigned char*));
		for(i = 0; i < out->height; i++)
			out->matrix[i] = (unsigned char*)malloc(out->width * sizeof(unsigned char));
	}
	
	kernel = (int**)malloc(resize_factor * sizeof(int*));
	for(i = 0; i < resize_factor; i++)
		kernel[i] = (int*)malloc(resize_factor * sizeof(int));
	
	for(i = 0; i < resize_factor; i++)
		for(j = 0; j < resize_factor; j++)
			kernel[i][j] = 1;
	nr = resize_factor * resize_factor;

	if(resize_factor == 3)
	{
		nr = 16;
		for(i = 0; i < resize_factor; i++)
		{
			for(j = 0; j < resize_factor; j++)
			{
				if(i % 2 == 0)
				{
					if(j % 2 == 0)
						kernel[i][j] == 1;
					else
						kernel[i][j] *= 2;
				}
				else
				{
					if(j % 2 == 0)
						kernel[i][j] *= 2;
					else
						kernel[i][j] *= 4;					
				}
			}
		}
	}

	if(strcmp(in->image_type, "P5") == 0)
	{

		omp_set_num_threads(num_threads);
		
		#pragma omp parallel for \
		firstprivate(l, nr, kernel) \
		shared(in, out, resize_factor) \		
		private(j, lin, col, suma_gr, c)
	
		for(i = 0; i < out->height; i++)
		{
			l = i * resize_factor;
			c = 0;
			for(j = 0; j < out->width; j++)
			{
				suma_gr = 0;
				for(lin = l; lin < l + resize_factor; lin++)
					for(col = c; col < c + resize_factor; col++)						
						suma_gr += (in->matrix[lin][col]  * kernel[lin - l][col - c]);

				out->matrix[i][j] = suma_gr/nr;			
				c += resize_factor;
			}
		}
	}

	if(strcmp(in->image_type, "P6") == 0)
	{
		omp_set_num_threads(num_threads);
		
		#pragma omp parallel for\
		firstprivate(l, nr, kernel)\
		shared(in, out, resize_factor)\		
		private(j, lin, col, suma_r, suma_g, suma_b, c)
		
		for(i = 0; i < out->height; i++)
		{
			l = i * resize_factor;
			c = 0;
			for(j = 0; j < out->width; j++)
			{
				suma_r = 0;
				suma_g = 0;
				suma_b = 0;

				for(lin = l; lin < l + resize_factor; lin++)
				{
					for(col = c; col < (c + resize_factor * 3); col = col + 3)
					{
						suma_r += (in->matrix[lin][col] * kernel[lin - l][(col - c)/3]);
						suma_g += (in->matrix[lin][col + 1] * kernel[lin - l][(col - c)/3]);
						suma_b += (in->matrix[lin][col + 2] * kernel[lin - l][(col - c)/3]);
					}
				}

				out->matrix[i][j * 3] = suma_r/nr;
				out->matrix[i][j * 3 + 1] = suma_g/nr;
				out->matrix[i][j * 3 + 2] = suma_b/nr;			
				
				c += (resize_factor * 3);
			}			
		}		
	}
}


