#include "funcao.h"

using namespace cv;
using namespace std;

int main()
{
	Mat	im0 = imread("../data/lena.jpg", IMREAD_GRAYSCALE);
	Mat im2 = imread("../data/lena.jpg", IMREAD_GRAYSCALE);
	Mat im1 = imread("../data/lena.jpg", IMREAD_GRAYSCALE);
	/*FAÇA AS CHAMADAS DE FUNÇÃO Q VC QUISER*/	

	Mat im;
	cv::resize(im1, im, cv::Size(im1.cols, im1.rows / 2), INTER_AREA);	 //operacao do opencv, reduz a "altura" da imagem que será usada na metade
	
	string file3 = "../data/output3.txt";
	writeMatToFile4(im, file3.c_str());
	string file2 = "../data/output2.txt";
	writeMatToFile3(im, file2.c_str());
	string file1 = "../data/output1.txt";
	writeMatToFile(im, file1.c_str());
    string file = "../data/output.txt";
	writeMatToFile2(im, file.c_str());

	waitKey(0);
	return 0;
}