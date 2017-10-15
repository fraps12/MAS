#include <opencv2/opencv.hpp>
#include <windows.h> // for windows systems
//#include <E:\Users\frapsdev\Documents\Visual Studio 2017\Projects\MAS\MAS\dirent\dirent.h> // for linux systems
#include <sys/stat.h> // for linux systems
#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc_c.h" 
#include "opencv2/core/core_c.h"      
#include "opencv2/highgui/highgui_c.h"
#include <algorithm>
using namespace cv;
using namespace std;
//
// конвертирует изображение в HSV
// и позволяет подобрать параметры
// Hmin, Hmax, Smin, Smax, Vmin, Vmax
// для выделения нужного объекта
//
// robocraft.ru
//

IplImage* image = 0;
IplImage* dst = 0;

IplImage *sobelX;
IplImage *sobelY;
IplImage *tempX;
IplImage *tempY;

// для хранения каналов HSV
IplImage* hsv = 0;
IplImage* h_plane = 0;
IplImage* s_plane = 0;
IplImage* v_plane = 0;
// для хранения каналов HSV после преобразования
IplImage* h_range = 0;
IplImage* s_range = 0;
IplImage* v_range = 0;
// для хранения суммарной картинки
IplImage* hsv_and = 0;

int Hmin = 0;
int Hmax = 256;

int Smin = 0;
int Smax = 256;

int Vmin = 0;
int Vmax = 256;

int HSVmax = 256;

//
// функции-обработчики ползунков
//
void myTrackbarHmin(int pos) {
	Hmin = pos;
	cvInRangeS(h_plane, cvScalar(Hmin), cvScalar(Hmax), h_range);
}

void myTrackbarHmax(int pos) {
	Hmax = pos;
	cvInRangeS(h_plane, cvScalar(Hmin), cvScalar(Hmax), h_range);
}

void myTrackbarSmin(int pos) {
	Smin = pos;
	cvInRangeS(s_plane, cvScalar(Smin), cvScalar(Smax), s_range);
}

void myTrackbarSmax(int pos) {
	Smax = pos;
	cvInRangeS(s_plane, cvScalar(Smin), cvScalar(Smax), s_range);
}

void myTrackbarVmin(int pos) {
	Vmin = pos;
	cvInRangeS(v_plane, cvScalar(Vmin), cvScalar(Vmax), v_range);
}

void myTrackbarVmax(int pos) {
	Vmax = pos;
	cvInRangeS(v_plane, cvScalar(Vmin), cvScalar(Vmax), v_range);
}

int main(int argc, char* argv[])
{

	
	// имя картинки задаётся первым параметром
	char* filename1 = "E:\\infarction_foto\\123\\Группа 3\\опыт бактерии 15\\changed\\P1010054.tif";
	char* filename = argc == 2 ? argv[1] : filename1;
	// получаем картинку
	image = cvLoadImage(filename, 1);

	printf("[i] image: %s\n", filename);
	assert(image != 0);

	// создаём картинки
	hsv = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 3);
	h_plane = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 1);
	s_plane = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 1);
	v_plane = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 1);
	h_range = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 1);
	s_range = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 1);
	v_range = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 1);
	hsv_and = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 1);
	//  конвертируем в HSV 
	cvCvtColor(image, hsv, CV_BGR2HSV);
	// разбиваем на отельные каналы
	cvSplit(hsv, h_plane, s_plane, v_plane, 0);

	//
	// определяем минимальное и максимальное значение
	// у каналов HSV
	double framemin = 0;
	double framemax = 0;

	cvMinMaxLoc(h_plane, &framemin, &framemax);
	printf("[H] %f x %f\n", framemin, framemax);
	Hmin = framemin;
	Hmax = framemax;
	cvMinMaxLoc(s_plane, &framemin, &framemax);
	printf("[S] %f x %f\n", framemin, framemax);
	Smin = framemin;
	Smax = framemax;
	cvMinMaxLoc(v_plane, &framemin, &framemax);
	printf("[V] %f x %f\n", framemin, framemax);
	Vmin = framemin;
	Vmax = framemax;

	// окна для отображения картинки
	//cvNamedWindow("original", CV_WINDOW_AUTOSIZE);
	//cvNamedWindow("H", CV_WINDOW_AUTOSIZE);
	//cvNamedWindow("S", CV_WINDOW_AUTOSIZE);
	//cvNamedWindow("V", CV_WINDOW_AUTOSIZE);
	//cvNamedWindow("H range", CV_WINDOW_AUTOSIZE);
	//cvNamedWindow("S range", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("V range", CV_WINDOW_AUTOSIZE);
	//cvNamedWindow("hsv and", CV_WINDOW_AUTOSIZE);

	cvCreateTrackbar("Hmin", "H range", &Hmin, HSVmax, myTrackbarHmin);
	cvCreateTrackbar("Hmax", "H range", &Hmax, HSVmax, myTrackbarHmax);
	cvCreateTrackbar("Smin", "S range", &Smin, HSVmax, myTrackbarSmin);
	cvCreateTrackbar("Smax", "S range", &Smax, HSVmax, myTrackbarSmax);
	cvCreateTrackbar("Vmin", "V range", &Vmin, HSVmax, myTrackbarVmin);
	cvCreateTrackbar("Vmax", "V range", &Vmax, HSVmax, myTrackbarVmax);
	cvSetTrackbarPos("Vmax", "V range", 254);
	cvSetTrackbarPos("Vmin", "V range", 118);
	//
	// разместим окна по рабочему столу
	//
	if (image->width <1920 / 4 && image->height<1080 / 2) {
		cvMoveWindow("original", 0, 0);
		cvMoveWindow("H", image->width + 10, 0);
	cvMoveWindow("S", (image->width + 10) * 2, 0);
		cvMoveWindow("V", (image->width + 10) * 3, 0);
		cvMoveWindow("hsv and", 0, image->height + 30);
		cvMoveWindow("H range", image->width + 10, image->height + 30);
		cvMoveWindow("S range", (image->width + 10) * 2, image->height + 30);
		cvMoveWindow("V range", (image->width + 10) * 3, image->height + 30);
	}

	while (true) {

		// показываем картинку
		//cvShowImage("original", image);

		//cvShowImage("H", h_plane);
		//cvShowImage("S", s_plane);
		//cvShowImage("V", v_plane);

		//cvShowImage("H range", h_range);
		//cvShowImage("S range", s_range);
		cvShowImage("V range", v_range);

		// складываем 
		cvAnd(h_range, s_range, hsv_and);
		cvAnd(hsv_and, v_range, hsv_and);

		


		//cvShowImage("hsv and", hsv_and);
		char* str = "F:\\saved_1\\1.tif";


		/*sobelX = cvCreateImage(cvSize(hsv_and->width, hsv_and->height), 8, 1);
		sobelY = cvCreateImage(cvSize(hsv_and->width, hsv_and->height), 8, 1);

		// Create Temp Images
		tempX = cvCreateImage(cvSize(hsv_and->width, hsv_and->height), IPL_DEPTH_16S, 1);
		tempY = cvCreateImage(cvSize(hsv_and->width, hsv_and->height), IPL_DEPTH_16S, 1);

		cvSobel(hsv_and, tempX, 1, 0, 3);
		cvSobel(hsv_and, tempY, 0, 1, 3);
		
		// Convert to the Absolute Value
		cvConvertScaleAbs(tempX, sobelX, 1, 0);
		cvConvertScaleAbs(tempY, sobelY, 1, 0);
		addWeighted(tempX, 0.5, tempY, 0.5, 0, grad);
		*/

		
									 // don't forget to free the string after finished using it
		
		


		//cvSaveImage(writable, s_plane);
		

		
		
	

		char c = cvWaitKey(33);
		if (c == 27) { // если нажата ESC - выходим
			//cvSaveImage("F:\\saved\\", hsv_and);
			break;
		}

	}
	printf("\n[i] Results:\n");
	printf("[H] %d x %d\n", Hmin, Hmax);
	printf("[S] %d x %d\n", Smin, Smax);
	printf("[V] %d x %d\n", Vmin, Vmax);
	int edgeThresh = 1;
	int lowThreshold;
	int const max_lowThreshold = 100;
	int ratio = 3;
	int kernel_size = 3;
	char* window_name = "Edge Map";
	Mat dst;
	Mat detected_edges;

	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* contours = 0;
	
	

	// освобождаем ресурсы
	cvReleaseImage(&image);
	cvReleaseImage(&hsv);
	cvReleaseImage(&h_plane);
	cvReleaseImage(&s_plane);
	cvReleaseImage(&v_plane);
	cvReleaseImage(&h_range);
	cvReleaseImage(&s_range);
	cvReleaseImage(&v_range);
	cvReleaseImage(&hsv_and);
	// удаляем окна
	cvDestroyAllWindows();
	return 0;
}