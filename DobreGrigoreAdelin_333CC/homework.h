#ifndef HOMEWORK_H
#define HOMEWORK_H

typedef struct{
	char* image_type;
	int width, height;
	int maxval;
	unsigned char** matrix;
}image; 

#endif