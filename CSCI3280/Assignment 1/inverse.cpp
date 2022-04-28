/*

CSCI 3280, Introduction to Multimedia Systems
Spring 2022

Assignment 01 Skeleton

inverse.cpp

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
    // 1. Open the txt file
    //
    FILE *fp = fopen(argv[1], "r");
    if (!fp)
        return -1;


    //
    // Your code goes here ////
    //
    //
    // 2. read the ascii art file.
    //
	int height, width;
	fscanf(fp, "%d %d", &height, &width);
	//fp = 
	//Bitmap:create(height, width);
	Bitmap myBmp(height, width);
	//printf("%d %d\n", height, width);
    //
    // 3. Get RGB values for the resultant bitmap
    //
    //char *readstring = (char*)malloc(sizeof(char) * (width+1));
    char readchar;
	for (int i = 0; i < height;i++)
	{
		readchar = fgetc(fp);
		for (int j = 0; j < width; j++)
		{
			readchar = fgetc(fp);
			for (int k = MAX_SHADES; k >= 0; k--)
			{
				
				if (readchar == shades[k])
				{
					myBmp.setColor(j, i, (int)((MAX_SHADES -1 - k) * 255/7), (int)((MAX_SHADES -1 - k) * 255/7),  (int)((MAX_SHADES -1 - k) * 255/7));
					//break;
				}
			}
			
		}
		//printf("%d ", i);
		//puts(readstring);
	}
    //
    // 4. Store the BMP file to the given place
    //
	myBmp.save(argv[2]);
    fclose(fp);
    // free memory if any


    return 0;
}
