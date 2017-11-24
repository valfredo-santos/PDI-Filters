#include "CIFilters.hpp"
#include <iostream>
#include <string>

int main(int argc, char** argv){
    using namespace cv;
    using namespace std;

    if(argc != 2){
        cout << "Uso: filters image_path" << endl;
        return -1;
    }

    Mat image;
    image = imread(argv[1], CV_LOAD_IMAGE_COLOR);

    if(!image.data){
        cout << "Não foi possível abrir imagem" << endl;
        return -1;
    }

    Mat sharpen = (Mat_<float>(3, 3) <<
    0, -1, 0,
    -1, 5, -1,
    0, -1, 0);

    Mat emboss = (Mat_<float>(3, 3) <<
    -2, -1, 0,
    -1, 1, 1,
    0,  1, 2);

    cv::Mat image2 = image.clone();

    cifilters::GrayCopy(image,image2);
    //cifilters::Negative(image);
    //cifilters::ApplyKernel(image, sharpen);
    //cifilters::ApplyKernel(image, emboss);
    //cifilters::multiBrighten(image, 1.3);
    //cifilters::ApplyKernel(image, sharpen);


    namedWindow("Display Window", WINDOW_AUTOSIZE);         //cria janela
    imshow("Display Window", image);                        //mostra imagem na janela

    namedWindow("Display Window2", WINDOW_AUTOSIZE);         //cria janela
    imshow("Display Window2", image2);                        //mostra imagem na janela

    waitKey(0);                                             //espera tecla
    return 0;
}