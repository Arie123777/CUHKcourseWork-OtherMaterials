/*

CSCI 3280, Introduction to Multimedia Systems
Spring 2022

Assignment 01 Skeleton

bonus.cpp

*/

#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <string.h>
#include "bmp.h"		//	Simple .bmp library

#define MAX_SHADES 8

char shades[MAX_SHADES] = {'@','#','%','*','|','-','.',' '};
void bitmapCompress(Bitmap image_data);
void eightLevelBitmap(Bitmap image_data);
void myLevelBitmap(Bitmap image_data);
#define SAFE_FREE(p)  { if(p){ free(p);  (p)=NULL;} }


int main(int argc, char** argv)
{
	Bitmap image_data(argv[1]);
	int height = image_data.getHeight();
	int width = image_data.getWidth();
	char input = '?';
	if (height > 256 || width > 256)
	{
		while (!(input == 'N' || input == 'Y'))
		{
		printf("The height/weight is LARGER than 256 bits, compress? (Y/N) ");
		scanf("\n%c", &input);
		if (input == 'Y')
		{
			bitmapCompress(image_data);
			printf("Lu! Compression Complete! Please run the program again with your resized file! :D\n");
		}
		if (!(input == 'N' || input == 'Y'))
			printf("BUUUUUUU! Wrong input!\n");
		}
		return 0;
	}
	while (!(input == 'A' || input == 'B' || input == 'C'))
	{
		printf("What do you want to try?\n");
		printf("A: The 8-level ASCII bitmap library!\n");
		printf("B: Try the new ASCII bitmap library! (With COLORED!!!)\n");
		printf("C: I don't want to try ANYTHING! Let me QUIT! :P\n");
		scanf("\n%c", &input);
		if (!(input == 'A' || input == 'B' || input == 'C'))
			printf("BUUUUUUU! Wrong input!\n");
	}
	if (input == 'A')
		eightLevelBitmap(image_data);
	if (input == 'B')
		myLevelBitmap(image_data);
	printf("lu! This program is quited very safely!");
    return 0;
}

void bitmapCompress(Bitmap image_data)
{
	int height = image_data.getHeight();
	int width = image_data.getWidth();
	int want_height = height, want_width = width, want_ratio = 1;
	while (want_height > 256 || want_width > 256)
	{
		want_height /= 2;
		want_width /= 2;
		want_ratio *= 2;
	}
	Bitmap new_image(want_width, want_height);
	for (int i = 0; i < want_height; i++)
	{
		for (int j = 0; j < want_width; j++)
		{
			unsigned char cr, cg, cb;
			int totalr = 0, totalg = 0, totalb = 0;
			for (int k = 0; k < want_ratio; k++)
				for (int l = 0; l < want_ratio; l++)
				{
					image_data.getColor(j*want_ratio+k, i*want_ratio+l, cr, cg, cb);
					totalr += (int)cr;
					totalg += (int)cg;
					totalb += (int)cb;
				}
			new_image.setColor(j, i, totalr/want_ratio/want_ratio, totalg/want_ratio/want_ratio, totalb/want_ratio/want_ratio);
		}			
	}
	new_image.save("compressed.bmp");
}

void eightLevelBitmap(Bitmap image_data)
{
	Bitmap our_image(image_data.getWidth()*8, image_data.getHeight()*8);
	Bitmap bitmap_shades[8];
	for (char i = 0; i < 8; i++)
	{
		char ind[10] = {i+'0', '\0'};
		char shade_path[500] = "shades\\";
		strcat(shade_path, (strcat(ind, ".bmp")));
		bitmap_shades[i].create(shade_path);
	}
	for (int i = 0; i < image_data.getWidth(); i++)
		for (int j = 0; j < image_data.getHeight(); j++)
		{
			unsigned char r, g, b;
			image_data.getColor(i, j, r, g, b);
			double gray = r * 0.299 + g * 0.587 + b * 0.114;
			int myshade = (int)gray/32;
			for (int k = 0; k < 8; k++)
				for (int l = 0; l < 8; l++)
				{
					unsigned char cr, cg, cb;
					bitmap_shades[myshade].getColor(k, l, cr, cg, cb);
					our_image.setColor(i*8+k, j*8+l, cr, cg, cb);
				}
		}
	printf("Tell me what is the name of this new file! ");
	char savepath[256];
	scanf("%s", savepath);
	our_image.save(savepath);
}

void myLevelBitmap(Bitmap image_data)
{
	Bitmap our_image(image_data.getWidth()*8, image_data.getHeight()*8);
	Bitmap bitmap_shades[16];
	for (char i = 0; i < 16; i++)
	{
		char ind[10] = {i+'0', '\0'};
		if (i >= 10)
		{
			ind[0] = '1';
			ind[1] = i-10+'0';
			ind[2] = '\0';
		}
		char shade_path[500] = "shades\\my\\";
		strcat(shade_path, (strcat(ind, ".bmp")));
		bitmap_shades[i].create(shade_path);
	}
	for (int i = 0; i < image_data.getWidth(); i++)
		for (int j = 0; j < image_data.getHeight(); j++)
		{
			unsigned char r, g, b;
			image_data.getColor(i, j, r, g, b);
			double gray = r * 0.299 + g * 0.587 + b * 0.114;
			int myshade = (int)gray/16;
			for (int k = 0; k < 8; k++)
				for (int l = 0; l < 8; l++)
				{
					unsigned char cr, cg, cb;
					bitmap_shades[myshade].getColor(k, l, cr, cg, cb);
					if (cr*cg*cb != 0)
						our_image.setColor(i*8+k, j*8+l, r, g, b);
					else
						our_image.setColor(i*8+k, j*8+l, 255, 255, 255);
				}
		}
	printf("Tell me what is the name of this new file! ");
	char savepath[256];
	scanf("%s", savepath);
	our_image.save(savepath);
	char input = '?';
	while (!(input == 'Y' || input == 'N'))
	{
		printf("And one more thing! Do you want a black/white version of that picture? (Y/N) ");
		scanf("\n%c", &input);
		if (!(input == 'Y' || input == 'N'))
			printf("BUUUUUUUU! Wrong input!\n");
	}
	if (input == 'Y')
	{
		for (int i = 0; i < our_image.getWidth(); i++)
			for (int j = 0; j < our_image.getHeight(); j++)
			{
				unsigned char r, g, b;
				our_image.getColor(i, j, r, g, b);
				if (r+g+b < 765)
					our_image.setColor(i, j, 0, 0, 0);
			}
		printf("Again, tell me what is the name of this new file! ");
		scanf("%s", savepath);
		our_image.save(savepath);
	}
	
}
