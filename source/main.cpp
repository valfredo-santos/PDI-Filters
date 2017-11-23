
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

    cifilters::Negative(image);

    namedWindow("Display Window", WINDOW_AUTOSIZE);         //cria janela
    imshow("Display Window", image);                        //mostra imagem na janela

    waitKey(0);                                             //espera tecla
    return 0;
}