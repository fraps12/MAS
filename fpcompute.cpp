#include <opencv2/opencv.hpp>
#include <string>
#include <filesystem>
#include <vector>

using namespace cv;
using namespace std;

double mean_of(const std::string& pathval) {
    Mat image = imread(pathval);    
    cvtColor(image, image, COLOR_BGR2YCrCb); 
    Mat channel[3];     
    split(image, channel);       
    int histogram[256];   
    Scalar meanK = mean(channel[0]);    
    int size_arr = sizeof(histogram) / sizeof(histogram[0]);    
    return meanK[0];
}

int hist_correlation(double scalar_val, int var_total_work_area) {
   
    double scaled_meanK = scalar_val * 0.855;//scaled MeanK to achieve offset values of threshold
    double const_total_work_area = 3584.0 * 2746.0;
    double calculated_work_area = double(var_total_work_area) / const_total_work_area;      
    int v_min = ((scalar_val / (11.93 * double((var_total_work_area / 1859908.0)))) * 1.085) * 58.0;    
    return v_min;
}

double calc_total_area(Mat img1) {
    int count_white = 0;    
    int countWhite = countNonZero(img1);   
    cout << endl << countWhite;
    return countWhite;  
}

Mat applying_thresh(const string& pathval,  const string& path_to_file,
                    const string& filename, int v_min) {
           
    int Vmax = 240;

    Mat src, dst_gray, dst_final;
    Mat v_plane;
    Mat v_range;
    Mat test;

    src = imread(pathval, 1);
    cvtColor(src, dst_gray, COLOR_BGR2GRAY, CV_8U);
    inRange(dst_gray, Scalar(v_min), Scalar(Vmax), dst_final);
   
    string str;    
    string generate_filename = path_to_file + "//c//" + filename;    
    return dst_final;    
}

int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "Russian");	
    const char* path_to_files = ""; // Path where entries starts from
	const char* path_to_files_arg = argc == 2 ? argv[1] : path_to_files;
    ofstream myfile;
    myfile.open("C://csv//test.csv", std::ofstream::app);
    myfile << "Общая площадь" << ';' << ';' << "Инфаркт" << ';' << ';' << "Процент\n";
    string without_filename1 = " ";
	for (const auto& entry : filesystem::recursive_directory_iterator(path_to_files_arg))
	{
        
        if (filesystem::is_regular_file(entry)) {
                        
            string without_filename = filesystem::path(entry).remove_filename().string();           
            string path = entry.path().string();//Full path to file {ex: C:/example/exmpl.txt}     
            string file = filesystem::path(path).filename().string();//only filename {ex: exmpl.txt}
            string relative_path = filesystem::path(path).relative_path().string();
            string parent_path = filesystem::path(relative_path).parent_path().string();

            if (parent_path != without_filename1) {
                myfile << "|||||||||||||||||||||||||||||||||||||||||||||||||" << ';' << "|||||||||||||||||||||||||||||||||||||||||||||||||" << '\n';
            }
            without_filename1 = parent_path;

            double total_area = (calc_total_area(applying_thresh(path, without_filename, file, 2)))/ 32.036;
            double calc_area = (calc_total_area(applying_thresh(path, without_filename, file,
                               hist_correlation(mean_of(path),
                               calc_total_area(applying_thresh(path, without_filename, file, 2))))))/ 32.036;
            double percentage = ((calc_area * 100.0) / total_area);

            cout << "filepath  " << entry.path().string() << endl;            
            myfile << '\n';            
            myfile << parent_path << "\n";                      
            myfile << total_area << ';' << ';';
            myfile << calc_area << ';' << ';' << percentage << '\n';
        }        
	}    
    myfile.close();	
}



