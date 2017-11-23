#include "CIFilters.hpp"

namespace cifilters{


void Gray(cv::Mat img){
    
	float media;
	int r, g, b;
	int n = 0, m = 0;
	for (n = 0; n < img.rows; n++) {
		for (m = 0; m < img.cols; m++) {
			b = img.at<cv::Vec3b>(n, m)[0];
			g = img.at<cv::Vec3b>(n, m)[1];
			r = img.at<cv::Vec3b>(n, m)[2];
			media = (b + r + g) / 3;
			img.at<cv::Vec3b>(n, m)[0] = (int)media;
			img.at<cv::Vec3b>(n, m)[1] = (int)media;
			img.at<cv::Vec3b>(n, m)[2] = (int)media;
		}
	}
}



}