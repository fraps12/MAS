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

using namespace cv;
using namespace std;


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

int main(int argc, const char** argv)
{
	if (argc != 2) {
		cerr << "\nIncorrect number of parameters: " << argc << ", should be 2\n" << endl;
		//help(argv);
		return -1;
	}
	string folder = argv[1];
	string folder_for_calculated = "";


	vector<string> filenames1;


	cout << "Reading in directory " << folder << endl;
	vector<string> filenames;


	int num_files = readFilenames(filenames, folder);
	//int num_files1 = readFilenames(filenames, folder_for_calculated);

	cout << "Number of files = " << num_files << endl;

	//cv::namedWindow("image", 1);
	for (size_t i = 0; i < filenames.size(); ++i)

	{




		cout << folder + filenames[i] << " #" << i << endl;
		cv::Mat src = cv::imread(folder + filenames[i]);

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
		//cout << count_white << "infarction area" << "           " << endl;
		
		ofstream strm("", ios_base::app);
		strm << count_white << "        " << "infarction area" << endl;
		//strm << count_black << "           " << "healthy area" << endl;


		if (!src.data) { //Protect against no file
			cerr << folder + filenames[i] << ", file #" << i << ", is not an image" << endl;
			continue;
		}
		//cv::imshow("image", src);
		//cv::waitKey(250);

	}
}
