#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

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

inline void ApplyKernel(cv::Mat& image, cv::Mat kernel)
{
	using namespace cv;

	int raio = kernel.rows / 2;

	Mat image_final(image.size(), image.type());

	Mat image_border;
	copyMakeBorder(image, image_border, raio, raio, raio, raio, cv::BORDER_REFLECT101);

	Mat image_region;

	int pixel;

	Mat mask = Mat(kernel.rows, kernel.cols, CV_32FC3);

	std::vector<Mat> vChannels;
	for (int k = 0; k < image.channels(); k++)
	{
		vChannels.push_back(kernel);
	}
	merge(vChannels, mask);

	for (int y = raio; y < image_border.rows - raio; y++)
	{
		for (int x = raio; x < image_border.cols - raio; x++)
		{
			for (int c = 0; c < image.channels(); c++)
			{
				Rect region(x - raio, y - raio, kernel.rows, kernel.cols);
				image_region = image_border(region);

				image_region.convertTo(image_region, CV_32FC3);

				image_region = image_region.mul(mask);

				pixel = sum(image_region)[c];

				if (pixel > 255)	pixel = 255;
				if (pixel < 0)		pixel = 0;

				image_final.at<Vec3b>(y - raio, x - raio)[c] = pixel;
			}
		}
	}

	image = image_final;
}

}