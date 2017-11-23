#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

namespace cifilters{


    void Gray(cv::Mat img);
    void Negative(cv::Mat img);


    void ApplyKernel(cv::Mat& image, cv::Mat kernel);
}