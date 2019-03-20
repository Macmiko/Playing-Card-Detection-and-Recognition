#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

class DIP
{
private:
public:

	static cv::Mat createMask(const cv::Mat mask);
	static void betterContrast(cv::Mat image, double alpha, int beta);
};

