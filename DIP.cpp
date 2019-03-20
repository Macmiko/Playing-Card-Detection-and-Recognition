#include "DIP.h"

using namespace cv;

Mat DIP::createMask(const cv::Mat image)
{
	
	//filtration
	medianBlur(image, image, 3);
	betterContrast(image, 1.2, 10);

	//create binary image
	Mat gray_image,mask;
	cvtColor(image, gray_image, COLOR_BGR2GRAY);
	threshold(gray_image, mask, 80, 255, THRESH_BINARY);

	return mask;
}

void DIP::betterContrast( Mat image, double alpha, int beta)
{

	///from documentation : https://docs.opencv.org/2.4/doc/tutorials/core/basic_linear_transform/basic_linear_transform.html
	/// Do the operation new_image(i,j) = alpha*image(i,j) + beta
	
	for (int y = 0; y < image.rows; y++)
	{
		for (int x = 0; x < image.cols; x++)
		{

			for (int c = 0; c < 3; c++)
			{
				image.at<Vec3b>(y, x)[c] = saturate_cast<uchar>(alpha*(image.at<Vec3b>(y, x)[c]) + beta);

			}

		}
	}
}