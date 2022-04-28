#include "bmp.h"		//	Simple .bmp library
#include<iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <vector>

using namespace std;
#define PI 3.14159265358979
int QuantizationMatrix[8][8] = {
	{3, 5, 7, 9, 11, 13, 15, 17}, 
	{5, 7, 9, 11, 13, 15, 17, 19},
	{7, 9, 11, 13, 15, 17, 19, 21},
	{9, 11,	13,	15,	17,	19,	21,	23},
	{11, 13, 15, 17, 19, 21, 23, 25},
	{13, 15, 17, 19, 21, 23, 25, 27},
	{15, 17, 19, 21, 23, 25, 27, 29},
	{17, 19, 21, 23, 25, 27, 29, 31}
	};

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		cout << "Arguments prompt: dct.exe <img_path> <apply_idct>" << endl;
		return 0;
	}
	string imgPath = argv[1];
	bool need_idct = stoi(argv[2]);

	//! read input image
	Bitmap s_img(imgPath.c_str());
	int rows = s_img.getHeight(), cols = s_img.getWidth();
	cout << "Apply DCT on image ("<<rows<<", "<<cols<< ")." << endl;
	
	//! preprocess by shifting pixel values by 128
	//TODO
	int preprocessed[256][256];
	for (int i = 0; i < 256; i++)
		for (int j = 0; j < 256; j++)
		{
			unsigned char mycolor;
			s_img.getPixel(i, j, mycolor);
			preprocessed[i][j] = (int)mycolor - 128;
		}
	//! 2D DCT for every 8x8 block (assume that the input image resolution is fixed to 256)	
	// The quantized coefficients should be stored into 'coeffArray'
	double coeffArray[256][256]={0};
	int blockRow = rows / 8, blockCol = cols / 8;
	for (int i = 0; i < blockRow; i++)
	{
		for (int j = 0; j < blockCol; j++)
		{
			double rowArray[8][8];
			int xpos = j*8, ypos = i*8;
			//! apply DCT on block_ij (basic requirement)
			//TODO
			for (int u = 0; u < 8; u++)
			{
				for (int v = 0; v < 8; v++)
				{
					double hi = 1.0/2 * ((u == 0)? 1.0/sqrt(2) : 1.0);
					double xtotal = 0;
					for (int x = 0; x < 8; x++)
					{
						xtotal += cos((2.0*x + 1.0)*u*PI/16.0)*preprocessed[xpos+x][ypos+v];
					}
					rowArray[u][v] = xtotal * hi;
				}
			}
			
			double colArray[8][8];
			for (int u = 0; u < 8; u++)
			{
				for (int v = 0; v < 8; v++)
				{
					double hi = 1.0/2 * ((v == 0)? 1.0/sqrt(2) : 1.0);
					double ytotal = 0;
					for (int y = 0; y < 8; y++)
					{
						ytotal += cos((2.0*y + 1.0)*v*PI/16.0)*rowArray[u][y];
					}
					colArray[u][v] = ytotal * hi;
					//printf("%lf\n", colArray[u][v]);
				}
			}
			//! quantize the frequency coefficient of this block (basic requirement)
			//TODO
			for (int u = 0; u < 8; u++)
				for (int v = 0; v < 8; v++)
					coeffArray[xpos+u][ypos+v] = round(colArray[u][v]/QuantizationMatrix[u][v]);
		}
	}
	
	//! output the computed coefficient array
	FILE *fp = fopen("coeffs.txt", "w");
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			fprintf(fp, "%3.3lf ", coeffArray[c][r]);
		}
		fprintf(fp, "\n");
	}
	cout << "Quantized coefficients saved!" << endl;

	if (need_idct)
	{
		Bitmap reconstructedImg(cols, rows);
		//! apply IDCT on the quantized coefficients (enhancement part)
		
		for (int i = 0; i < blockRow; i++)
		{
			for (int j = 0; j < blockCol; j++)
			{
				int xpos = j*8, ypos = i*8;
				//! apply de-quantization on block_ij 
				//TODO
				double block_element[8][8];
				for (int u = 0; u < 8; u++)
					for (int v = 0; v < 8; v++)
						block_element[u][v] = coeffArray[xpos+u][ypos+v] * QuantizationMatrix[u][v];
				//! apply IDCT on this block
				//TODO
				double rowArray[8][8];
				for (int x = 0; x < 8; x++)
				{
					for (int y = 0; y < 8; y++)
					{
						
						double utotal = 0;
						for (int u = 0; u < 8; u++)
						{
							double hi = 1.0/2 * ((u == 0)? 1.0/sqrt(2) : 1.0);
							utotal += cos((2.0*x + 1.0)*u*PI/16.0)*block_element[u][y] * hi;
						}
						rowArray[x][y] = utotal;
					}
				}
				
				double colArray[8][8];
				for (int x = 0; x < 8; x++)
				{
					for (int y = 0; y < 8; y++)
					{
						double vtotal = 0;
						for (int v = 0; v < 8; v++)
						{
							double hi = 1.0/2 * ((v == 0)? 1.0/sqrt(2) : 1.0);
							vtotal += cos((2.0*y + 1.0)*v*PI/16.0)*rowArray[x][v] * hi;
						}
						colArray[x][y] = vtotal;
					}
				}
				//! shiftting back the pixel value range to 0~255
				//TODO
				for (int x = 0; x < 8; x++)
					for (int y = 0; y < 8; y++)
					{
						unsigned char my_color = min(max ((int) round(colArray[x][y]) + 128, 0), 255);
						reconstructedImg.setPixel(xpos+x, ypos+y, my_color);
					}
			}
		}
		string savePath = "reconstructedImg.bmp";
		reconstructedImg.save(savePath.c_str());
		cout << "reconstructed image saved!" << endl;
	}

	return 0;
}
