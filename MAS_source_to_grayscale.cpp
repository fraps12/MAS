#include <opencv2/opencv.hpp>
#include <windows.h> // for windows systems
#include <E:\recoverForCover\Projects\MAS\MAS\dirent\dirent.h> // for linux systems
#include <sys/stat.h> // for linux systems
#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc_c.h" 
#include "opencv2/core/core_c.h"      
#include "opencv2/highgui/highgui_c.h"
#include <algorithm>
//#include "transientareassegmentationmodule.hpp"



using namespace std;
using namespace cv;

Mat erosion_dst, dilation_dst;

int erosion_elem = 0;
int erosion_size = 0;
int dilation_elem = 0;
int dilation_size = 0;
int const max_elem = 2;
int const max_kernel_size = 21;

IplImage* image1 = 0;
IplImage* gray = 0;
IplImage* bin = 0;
IplImage* dst1 = 0;
//Ñâåðõó êîíòóðû
IplImage* image = 0;

// äëÿ õðàíåíèÿ êàíàëîâ HSV
IplImage* hsv = 0;
IplImage* h_plane = 0;
IplImage* s_plane = 0;
IplImage* v_plane = 0;

int readFilenames(std::vector<string> &filenames, const string &directory)
{
#ifdef WINDOWS
	HANDLE dir;
	WIN32_FIND_DATA file_data;

	if ((dir = FindFirstFile((directory + "/*").c_str(), &file_data)) == INVALID_HANDLE_VALUE)
		return; /* No files found */

	do {
		const string file_name = file_data.cFileName;
		const string full_file_name = directory + "/" + file_name;
		const bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

		if (file_name[0] == '.')
			continue;

		if (is_directory)
			continue;

		filenames.push_back(full_file_name);
	} while (FindNextFile(dir, &file_data));

	FindClose(dir);
#else
	DIR *dir;
	class dirent *ent;
	class stat st;

	dir = opendir(directory.c_str());
	while ((ent = readdir(dir)) != NULL) {
		const string file_name = ent->d_name;
		const string full_file_name = directory + "/" + file_name;

		if (file_name[0] == '.')
			continue;

		if (stat(full_file_name.c_str(), &st) == -1)
			continue;

		const bool is_directory = (st.st_mode & S_IFDIR) != 0;

		if (is_directory)
			continue;

		    //filenames.push_back(full_file_name); // returns full path
		filenames.push_back(file_name); // returns just filename
	}
	closedir(dir);
#endif
	std::sort(filenames.begin(), filenames.end()); //optional, sort the filenames
	return(filenames.size()); //Return how many we found
} // GetFilesInDirectory



void help(const char **argv) {
	cout << "\n\n"
		<< "Call:\n" << argv[0] << " <directory path>\n\n"
		<< "Given a directory of images, create a vector of\n"
		<< "their names, read and display them. Filter out\n"
		<< "non-images\n"
		<< endl;
}







int main(int argc, const char** argv)
{
	if (argc != 2) {
		cerr << "\nIncorrect number of parameters: " << argc << ", should be 2\n" << endl;
		help(argv);
		return -1;
	}
	string folder = argv[1];
	string folder_for_calculated = "E:\\infarction_foto\3\\changed_cut\\";


	vector<string> filenames1;
	

		cout << "Reading in directory " << folder << endl;
		vector<string> filenames;


		int num_files = readFilenames(filenames, folder);
		//int num_files1 = readFilenames(filenames, folder_for_calculated);

		cout << "Number of files = " << num_files << endl;

		cv::namedWindow("image", 1);
		for (size_t i = 0; i < filenames.size(); ++i)

		{




			cout << folder + filenames[i] << " #" << i << endl;
			cv::Mat src = cv::imread(folder + filenames[i]);
			

			if (!src.data) { //Protect against no file
				cerr << folder + filenames[i] << ", file #" << i << ", is not an image" << endl;
				continue;
			}





			cv::imshow("image", src);
			cv::waitKey(250); //For fun, wait 250ms, or a quarter of a second, but you can put in "0" for no wait or -1 to wait for keypresses
							  /* do whatever you want with your images here */
			/*std::string str1 = folder + filenames[i];
			char * kekable = new char[str1.size() + 1];
			std::copy(str1.begin(), str1.end(), kekable);
			kekable[str1.size()] = '\0'; // don't forget the terminating 0
			

			// don't forget to free the string after finished using it
				/*hsv = cvCreateImage(cvGetSize(&iplDst), IPL_DEPTH_8U, 3);
				h_plane = cvCreateImage(cvGetSize(&iplDst), IPL_DEPTH_8U, 1);
				s_plane = cvCreateImage(cvGetSize(&iplDst), IPL_DEPTH_8U, 1);
				v_plane = cvCreateImage(cvGetSize(&iplDst), IPL_DEPTH_8U, 1);
				delete[] kekable;*/
				









					//cvSplit(hsv, h_plane, s_plane, v_plane, 0);


					/*gray = cvCreateImage(cvGetSize(kekable), IPL_DEPTH_8U, 1);
					bin = cvCreateImage(cvGetSize(kekable), IPL_DEPTH_8U, 1);*/
					// êëîíèðóåì

					// îêíî äëÿ îòîáðàæåíèÿ êàðòèíêè


					// ïðåîáðàçóåì â ãðàäàöèè ñåðîãî
					//cvCvtColor(kekable, gray, CV_RGB2GRAY);

					// ïðåîáðàçóåì â äâîè÷íîå
					//cvInRangeS(gray, cvScalar(40), cvScalar(150), bin);

					CvMemStorage* storage = cvCreateMemStorage(0);
					CvSeq* contours = 0;

					// íàõîäèì êîíòóðû
					/*int contoursCont = cvFindContours(bin, storage, &contours, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));

					// íàðèñóåì êîíòóðû
					for (CvSeq* seq0 = contours; seq0 != 0; seq0 = seq0->h_next) {
						cvDrawContours(dst1, seq0, CV_RGB(255, 216, 0), CV_RGB(0, 0, 250), 0, 1, 8); // ðèñóåì êîíòóð
					}
					*/



					//cv::Mat src = cv::imread(folder + filenames[i]);
					// ñîõðàíÿåì
			Mat fullImageHSV;
			cvtColor(src, fullImageHSV, CV_BGR2GRAY);

			
			Mat src_gray;
				
				Mat merged;
				Mat temp2;
				Mat temp3;
				Mat temp4;
				int edgeThresh = 1;
				int lowThreshold = 50;
				int const max_lowThreshold = 100;
				int ratio = 3;
				int kernel_size = 3;
				
				Mat GrayFrame = Mat(src.rows, src.cols, CV_8UC1);
				Mat Src = Mat(src.rows, src.cols, CV_32FC1);
				Mat Dst = Mat(src.rows, src.cols, CV_32FC1);
				Mat Temp = Mat(src.rows, src.cols, CV_32FC1);
				Mat Filtered = Mat(src.rows, src.cols, CV_32FC1);
			



				
				int erosion_type;
				if (erosion_elem == 0) { erosion_type = MORPH_RECT; }
				else if (erosion_elem == 1) { erosion_type = MORPH_CROSS; }
				else if (erosion_elem == 2) { erosion_type = MORPH_ELLIPSE; }

			


				Mat element5(3, 3, CV_8U, cv::Scalar(1));
				Mat element6(512, 512, CV_8U, cv::Scalar(1));
				
				// Apply the erosion operation
				cvtColor(src, src_gray, COLOR_BGR2GRAY);		
				
				//Mat src, src_gray;
				Mat grad;
				
				int scale = 1;
				int delta = 0;
				int ddepth = CV_16S;


				int morph_size = 2;

				int n = 0;
				const int NIter = 4;

				Mat grad_x, grad_y;
				Mat abs_grad_x, abs_grad_y;
				//morphologyEx(src_gray, src_gray, MORPH_OPEN, element5);
				//morphologyEx(src_gray, src_gray, MORPH_TOPHAT, element6);
				//Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
				//Sobel(src_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
				//Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
				//Sobel(src_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
				//convertScaleAbs(grad_x, abs_grad_x);
				//convertScaleAbs(grad_y, abs_grad_y);
				//addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);
				
				
				/*int count_black = 0;
				int count_white = 0;
				for (int y = 0; y < grad.rows; y++) {
					for (int x = 0; x < grad.cols; x++) {
						if (grad.at<cv::Vec3b>(y, x) == cv::Vec3b(255, 255, 255)) {
							count_white++;
							cout << count_white << endl;
						}
						else if (grad.at<cv::Vec3b>(y, x) == cv::Vec3b(0, 0, 0)) {
							count_black++;
							cout << count_black << endl;
						}
					}
				}*/
				
				
			
				
				//Canny(src_gray, src_gray, lowThreshold, lowThreshold*ratio, kernel_size);
				
			

				
			
				
				if (src.empty())
				{
					return -1;
				}	
			

				
				
					
				

			std::string str = "E:\\infarction_foto\\123\\Ãðóïïà 3\\îïûò áàêòåðèè 15\\changed\\" + filenames[i];
			char * writable = new char[str.size() + 1];
			std::copy(str.begin(), str.end(), writable);
			writable[str.size()] = '\0'; // don't forget the terminating 0

										 // don't forget to free the string after finished using it
			imwrite(writable, src_gray);
			



			//cvSaveImage(writable, s_plane);
			delete[] writable;




			char c = cvWaitKey(33);
			if (c == 27) { // åñëè íàæàòà ESC - âûõîäèì
				break;

			}


			// îñâîáîæäàåì ðåñóðñû
			cvReleaseImage(&image);
			cvReleaseImage(&hsv);
			cvReleaseImage(&h_plane);
			cvReleaseImage(&s_plane);
			cvReleaseImage(&v_plane);



			// óäàëÿåì îêíà
			cvDestroyAllWindows();


		}// ñêîáêà öèêëà èñõîäíûõ ôàéëîâ

		
	}

	

/*int main(int argc, char* argv[])
{
	// èìÿ êàðòèíêè çàäà¸òñÿ ïåðâûì ïàðàìåòðîì
	char* filename = argc == 2 ? argv[1] : "infarction.jpg";
	char* filename1 = argc == 2 ? argv[2] : "infarction.bmp";
	// ïîëó÷àåì êàðòèíêó
	image = cvLoadImage(filename, 1);
	
	//Ïîëó÷àåì èçîáðàæåíèå äëÿ êîíòóðèðîâàíèÿ
	
	image1 = cvLoadImage(filename1, 1);
	// íàõîäèì êîíòóðû
	
	
	

	printf("[i] image: %s\n", filename);
	assert(image != 0);

	// ñîçäà¸ì êàðòèíêè
	hsv = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 3);
	h_plane = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 1);
	s_plane = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 1);
	v_plane = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 1);
	
	//  êîíâåðòèðóåì â HSV 
	cvCvtColor(image, hsv, CV_BGR2HSV);
	// ðàçáèâàåì íà îòåëüíûå êàíàëû
	cvSplit(hsv, h_plane, s_plane, v_plane, 0);

	//
	// îïðåäåëÿåì ìèíèìàëüíîå è ìàêñèìàëüíîå çíà÷åíèå
	// ó êàíàëîâ HSV
	double framemin = 0;
	double framemax = 0;

	

	// îêíà äëÿ îòîáðàæåíèÿ êàðòèíêè
	cvNamedWindow("original", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("H", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("S", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("V", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("Contours", CV_WINDOW_AUTOSIZE);


	

	//
	// ðàçìåñòèì îêíà ïî ðàáî÷åìó ñòîëó
	//
	if (image->width <1920 / 4 && image->height<1080 / 2) {
		cvMoveWindow("original", 0, 0);
		cvMoveWindow("H", image->width + 10, 0);
		cvMoveWindow("S", (image->width + 10) * 2, 0);
		cvMoveWindow("V", (image->width + 10) * 3, 0);
		cvMoveWindow("Contours", (image1->width + 10) * 4, 0);
	}
	


	gray = cvCreateImage(cvGetSize(image1), IPL_DEPTH_8U, 1);
	bin = cvCreateImage(cvGetSize(image1), IPL_DEPTH_8U, 1);
	// êëîíèðóåì
	dst1 = cvCloneImage(image1);
	// îêíî äëÿ îòîáðàæåíèÿ êàðòèíêè
	

	// ïðåîáðàçóåì â ãðàäàöèè ñåðîãî
	cvCvtColor(image1, gray, CV_RGB2GRAY);

	// ïðåîáðàçóåì â äâîè÷íîå
	cvInRangeS(gray, cvScalar(40), cvScalar(150), bin);

	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* contours = 0;

	// íàõîäèì êîíòóðû
	int contoursCont = cvFindContours(bin, storage, &contours, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));

	// íàðèñóåì êîíòóðû
	for (CvSeq* seq0 = contours; seq0 != 0; seq0 = seq0->h_next) {
		cvDrawContours(dst1, seq0, CV_RGB(255, 216, 0), CV_RGB(0, 0, 250), 0, 1, 8); // ðèñóåì êîíòóð
	}

	
	

	while (true) {

		// ïîêàçûâàåì êàðòèíêó
		cvShowImage("original", image);

		cvShowImage("H", h_plane);
		cvShowImage("S", s_plane);
		cvShowImage("V", v_plane);
		cvShowImage("Contours", dst1);
		

		// ñîõðàíÿåì		
		cvSaveImage("2_1.bmp", s_plane);
		

		char c = cvWaitKey(33);
		if (c == 27) { // åñëè íàæàòà ESC - âûõîäèì
			break;
		}
	}
	printf("\n[i] Results:\n");
	printf("[H] %d x %d\n", Hmin, Hmax);
	printf("[S] %d x %d\n", Smin, Smax);
	printf("[V] %d x %d\n", Vmin, Vmax);

	// îñâîáîæäàåì ðåñóðñû
	cvReleaseImage(&image);
	cvReleaseImage(&hsv);
	cvReleaseImage(&h_plane);
	cvReleaseImage(&s_plane);
	cvReleaseImage(&v_plane);
	cvReleaseImage(&h_range);
	cvReleaseImage(&s_range);
	cvReleaseImage(&v_range);
	cvReleaseImage(&hsv_and);
	cvReleaseImage(&image1);

	// óäàëÿåì îêíà
	cvDestroyAllWindows();
	return 0;
}
*/


/*
using namespace cv;
using namespace std;

int main() {


	Mat src;


	string filepath = "C:\\Users\\frapsdev\\Desktop\\1111.tiff";
	src = imread(filepath);

	if (!src.data)
	{
		return -1;
	}
	imshow("kek", src);
	int count_black = 0;
	int count_white = 0;
	for (int y = 0; y < src.rows; y++) {
		for (int x = 0; x < src.cols; x++) {
			if (src.at<cv::Vec3b>(y, x) == cv::Vec3b(255, 255, 255)) {
				count_white++;
				

			}
			else if (src.at<cv::Vec3b>(y, x) == cv::Vec3b(0, 0, 0)) {
				count_black++;
				
			}
		}
	}
	
	cout << count_black << "black" <<  endl;
	cout << count_white << "white" <<  endl;
	
	waitKey(0);
	return 0;


}*/
