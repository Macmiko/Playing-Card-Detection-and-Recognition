#include "SANDetector.h"

using namespace cv;
using namespace std;


SymbolAndNumberDetector::SymbolAndNumberDetector()
{
}

void SymbolAndNumberDetector::findNumberAndSymbol(Mat detectedCard, int whichCard)
{
	
	detectedCard.copyTo(card);

	filter2D(card, card, -1, kernel); //high-pass filter

	Rect RecWithSymbols(20, 55, 150, 200); //get rect with symbols

	Mat Symbols = card(RecWithSymbols);

	rectangle(card, RecWithSymbols, Scalar(255), 1, 8, 0);


	Mat gray_card(Symbols.size(), CV_8UC1), tresh(Symbols.size(), CV_8UC1);
	cvtColor(Symbols, gray_card, COLOR_BGR2GRAY);
	threshold(gray_card, tresh, 120, 255, THRESH_OTSU);
	

	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;

	// find contours on the mask and then create a vector which holds the separated contours

	findContours(tresh, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));
	std::vector<std::vector<cv::Point> > contoursPoly(contours.size()); 
	std::vector<cv::Rect> numberRects; 

	for (int j = 0; j < contours.size(); j++) 
	{
		//Get only symbols
		if (contourArea(contours[j]) > 250 && contourArea(contours[j]) < 1000)

		{

			cv::approxPolyDP(cv::Mat(contours[j]), contoursPoly[j], 3, true);
			cv::Rect appRect = cv::boundingRect(cv::Mat(contoursPoly[j]));

			numberRects.push_back(appRect);

		}
		

	}

	cardValue.push_back(numberRects.size());
	cardNumber=Int_to_string(numberRects.size());

	cv::Mat symbol = cv::Mat(Symbols, numberRects[numberRects.size() - 1]);

	cv::resize(symbol, symbol, cv::Size(40, 60), 0, 0, cv::INTER_AREA);

	SymbolClassifier(symbol, detectedCard, cardNumber);
	NumberOfCard = Int_to_string(whichCard);

	string title4 = "Card" + NumberOfCard;
	imshow(title4, detectedCard);

}

void SymbolAndNumberDetector::SymbolClassifier(Mat symbol, Mat card, string cardNumber)
{
	//classif symbols and color

	redChannel = (int)symbol.at<Vec3b>(symbol.rows / 2, symbol.cols / 2)[2];
	greenChannel = (int)symbol.at<Vec3b>(symbol.rows / 2, symbol.cols / 2)[1];
	blueChannel = (int)symbol.at<Vec3b>(symbol.rows / 2, symbol.cols / 2)[0];

	//Check card's color
	if (redChannel > greenChannel && redChannel > blueChannel && redChannel > 100)
		color = true; //Card is red
	
	else
		color = false; //Card is black
	
	cvtColor(symbol, graySymbol, COLOR_BGR2GRAY);
	threshold(graySymbol, binarySymbol, 120, 255, THRESH_OTSU);


	int SymbolNumber = 1;
	double result=0;
	double min = 10;
	int index = 0;

	//for all four samples
	for (int s = 0; s < 4; s++)
	{
		
		//Get sample
		string path = "C://Users/Mikolaj/Desktop/PW/7 semestr/WM/Projekt 2/Symbols/symbol" + Int_to_string(SymbolNumber) + ".png"; //path to samples
		Mat dataset = imread(path, 0);

		result = compareImages(binarySymbol, dataset);
		
		//Check if spades or clubs
		if (color == 0)
		{
			if (SymbolNumber == 1 && result < min)
			{
				min = result;
				index = SymbolNumber;
			}

			else if (SymbolNumber == 3 && result < min)
			{
				min = result;
				index = SymbolNumber;
			}

		}
		//Check if diamonds or heards
		else
		{
			if (SymbolNumber == 2 && result < min)
			{
				min = result;
				index = SymbolNumber;
			}

			else if (SymbolNumber == 4 && result < min)
			{
				min = result;
				index = SymbolNumber;
			}

		}

		SymbolNumber++;
	}


	//show results
	if (index == 1)
		putText(card, cardNumber + " CLUBS", Point2f(10, 280),
			FONT_HERSHEY_COMPLEX_SMALL, 1, cvScalar(0, 0, 0), 2, CV_AA);
	else if (index == 2)
		putText(card, cardNumber + " DIAMONDS", Point2f(10, 280),
			FONT_HERSHEY_COMPLEX_SMALL, 1, cvScalar(0, 0, 255), 2, CV_AA);
	else if (index == 3)
		putText(card, cardNumber + " SPADES", Point2f(10, 280),
			FONT_HERSHEY_COMPLEX_SMALL, 1, cvScalar(0, 0, 0), 2, CV_AA);
	else if (index == 4)
		putText(card, cardNumber + " HEARTS", Point2f(10, 280),
			FONT_HERSHEY_COMPLEX_SMALL, 1, cvScalar(0, 0, 255), 2, CV_AA);
}





double SymbolAndNumberDetector::compareImages(const Mat &firstImage, const Mat &secondImage)
{
	int histSize = 256;
	float range[] = { 0, 256 }; //the upper boundary is exclusive
	const float* histRange = { range };
	bool uniform = true, accumulate = false;
	Mat firstHist, secondHist;

	int hist_w = 512, hist_h = 400; 
	int bin_w = cvRound((double)hist_w / histSize);
	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));


	//Calculate histograms
	calcHist(&firstImage, 1, 0, Mat(), firstHist, 1, &histSize, &histRange, uniform, accumulate);
	//normalize(firstHist, firstHist, 0, 1, NORM_MINMAX, -1, Mat());
	
	calcHist(&secondImage, 1, 0, Mat(), secondHist, 1, &histSize, &histRange, uniform, accumulate);
	//normalize(secondHist, secondHist, 0, 1, NORM_MINMAX, -1, Mat());

	double result = compareHist(firstHist, secondHist, 3);

	return result;
}

string SymbolAndNumberDetector::Int_to_string(int index)
{
	stringstream ss;
	ss << index;
	string stringIndex = ss.str();

	return stringIndex;
}


SymbolAndNumberDetector::~SymbolAndNumberDetector()
{

}