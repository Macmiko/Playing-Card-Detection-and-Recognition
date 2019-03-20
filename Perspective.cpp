#include "Perspective.h"

using namespace cv;
using namespace std;

//Get perspective transform
void Perspective::TransformImage(cv::Mat & image)
{
	sortQuads();
	lambda = Mat::zeros(2, 4, CV_32FC1);
	lambda = getPerspectiveTransform(inputQuad, outputQuad);
	warpPerspective(image, image, lambda, image.size());
}


Perspective::Perspective()
{
}


void Perspective::setOutputQuad(const cv::Mat & image)
{
	outputQuad[0] = Point2f(0, 0);
	outputQuad[1] = Point2f(0, float(image.rows - 1));
	outputQuad[2] = Point2f(float(image.cols - 1), 0);
	outputQuad[3] = Point2f(float(image.cols - 1), float(image.rows - 1));
}

 void Perspective::setInputQuad(std::vector<cv::Point2f> points)
{
	if (points.size() < 4)
	{
		cout << "Not enough points" << endl;
		return;
	}
	for (int i = 0; i < 4; i++)
	{
		inputQuad[i] = points.at(i);

	}

}

 //Sort input points
 void Perspective::sortQuads()
 {
	 Point2f sortedQuad[4];
	 float distance;
	 float min = 0;

	 for (int i = 0; i < 4; i++)
	 {
		 min = sqrt(pow((outputQuad[0].x - inputQuad[i].x), 2) + pow((outputQuad[0].y - inputQuad[i].y), 2));
		 int best = 0;
		 for (int j = 0; j < 4; j++)
		 {
			 distance = sqrt(pow((outputQuad[j].x - inputQuad[i].x), 2) + pow((outputQuad[j].y - inputQuad[i].y), 2));

			 if (distance < min)
			 {
				 min = distance;
				 best = j;
			 }
		 }
		 sortedQuad[best] = inputQuad[i];
	 }
	 for (int i = 0; i < 4; i++)
	 {
		 inputQuad[i] = sortedQuad[i];
	 }
 }


Perspective::~Perspective()
{
}
