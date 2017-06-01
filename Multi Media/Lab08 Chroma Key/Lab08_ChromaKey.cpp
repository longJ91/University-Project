#include <opencv2\core\core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>

#include <iostream>
#include <string>
#include <math.h>

using namespace std;
using namespace cv;

int main() {
	
	Mat beforeImage, backgroundImage;
	float innerThreshold, outerThreshold;

	String before("D:\\Program/OpenCV-3.2.0/opencv/sources/samples/data/before.jpg");
	String background("D:\\Program/OpenCV-3.2.0/opencv/sources/samples/data/background.jpg");

	beforeImage = imread(before.c_str(), IMREAD_COLOR);
	backgroundImage = imread(background.c_str(), IMREAD_COLOR);

	resize(backgroundImage, backgroundImage, beforeImage.size());

	Mat dst(Size(backgroundImage.cols, backgroundImage.rows), CV_8UC3);

	//Transform RGB to YCbCr
	cvtColor(beforeImage, beforeImage, CV_BGR2YCrCb);
	cvtColor(backgroundImage, backgroundImage, CV_BGR2YCrCb);

	int Cr_key = beforeImage.at<Vec3b>(0, 0)[1];
	int Cb_key = beforeImage.at<Vec3b>(0, 0)[2];

	//outerThreshold = sqrt(pow(Cr_key - 113, 2.0) + pow(Cb_key - 115, 2.0));
	//innerThreshold = sqrt(pow(Cr_key - 42, 2.0) + pow(Cb_key - 53, 2.0));

	outerThreshold = 115.0;
	innerThreshold = 100.0;

	for (int x = 0; x < beforeImage.rows; x++) {
		for (int y = 0; y < beforeImage.cols; y++) {
			float alpha, D;

			D = sqrt(pow((Cr_key - beforeImage.at<Vec3b>(x,y)[1]), 2.0) + pow((Cb_key - beforeImage.at<Vec3b>(x, y)[2]), 2.0));
			
			if (D <= innerThreshold) alpha = 1.0f;
			else if (D >= outerThreshold) alpha = 0.0f;
			else alpha = (D - innerThreshold) / (outerThreshold - innerThreshold);

			for (int k = 0; k < 3; k++) {
				dst.at<Vec3b>(x, y)[k] = (1.0 - alpha) * beforeImage.at<Vec3b>(x, y)[k] + (alpha * backgroundImage.at<Vec3b>(x, y)[k]);
			}
		}
	}

	cvtColor(dst, dst, CV_YCrCb2BGR);

	//Display the before
	namedWindow("Display window before", WINDOW_AUTOSIZE);
	imshow("Display window before", dst);

	waitKey(0);

	return 0;
	
}