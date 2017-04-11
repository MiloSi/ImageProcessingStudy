#pragma once
#pragma warning(disable : 4819)
#pragma warning(disable : 4996)


#include <iostream>
#include <opencv2/opencv.hpp>

/*************************************************
Latest Version 2017 - 04 - 05

Created by Milo Si, (or call me C)
This program is to study Multimedia (Computer Vision) using OpenCV
This program has 2 Fucntion.
First  :  Average Filter.
Second :  Gaussian Filter.
**************************************************/


//#include <amp.h>
using namespace std;
using namespace cv;

/*
inline T MULT(T value)
inline T MULT(T value1, T value2)
return
T value			(One Parameter)	  -> pow(value,  2)
T value1, T value2 (Two Parameters)  -> pow(value1, 2) + pow(value2, 2)

these functions are used when calculate Gaussian distribution.

G(x, y)  = exp(-MULT<float>(x, y) / (2 * MULT<float>(sigma))) / (2 * (3.141592...) * MULT<float>(sigma));

*/
template <typename T>
inline T MULT(T value) {
	return value*value;
};
template <typename T>
inline T MULT(T value1, T value2)
{
	return (value1 * value1) + (value2 * value2);
};

/*
uchar calculation(Mat kernal, Mat pixel)

Param	: Mat, Mat
Return  : uchar

kernal  is the mask.
pixel   is the one of parts of image.

This function used for convoltion.
Mutiply kernal's coefficients and pixel's coefficients.
And and sum all cofficients, then return the result.

For example

kernal = {						pixel =  {
a00, a01, a02,					b00, b01, b02,
a10, a11, a12,					b10, b11, b12,
a20, a21, a22					b20, b21, b22
}								}

dst = (a00 * b00 )+ (a01 * b01) .....  (a21 * b21) + (a22 * b22)

then return dst.

*/
uchar calculation(Mat kernal, Mat pixel)
{
	float dst = 0;
	pixel.convertTo(pixel, CV_32F);

	for (int i = 0; i < pixel.rows; i++)
	{
		for (int j = 0; j < pixel.cols; j++)
		{
			dst += pixel.at<float>(i, j) * kernal.at<float>(i, j);
		}

	}
	return (uchar)dst;
}

/*************************************************

Mat averageFunc(Mat src, int nSize)

Func	:  Remove image noise using Average filter.
Param	:  Mat, int
Return  :  Mat

src		is original image
nSize	is mask(kernal) size

Return filtered image


for example,
if nSize == 3

kernal =  {
{1, 1, 1},
{1, 1, 1},
{1, 1, 1}
}
and,  kernal =  kernal / (3*3) then,
kernal =  {
{1/9, 1/9, 1/9},
{1/9, 1/9, 1/9},
{1/9, 1/9, 1/9}
}
**************************************************/


Mat averageFunc(Mat src, const int nSize)
{
	Mat kernal; // this is Median Kernal
	Mat dst = src.clone();


	kernal = Mat::ones(Size(nSize, nSize), CV_32F);								//setting
	kernal /= (nSize * nSize);


	cout << kernal << endl;

	int half = nSize >> 1;

	for (int y = half; y < src.rows - half; y++)
	{
		for (int x = half; x < src.cols - half; x++)
		{
			Mat pixel_group(src, Rect(x - half, y - half, nSize, nSize));
			dst.at<uchar>(y, x) = calculation(kernal, pixel_group.clone());
			//pixel_group(uchar)
		}
	}

	return dst;
}
/*************************************************

Mat gaussianFunc(Mat src, int nSize, float sigma)


Func		:  remove image noise using gaussian distribution
Param		:  Mat, int, float
Return type :  Mat

src		is original image.
nSize	is Mask(Kernal) size
sigma	is used Gaussian distribution

and return filtered image.
**************************************************/

Mat gaussianFunc(Mat src, int nSize, float sigma)
{
	Mat kernal(nSize, nSize, CV_32F);
	int half = nSize >> 1;

	//Calcuate Gaussian distribution
	for (int y = 0; y < nSize; y++)
	{
		for (int x = 0; x < nSize; x++)
		{
			float kernal_x = x - half;
			float kernal_y = y - half;
			kernal.at<float>(y, x)
				= exp(-MULT<float>(kernal_y, kernal_x) / (2 * MULT<float>(sigma)))
				/ (2 * CV_PI * MULT<float>(sigma));
		}
	}

	//Show Gaussian distribution
	cout << kernal << endl;
	Mat dst = src.clone();


	//Convolution 
	for (int y = half; y < src.rows - half; y++)
	{
		for (int x = half; x < src.cols - half; x++)
		{
			Mat pixel_group(src, Rect(x - half, y - half, nSize, nSize));
			dst.at<uchar>(y, x) = calculation(kernal, pixel_group.clone());
			//pixel_group(uchar)
		}
	}
	return dst;
}


/*************************************************

void showFunc(const char *name, Mat ori, Mat src)

Func		:  Show us two images. One is original image, the other is filtered image.
Param		:  char, Mat, Mat
Return type :  void

name	is window name.
ori		is original Image.
src		is filtered Image.

**************************************************/
void showFunc(const char *name, Mat ori, Mat src)
{

	namedWindow("Original");
	imshow("Original", ori);


	namedWindow(name);
	imshow(name, src);

	waitKey(0);

	cvDestroyAllWindows();
}
/*************************************************

int command(const int turn, float& sigma)

Func		:	Operate two Image processing
First is average filter.
Seceond is gaussian filter.

Param		:  int, float
Return type :  int

turn	is used to distinguish two image processing.
sigma	is the sigma of Gaussian distribution.

Return mask size.
**************************************************/

int command(const int turn, float& sigma)
{
	int nSize;

	if (turn == 0)
		cout << "FIRST  - AVERAGE FILTERING" << endl;
	else
		cout << "SECOND - GAUSSIAN FILTERING" << endl;


	cout << "INPUT 'N' SIZE (Only,  N >= 3 And N is Odd) :  ";
	cin >> nSize;
	while (nSize < 3)
	{
		cerr << "ERROR, INPUT NUMBER IS UNDER 3. TRY AGIAN" << endl;
		cout << "INPUT 'N' SIZE (Only,  N >= 3 And N is Odd) :  ";
	}

	if (nSize % 2 == 0) {
		cerr << "INPUT NUMBER IS " << nSize << "BUT, I WANT ODD NUMBER. SO, ADD 1" << endl;
		cout << "N + 1 = " << ++nSize << endl;
	}


	if (turn == 1)
	{
		cout << "INPUT 'SIGMA' SIZE (Only sigma >= 0) : ";
		cin >> sigma;

		if (sigma <= 0)
		{
			cerr << "INPUT 'SIGMA' IS " << nSize << "IT IS WRONG NUMBER, SO DEFAULT NUMBER IS 1" << endl;
			cout << "SIGMA = 1" << endl;
			sigma = 1;
		}
		else
		{
			cout << "SIGMA = " << sigma << endl;
		}

	}

	return nSize;

}


int main(int argc, wchar_t** argv)
{
	int nSize;
	float sigma = 0;
	Mat src;
	String file_name = "./lenna.bmp";

	src = imread(file_name.c_str(), IMREAD_GRAYSCALE);
	if (src.empty())
	{
		cerr << "THERE IS NO FILE NAME!" << endl;
		return 0;
	}
	for (int i = 0; i < 2; i++)
	{
		nSize = command(i, sigma);
		if (i == 0)	
			showFunc("Average Filtering", src, averageFunc(src.clone(), nSize));
		else
			showFunc("Guassian Filtering", src, gaussianFunc(src.clone(), nSize, sigma));
	}
	return 0;
}