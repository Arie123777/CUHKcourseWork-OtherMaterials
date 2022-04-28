#include "bmp.h"		//	Simple .bmp library
#include<iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

#define Baseline 30.0
#define Focal_Length 100
#define Image_Width 35.0
#define Image_Height 35.0
#define Resolution_Row 512
#define Resolution_Col 512
#define View_Grid_Row 9
#define View_Grid_Col 9

struct Point3d
{
	double x;
	double y;
	double z;
	Point3d(double x_, double y_, double z_) :x(x_), y(y_), z(z_) {}
};

struct Point2d
{
	double x;
	double y;
	Point2d(double x_, double y_) :x(x_), y(y_) {}
};


int main(int argc, char** argv)
{
	if(argc < 5 || argc > 6)
	{
		cout << "Arguments prompt: viewSynthesis.exe <LF_dir> <X Y Z> OR: viewSynthesis.exe <LF_dir> <X Y Z> <focal_length>" << endl;
		return 0;
	} // 5-6 args: .exe, 
	string LFDir = argv[1];
	double Vx = stod(argv[2]), Vy = stod(argv[3]), Vz = stod(argv[4]); //X Y Z
	double targetFocalLen = 100; // default focal length for "basic requirement" part
	if(argc == 6)
	{
		targetFocalLen = stod(argv[5]); //custom focal_length
	}
	

	vector<Bitmap> viewImageList;
	//! loading light field views
	for (int i = 0; i < View_Grid_Col*View_Grid_Row; i++)
	{
		char name[128];
		sprintf(name, "/cam%03d.bmp", i+1);
		string filePath = LFDir + name;
		Bitmap view_i(filePath.c_str());
		viewImageList.push_back(view_i);
	}

	Bitmap targetView(Resolution_Col, Resolution_Row);
	cout << "Synthesizing image from viewpoint: (" << Vx << "," << Vy << "," << Vz << ") with focal length: " << targetFocalLen << endl;
	//! resample pixels of the target view one by one
	for (int r = 0; r < Resolution_Row; r++)
	{
		for (int c = 0; c < Resolution_Col; c++)
		{
			Point3d rayRGB(0, 0, 0);
			//! resample the pixel value of this ray: TODO
			double roundDown = 0.00000001;
			Point3d eyeDir(((double)c / Resolution_Col - 0.5) * Image_Width, ((double)r / Resolution_Col - 0.5)* Image_Height, -targetFocalLen);
			Point2d viewPoint(Vx - Vz / eyeDir.z * eyeDir.x, Vy - Vz / eyeDir.z * eyeDir.y);
			Point2d stPoint(viewPoint.x / Baseline + 4, viewPoint.y / Baseline + 4);
			Point2d stPointInt(floor(stPoint.x + roundDown), floor(stPoint.y + roundDown));
			Point2d stAlphaBeta(stPoint.x-stPointInt.x, stPoint.y-stPointInt.y);
			vector<Point2d> stNeighbour;
			for (int i = 0; i < 4; i++)
				stNeighbour.push_back(Point2d(stPointInt.x + i % 2, stPointInt.y + i / 2));
				
			vector<vector<double>> stColor;
			for (int i = 0; i < 4; i++)
			{
				//printf("%lf %lf %d\n", stNeighbour[i].x, stNeighbour[i].y, stNeighbour[i].x < 0.0 || stNeighbour[i].y < 0.0 || stNeighbour[i].x + roundDown > 8.0 || stNeighbour[i].y + roundDown > 8.0);
				if (stNeighbour[i].x < 0-roundDown || stNeighbour[i].y < 0-roundDown || stNeighbour[i].x > 8 + roundDown|| stNeighbour[i].y > 8 + roundDown)
				{
					vector<double> myColor{-99999999, -99999999, -99999999};
					stColor.push_back(myColor);
					continue;
				}
				Point2d uvPoint((-Focal_Length / eyeDir.z * eyeDir.x / Image_Width + 0.5) * Resolution_Col , (-Focal_Length / eyeDir.z * eyeDir.y / Image_Height + 0.5) * Resolution_Row);
				Point2d uvPointInt(floor(uvPoint.x + roundDown), floor(uvPoint.y + roundDown));
				Point2d uvAlphaBeta(uvPoint.x - uvPointInt.x, uvPoint.y - uvPointInt.y);
				vector<Point2d> uvNeighbour;
				for (int j = 0; j < 4; j++)
					uvNeighbour.push_back(Point2d(uvPointInt.x + j % 2, uvPointInt.y + j / 2));
					
				vector<vector<double>> uvColor;
				for (int j = 0; j < 4; j++)
				{
					if (uvNeighbour[j].x < 0 - roundDown || uvNeighbour[j].y < 0 - roundDown || uvNeighbour[j].x > 511 + roundDown|| uvNeighbour[j].y > 511 + roundDown)
					{
						vector<double> myColor{-99999999, -99999999, -99999999};
					//	if (c==256)
					//		printf("fuck");
						uvColor.push_back(myColor);
						continue;
					}
					unsigned char myR, myG, myB;
					viewImageList[(unsigned int)(stNeighbour[i].x+9*stNeighbour[i].y+roundDown)].getColor((int)(uvNeighbour[j].x + roundDown), (int)(uvNeighbour[j].y + roundDown), myR, myG, myB);	
					vector<double> myColor{(double) myR, (double) myG, (double) myB};
					uvColor.push_back(myColor);
				}
				for (int j = 0; j < 4; j+=2)
				{
					if ((uvColor[j][0] >= 0 - roundDown) && (uvColor[j+1][0] >= 0 - roundDown))
					{
						vector<double> myColor;
						for (int k = 0; k < 3; k++)
							myColor.push_back(uvColor[j][k] * (1 - uvAlphaBeta.x) + uvColor[j+1][k] * uvAlphaBeta.x);
						uvColor.push_back(myColor);
					}
					else if (uvColor[j][0] >= 0 - roundDown)
						uvColor.push_back(uvColor[j]);
					else if (uvColor[j+1][0] >= 0 - roundDown)
						uvColor.push_back(uvColor[j+1]);
					else
					{
						vector<double> myColor {-99999999, -99999999, -99999999};
						uvColor.push_back(myColor);
					}
				}
				if ((uvColor[4][0] >= 0 - roundDown) && (uvColor[5][0] >= 0 - roundDown))
				{
					vector<double> myColor;
					for (int k = 0; k < 3; k++)
					myColor.push_back(uvColor[4][k] * (1 - uvAlphaBeta.y) + uvColor[5][k] * uvAlphaBeta.y);
					stColor.push_back(myColor);
				}
				else if (uvColor[4][0] >= 0 - roundDown)
					stColor.push_back(uvColor[4]);
				else if (uvColor[5][0] >= 0 - roundDown)
					stColor.push_back(uvColor[5]);
				else
				{
					vector<double> myColor {-99999999, -99999999, -99999999};
					stColor.push_back(myColor);
				}				
			} 
			
			for (int i = 0; i < 4; i+=2)
			{
					//printf("%lf %lf\n", stColor[i][0], stColor[i+1][0]);
					vector<double> myColor;
					for (int j = 0; j < 3; j++)
					myColor.push_back(stColor[i][j] * (1 - stAlphaBeta.x) + stColor[i+1][j] * stAlphaBeta.x);
					stColor.push_back(myColor);
			}
			vector<double> myColor;
			for (int j = 0; j < 3; j++)
			myColor.push_back(stColor[4][j] * (1 - stAlphaBeta.y) + stColor[5][j] * stAlphaBeta.y);
			stColor.push_back(myColor);
			//else if (stColor[4][0] > 0)
			//	stColor.push_back(stColor[4]);
			//else if (stColor[5][0] > 0)
			//	stColor.push_back(stColor[5]);
			//else	
			//! record the resampled pixel value
			rayRGB.x = max(0.01, stColor[6][0]+ roundDown);
			rayRGB.y = max(0.01, stColor[6][1])+ roundDown;
			rayRGB.z = max(0.01, stColor[6][2] + roundDown);
			//printf("%d %d %lf\n", c, r, rayRGB.x);
			targetView.setColor(c, r, (unsigned char) rayRGB.x, (unsigned char) rayRGB.y, (unsigned char) rayRGB.z);
		}
	}
	string savePath = "newView.bmp";
	targetView.save(savePath.c_str());
	cout << "Result saved!" << endl;
	return 0;
}
