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

IplImage* image = 0;


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
			cv::waitKey(250); 

					CvMemStorage* storage = cvCreateMemStorage(0);
					CvSeq* contours = 0;


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
			
				
				if (src.empty())
				{
					return -1;
				}	
			

				
				
					
				

			std::string str = "" + filenames[i];
			char * writable = new char[str.size() + 1];
			std::copy(str.begin(), str.end(), writable);
			writable[str.size()] = '\0'; 

										 // don't forget to free the string after finished using it
			imwrite(writable, src_gray);
			



			
			delete[] writable;




			char c = cvWaitKey(33);
			if (c == 27) { 
				break;

			}


			
			cvReleaseImage(&image);
			cvReleaseImage(&hsv);
			cvReleaseImage(&h_plane);
			cvReleaseImage(&s_plane);
			cvReleaseImage(&v_plane);



			
			cvDestroyAllWindows();


		}

		
	}

	

