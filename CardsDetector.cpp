#include "Perspective.h"
#include "CardsDetector.h"
#include "DIP.h"
#include "SANDetector.h"


using namespace std;
using namespace cv;


string IntToString(int index)
{
	stringstream ss;
	ss << index;
	string stringIndex = ss.str();

	return stringIndex;
}

CD::CD(const Mat image)
{
	//mask=binary image after filtration
	mask = DIP::createMask(image);
	detectCards(image);
}



void CD::detectCards(Mat image)
{
	//create constructors
	Perspective perspective;
	SymbolAndNumberDetector SAND;

	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;

	// find contours on the mask and then create a vector which holds the separated contours

	cv::findContours(mask, contours, hierarchy, 0, 2, cv::Point(0, 0));
	std::vector<std::vector<cv::Point> > contoursPoly(contours.size());
	std::vector<cv::Point> corners;
	std::vector<cv::Rect> cardsRects;


	for (int i = 0; i < contours.size(); i++)
	{
		//get only cards (area bigger then 100000)
		if (contourArea(contours[i]) > 100000) 
		{

			cv::approxPolyDP(cv::Mat(contours[i]), contoursPoly[i], 3, true); //close contours

			cv::Rect appRect = cv::boundingRect(cv::Mat(contoursPoly[i])); // take ROI on detected card

			//resize ROI with card
			Rect returnRect = Rect(appRect.x - 5, 
				appRect.y - 5,
				appRect.width + 10,
				appRect.height + 10);

			//check if new ROI isn't bigger than image
			if (returnRect.x < 0)returnRect.x = 0;
			if (returnRect.y < 0)returnRect.y = 0;
			if (returnRect.x + returnRect.width >= mask.cols)returnRect.width = mask.cols - returnRect.x;
			if (returnRect.y + returnRect.height >= mask.rows)returnRect.height = mask.rows - returnRect.y;

			cardsRects.push_back(returnRect);
		}
	}



	for (int i = 0; i < cardsRects.size(); i++)
	{

		cv::Mat detectedCard = cv::Mat(image, cardsRects[i]);

		//check if the card is tilted
		if (detectedCard.cols > detectedCard.rows)
		{
			
		
			Mat rotated_image = detectedCard.t();    //new transformed image
			rotated_image.copyTo(detectedCard);
			cv::flip(detectedCard, detectedCard, 1);

		}

		cv::resize(detectedCard, detectedCard, cv::Size(200, 300), 0, 0, cv::INTER_AREA);

		detectedCards.push_back(detectedCard);	

	}
	waitKey(0);


	///PERSPECTIVE
	for (int i = 0; i < detectedCards.size(); i++)
	{
		std::vector<std::vector<cv::Point> > cardContours;
		std::vector<cv::Vec4i> hierarchy1;

		Mat grayCard, binary;
		cvtColor(detectedCards[i], grayCard, COLOR_BGR2GRAY);
		threshold(grayCard, binary, 120, 255, THRESH_OTSU);

		// find contours on the mask and then create a vector which holds the separated contours
		cv::findContours(binary, cardContours, hierarchy1, 0, 2, cv::Point(0, 0));
		std::vector<std::vector<cv::Point> > cardContoursPoly(cardContours.size());

		//get the biggest contour (card)
		int maxAreaContourId = MaxArea(cardContours);

		cv::approxPolyDP(cv::Mat(cardContours[maxAreaContourId]), cardContoursPoly[0], 20, true);

		std::vector<cv::Point2f> corners;

		//Get corners
		if (cardContoursPoly[0].size() == 4)
			for (int jj = 0; jj < 4; jj++)
				corners.push_back(cardContoursPoly[0][jj]);
				

		else
			cout << "Something is wrong with corners" << endl;

		//Take perspective transformation
		perspective.setInputQuad(corners);
		perspective.setOutputQuad(detectedCards[i]);
		perspective.TransformImage(detectedCards[i]);

	
		
	}
	waitKey(0);

	//Detect number and symbol of each card
	for (int i = 0; i < detectedCards.size(); i++)
	{
		SAND.findNumberAndSymbol(detectedCards[i], i);
	}

	//Check if is a trey
	threeOfKind(SAND.cardValue);

	waitKey(0);
}

void CD::threeOfKind(vector<int>values)
{
	sort(values.begin(), values.end());
	bool Three=false;
	for (int i = 0; i < values.size()-2; i++)
	{
		if (values[i + 1] == values[i]) //Check if next value is the same
			if (values[i + 2] == values[i]) //If yes, check also next one
				Three = true;

	}

	if (Three)
		cv::putText(secondTask, "There is a trey", cv::Point(50, 100), CV_FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 1, 8, false);
	else
		cv::putText(secondTask, "There isn't any trey", cv::Point(50, 100), CV_FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 1, 8, false);
	
	imshow("Second Task", secondTask);

}

int CD::MaxArea(std::vector<std::vector<cv::Point> > contours)
{

	double maxArea = 0;
	int maxAreaContourId = -1;
	for (int j = 0; j < contours.size(); j++)
	{
		double newArea = contourArea(contours[j]);
		if (newArea > maxArea) {
			maxArea = newArea;
			maxAreaContourId = j;
		}
	}
	return maxAreaContourId;
}


CD::~CD()
{
	destroyAllWindows();
}
