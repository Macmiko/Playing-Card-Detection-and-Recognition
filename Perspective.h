#pragma once
#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/objdetect.hpp"
#include <iostream>
#include <string>
#include <cmath>



class Perspective
{
private:
	cv::Point2f inputQuad[4];
	cv::Point2f outputQuad[4];
	cv::Mat lambda;
public:
	
	void TransformImage(cv::Mat &image);
	void setOutputQuad(const cv::Mat & image);
	void setInputQuad(std::vector<cv::Point2f> points);
	Perspective();
	~Perspective();
private:
	
	void sortQuads();

};


