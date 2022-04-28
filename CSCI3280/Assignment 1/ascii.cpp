/*

CSCI 3280, Introduction to Multimedia Systems
Spring 2022

Assignment 01 Skeleton

ascii.cpp

*/

#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include "bmp.h"		//	Simple .bmp library

#define MAX_SHADES 8

char shades[MAX_SHADES] = {'@','#','%','*','|','-','.',' '};

#define SAFE_FREE(p)  { if(p){ free(p);  (p)=NULL;} }


int main(int argc, char** argv)
{
	//
	//	1. Open BMP file
	//
	Bitmap image_data(argv[1]);

	if(image_data.getData() == NULL)
	{
		printf("unable to load bmp image!\n");
		return -1;
	}

	int height = image_data.getHeight();
	int width = image_data.getWidth();
	char output[256][257];
	//for (int i = 0; i < height; i++)
		
	//
	//	Your code goes here ....
	//
	//	2. Obtain Luminance
	//
	for (int i = 0; i < height; i++)
	{
		//output[i] = (char *) malloc(width * sizeof(char) + 1);
		for (int j = 0; j < width; j++)
		{
			unsigned char r, g, b;
			image_data.getColor(j, i, r, g, b);
			double gray = r * 0.299 + g * 0.587 + b * 0.114;
			char myascii = shades[MAX_SHADES - 1 - (int)gray/32];
			output[i][j] = myascii;
		}
		output[i][width] = '\0';
	}
	//
	//	3. Quantization & ascii mapping
	//
	//  4. ASCII art txt file
	//
	FILE *fp = fopen(argv[2], "w");
	if (!fp)
		return -1;
	
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
			fputc(output[i][j], fp);
		fputc('\n', fp);
	}

	fclose(fp);
	
	
	//  free memory if any

	return 0;
} 
