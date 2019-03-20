#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

class SymbolAndNumberDetector
{
public:
	std::vector<int> cardValue;
private:

	cv::Mat card;
	cv::Mat kernel = (cv::Mat_<double>(3, 3) << -1, -1, -1, -1, 9, -1, -1, -1, -1); //create kernel
	int redChannel, blueChannel, greenChannel;
	bool color; //0=black, 1=red
	cv::Mat graySymbol, binarySymbol;
	std::string cardNumber;
	std::string NumberOfCard;


public:
	SymbolAndNumberDetector();
	~SymbolAndNumberDetector();
	std::string Int_to_string(int index); //change int into string

	void findNumberAndSymbol(cv::Mat detectedCard, int whichCard);
private:

	void SymbolClassifier(cv::Mat symbol,cv::Mat card, std::string cardNumber);
	double compareImages(const cv::Mat &firstImage, const cv::Mat &secondImage);

};

