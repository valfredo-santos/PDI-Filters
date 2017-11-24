#ifndef CIFILTERS_HPP
#define CIFILTERS_HPP

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>

namespace cifilters{

inline void Gray(cv::Mat img){
    
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

inline void GrayCopy(const cv::Mat img, cv::Mat copy){
	
	img.copyTo(copy);

	float media;
	int r, g, b;
	int n = 0, m = 0;
	for (n = 0; n < copy.rows; n++) {
		for (m = 0; m < copy.cols; m++) {
			b = copy.at<cv::Vec3b>(n, m)[0];
			g = copy.at<cv::Vec3b>(n, m)[1];
			r = copy.at<cv::Vec3b>(n, m)[2];
			media = (b + r + g) / 3;
			copy.at<cv::Vec3b>(n, m)[0] = (int)media;
			copy.at<cv::Vec3b>(n, m)[1] = (int)media;
			copy.at<cv::Vec3b>(n, m)[2] = (int)media;
		}
	}

}

inline void Negative(cv::Mat img){
	/*segue a formula negativo = 255 - valor do pixel*/
	for (int n = 0; n < img.rows; n++) {
		for (int m = 0; m < img.cols; m++) {
			img.at<cv::Vec3b>(n, m)[0] = 255 - img.at<cv::Vec3b>(n, m)[0];
			img.at<cv::Vec3b>(n, m)[1] = 255 - img.at<cv::Vec3b>(n, m)[1];
			img.at<cv::Vec3b>(n, m)[2] = 255 - img.at<cv::Vec3b>(n, m)[2];
		}
	}
}

inline void AdditiveBrighten(cv::Mat & image, const int beta){
	using namespace cv;	
	for (int y = 0; y < image.rows; y++){
		for (int x = 0; x < image.cols; x++){
			for (int channel = 0; channel < image.channels(); channel++){
				image.at<Vec3b>(y, x)[channel] = saturate_cast<uchar>(image.at<Vec3b>(y, x)[channel] + beta);
			}
		}
	}
}

inline void multiBrighten(cv::Mat & image, const float c){
	using namespace cv;
	if (c < 0){
		std::cout << "Multiplicative Brighten not applied: The c parameter must be non negative" << std::endl;		
	}else{
		for (int y = 0; y < image.rows; y++){
			for (int x = 0; x < image.cols; x++){
				for (int channel = 0; channel < image.channels(); channel++){
					image.at<Vec3b>(y, x)[channel] = saturate_cast<uchar>(image.at<Vec3b>(y, x)[channel] * c);
				}
			}
		}
	}
}

inline void ApplyKernel(cv::Mat & image, cv::Mat kernel){
	using namespace cv;

	int raio = kernel.rows / 2;

	Mat imageFinal(image.size(), image.type());

	Mat imageBorder;
	copyMakeBorder(image, imageBorder, raio, raio, raio, raio, cv::BORDER_REFLECT101);

	Mat imageRegion;

	int pixel;

	Mat mask = Mat(kernel.rows, kernel.cols, CV_32FC3);

	std::vector<Mat> vChannels;
	for (int k = 0; k < image.channels(); k++)
	{
		vChannels.push_back(kernel);
	}
	merge(vChannels, mask);

	for (int y = raio; y < imageBorder.rows - raio; y++){
		for (int x = raio; x < imageBorder.cols - raio; x++){
			for (int c = 0; c < image.channels(); c++){
				Rect region(x - raio, y - raio, kernel.rows, kernel.cols);
				imageRegion = imageBorder(region);

				imageRegion.convertTo(imageRegion, CV_32FC3);

				imageRegion = imageRegion.mul(mask);

				pixel = sum(imageRegion)[c];

				if (pixel > 255)	pixel = 255;
				if (pixel < 0)		pixel = 0;

				imageFinal.at<Vec3b>(y - raio, x - raio)[c] = pixel;
			}
		}
	}

	image = imageFinal;
}

}

#endif /*CIFILTERS_HPP*/