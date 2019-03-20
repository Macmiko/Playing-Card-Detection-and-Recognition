#pragma once
#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/video/background_segm.hpp"

class CD
{

private:
	std::vector<cv::Mat> detectedCards; 
	cv::Mat card;
	cv::Mat mask; //binary image 
	std::vector<cv::Mat> detectedNumber; //Vector with cards's values
	void threeOfKind(std::vector<int>values);
	cv::Mat secondTask = cv::Mat::zeros(200, 400, CV_8UC3); //Second task: "Check if is a trey"
	int MaxArea(std::vector<std::vector<cv::Point>> contours); //Find max. area from all contours



public:
	CD(const cv::Mat image);
	~CD();
	void detectCards(cv::Mat image);

};

