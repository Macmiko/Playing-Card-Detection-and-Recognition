

#include"Perspective.h"
#include"CardsDetector.h"


using namespace std;
using namespace cv;

int main()
{
	Mat image;
	string img_path = "C://Users/Mikolaj/Desktop/PW/7 semestr/WM/Projekt 2/Mikolaj_Ciszek/5.png"; //get file path
	image = imread(img_path);
	CD test(image);

	

	return 0;
}