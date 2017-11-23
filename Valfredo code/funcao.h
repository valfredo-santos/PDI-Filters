#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "windows.h"
#include <iostream>
#include "math.h"
#include "opencv2/core/core.hpp"
#include <iomanip>
#include <fstream>

#ifndef FUNCAO_H_
#define FUNCAO_H_

using namespace cv;
using namespace std;

/*FUNCOES PARA AUXILIAR AS DEMAIS OU PARA TESTES*/

void quickSort(int arr[], int left, int right) {
	int i = left, j = right;
	int tmp;
	int pivot = arr[(left + right) / 2];

	/* partition */
	while (i <= j) {
		while (arr[i] < pivot)
			i++;
		while (arr[j] > pivot)
			j--;
		if (i <= j) {
			tmp = arr[i];
			arr[i] = arr[j];
			arr[j] = tmp;
			i++;
			j--;
		}
	};

	/* recursion */
	if (left < j)
		quickSort(arr, left, j);
	if (i < right)
		quickSort(arr, i, right);
}

void Gray(Mat img)
{
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

void ComparaRGBYIQ(Mat img1, Mat img2) {
	int i, j, k;
	for (int n = 0; n < img1.rows; n++) {
		for (int m = 0; m < img1.cols; m++) {
			i = img2.at<cv::Vec3b>(n, m)[0] - img1.at<cv::Vec3b>(n, m)[0];
			j = img2.at<cv::Vec3b>(n, m)[1] - img1.at<cv::Vec3b>(n, m)[1];
			k = img2.at<cv::Vec3b>(n, m)[2] - img1.at<cv::Vec3b>(n, m)[2];

			if (i < 0)
				i = 0;
			if (j < 0)
				j = 0;
			if (k < 0)
				k = 0;
			if (i > 255)
				i = 255;
			if (j > 255)
				j = 255;
			if (k > 255)
				k = 255;

			/*if (((i < 5) && (i > 0)) || ((j < 5) && (j > 0)) || ((k < 5) && (k > 0))) {
				i = 0;
				j = 0;
				k = 0;
			}*/

			img1.at<cv::Vec3b>(n, m)[0] = i;
			img1.at<cv::Vec3b>(n, m)[1] = j;
			img1.at<cv::Vec3b>(n, m)[2] = k;
		}
	}
}

/*QUESTAO 1.1*/

void RGBYIQ(Mat img)
{
	/*constantes para operacoes de transformacao*/
	const float MIN_I = -0.5957f;
	const float MIN_Q = -0.5226f;
	const float I_TO_BYTE = 255.0f / (MIN_I * -2.0f);
	const float Q_TO_BYTE = 255.0f / (MIN_Q * -2.0f);

	/*variaveis para armazenar novos valores dos pixels*/
	float R, G, B;
	int NR, NG, NB;
	float NY, NI, NQ;
	int Y, I, Q;

	/*acesso dos pixels e de seus valores*/
	for (int n = 0; n < img.rows; n++) {
		for (int m = 0; m < img.cols; m++) {
			NB = img.at<cv::Vec3b>(n, m)[0];
			NG = img.at<cv::Vec3b>(n, m)[1];
			NR = img.at<cv::Vec3b>(n, m)[2];

			/*passagem de Int 8 bits para float*/
			B = NB / 255.0f;
			G = NG / 255.0f;
			R = NR / 255.0f;

			/*conversao de RGB para YIQ*/
			NY = (0.299 * R) + (0.587 * G) + (0.114 * B);
			NI = (0.596 * R) - (0.274 * G) - (0.322 * B);
			NQ = (0.211 * R) - (0.523 * G) + (0.312 * B);

			/*retorno de float para int 8 bits*/
			Y = (int)(0.5f + NY * 255);
			I = (int)(0.5f + (NI - MIN_I) * I_TO_BYTE);
			Q = (int)(0.5f + (NQ - MIN_Q) * Q_TO_BYTE);

			/*tratar casos maiores que 255 e menores que 0*/
			if (Y > 255) Y = 255;
			if (I > 255) I = 255;
			if (Q > 255) Q = 255;

			if (Y < 0) Y = 0;
			if (I < 0) I = 0;
			if (Q < 0) Q = 0;

			/*atribuir os valores ao pixel destino*/
			img.at<cv::Vec3b>(n, m)[0] = Y;
			img.at<cv::Vec3b>(n, m)[1] = I;
			img.at<cv::Vec3b>(n, m)[2] = Q;
		}
	}
}

void YIQRGB(Mat img)
{
	/*tudo aqui segue a mesma logica que na funcao RGBYIQ,
	mudando apenas os valores de conversao e as constantes*/
	const float MIN_I = -0.5957f;
	const float MIN_Q = -0.5226f;
	const float I_TO_FLOAT = -2.0f * MIN_I / 255.0f;
	const float Q_TO_FLOAT = -2.0f * MIN_Q / 255.0f;
	int R, G, B;
	float NR, NG, NB;
	float Y, I, Q;
	int NY, NI, NQ;
	for (int n = 0; n < img.rows; n++) {
		for (int m = 0; m < img.cols; m++) {
			NY = img.at<cv::Vec3b>(n, m)[0];
			NI = img.at<cv::Vec3b>(n, m)[1];
			NQ = img.at<cv::Vec3b>(n, m)[2];

			Y = (float)NY / 255.0f;
			I = (float)NI * I_TO_FLOAT + MIN_I;
			Q = (float)NQ * Q_TO_FLOAT + MIN_Q;

			NB = Y - (1.106 * I) + (1.703 * Q);
			NG = Y - (0.272 * I) - (0.647 * Q);
			NR = Y + (0.956 * I) + (0.621 * Q);

			R = (int)(NR * 255.0f);
			G = (int)(NG * 255.0f);
			B = (int)(NB * 255.0f);

			if (R > 255) R = 255;
			if (G > 255) G = 255;
			if (B > 255) B = 255;

			if (R < 0) R = 0;
			if (G < 0) G = 0;
			if (B < 0) B = 0;

			img.at<cv::Vec3b>(n, m)[0] = B;
			img.at<cv::Vec3b>(n, m)[1] = G;
			img.at<cv::Vec3b>(n, m)[2] = R;
		}
	}
}


/*QUESTAO 1.2*/

void BandaB(Mat img)
{
	/*A banda da cor e mantida e as demais zeradas*/
	for (int n = 0; n < img.rows; n++) {
		for (int m = 0; m < img.cols; m++) {
			img.at<cv::Vec3b>(n, m)[1] = 0;
			img.at<cv::Vec3b>(n, m)[2] = 0;
		}
	}
}

void BandaG(Mat img)
{
	for (int n = 0; n < img.rows; n++) {
		for (int m = 0; m < img.cols; m++) {
			img.at<cv::Vec3b>(n, m)[0] = 0;
			img.at<cv::Vec3b>(n, m)[2] = 0;
		}
	}
}

void BandaR(Mat img)
{
	for (int n = 0; n < img.rows; n++) {
		for (int m = 0; m < img.cols; m++) {
			img.at<cv::Vec3b>(n, m)[0] = 0;
			img.at<cv::Vec3b>(n, m)[1] = 0;
		}
	}
}

/*QUESTAO 1.3*/

void Negativo(Mat img)
{
	/*segue a formula negativo = 255 - valor do pixel*/
	for (int n = 0; n < img.rows; n++) {
		for (int m = 0; m < img.cols; m++) {
			img.at<cv::Vec3b>(n, m)[0] = 255 - img.at<cv::Vec3b>(n, m)[0];
			img.at<cv::Vec3b>(n, m)[1] = 255 - img.at<cv::Vec3b>(n, m)[1];
			img.at<cv::Vec3b>(n, m)[2] = 255 - img.at<cv::Vec3b>(n, m)[2];
		}
	}
}

void NegativoBanda(Mat img, int val)
{
	/*segue a formula negativo = 255 - valor do pixel*/
	for (int n = 0; n < img.rows; n++) {
		for (int m = 0; m < img.cols; m++) {
			img.at<cv::Vec3b>(n, m)[val] = 255 - img.at<cv::Vec3b>(n, m)[val];
			//img.at<cv::Vec3b>(n, m)[1] = 255 - img.at<cv::Vec3b>(n, m)[1];
			//img.at<cv::Vec3b>(n, m)[2] = 255 - img.at<cv::Vec3b>(n, m)[2];
		}
	}
}

void NegativoY(Mat img)
{
	/*passa para YIQ*/
	RGBYIQ(img);
	for (int n = 0; n < img.rows; n++) {
		for (int m = 0; m < img.cols; m++) {
			/*trabalha somente na banda Y*/
			img.at<cv::Vec3b>(n, m)[0] = 255 - img.at<cv::Vec3b>(n, m)[0];
		}
	}
	/*retorna para RGB*/
	YIQRGB(img);
}

/*QUESTAO 1.4*/

void ABrilho(Mat img, int valor)
{
	int R, G, B;

	for (int n = 0; n < img.rows; n++) {
		for (int m = 0; m < img.cols; m++) {
			B = img.at<cv::Vec3b>(n, m)[0];
			G = img.at<cv::Vec3b>(n, m)[1];
			R = img.at<cv::Vec3b>(n, m)[2];

			B += valor;
			G += valor;
			R += valor;

			if (R > 255) R = 255;
			if (G > 255) G = 255;
			if (B > 255) B = 255;

			if (R < 0) R = 0;
			if (G < 0) G = 0;
			if (B < 0) B = 0;

			img.at<cv::Vec3b>(n, m)[0] = B;
			img.at<cv::Vec3b>(n, m)[1] = G;
			img.at<cv::Vec3b>(n, m)[2] = R;
		}
	}
}

void ABrilhoBanda(Mat img, int valor, int banda)
{
	int x;

	for (int n = 0; n < img.rows; n++) {
		for (int m = 0; m < img.cols; m++) {
			x = img.at<cv::Vec3b>(n, m)[banda];

			x += valor;

			if (x > 255) x = 255;

			if (x < 0) x = 0;

			img.at<cv::Vec3b>(n, m)[banda] = x;
		}
	}
}

/*QUESTAO 1.5*/

void MBrilho(Mat img, float valor)
{
	int R, G, B;
	if (valor < 0)
		valor *= -1;
	for (int n = 0; n < img.rows; n++) {
		for (int m = 0; m < img.cols; m++) {
			B = img.at<cv::Vec3b>(n, m)[0];
			G = img.at<cv::Vec3b>(n, m)[1];
			R = img.at<cv::Vec3b>(n, m)[2];

			B *= valor;
			G *= valor;
			R *= valor;

			if (R > 255) R = 255;
			if (G > 255) G = 255;
			if (B > 255) B = 255;

			if (R < 0) R = 0;
			if (G < 0) G = 0;
			if (B < 0) B = 0;

			img.at<cv::Vec3b>(n, m)[0] = B;
			img.at<cv::Vec3b>(n, m)[1] = G;
			img.at<cv::Vec3b>(n, m)[2] = R;
		}
	}
}

void MBB(Mat img, float valor) {
	{
		int R, G, B;
		if (valor < 0)
			valor *= -1;
		for (int n = 0; n < img.rows; n++) {
			for (int m = 0; m < img.cols; m++) {
				B = img.at<cv::Vec3b>(n, m)[0];

				B *= valor;

				if (B > 255) B = 255;

				if (B < 0) B = 0;

				img.at<cv::Vec3b>(n, m)[0] = B;
			}
		}
	}
}

void MBrilhoBanda(Mat img, float valor, int banda)
{
	int x;
	if (valor < 0)
		valor *= -1;
	for (int n = 0; n < img.rows; n++) {
		for (int m = 0; m < img.cols; m++) {
			x = img.at<cv::Vec3b>(n, m)[banda];

			x *= valor;

			if (x > 255) x = 255;

			if (x < 0) x = 0;

			img.at<cv::Vec3b>(n, m)[banda] = x;
		}
	}
}

/*QUESTAO 1.6*/


void Limiarizacao(Mat img, int x)
{
	for (int n = 0; n < img.rows; n++) {
		for (int m = 0; m < img.cols; m++) {
			/*verifica se o valor e maior que o limite
			e decidi se fica branco ou preto*/
			if (img.at<uchar>(n, m) >= x) {
				//img.at<uchar>(n, m) = 255;
			}
			else {
				img.at<uchar>(n, m) = 0;
			}
		}
	}
}

void LimiarizacaoEscolhidaY(Mat img, int x)
{
	/*passa para tons de cinza e logo em seguida para YIQ*/
	Gray(img);
	RGBYIQ(img);
	for (int n = 0; n < img.rows; n++) {
		for (int m = 0; m < img.cols; m++) {
			/*a checagem e feita apenas na banda Y*/
			if (img.at<cv::Vec3b>(n, m)[0] >= x) {
				img.at<cv::Vec3b>(n, m)[0] = 255;
			}
			else {
				img.at<cv::Vec3b>(n, m)[0] = 0;
			}
		}
	}
	/*retorna para RGB*/
	YIQRGB(img);
}

void LimiarizacaoMediaY(Mat img)
{
	/*O mesmo processo da anterior, porem o limite
	e dado pela media dos valores da banda Y*/
	Gray(img);
	RGBYIQ(img);
	long total = 0, soma = 0;
	float media;
	int limite;

	/*calculo da media*/
	for (int n = 0; n < img.rows; n++) {
		for (int m = 0; m < img.cols; m++) {
			soma += img.at<cv::Vec3b>(n, m)[0];
			total++; //valor total de pixeis que foram somados
		}
	}
	media = soma / total;
	limite = (int)media;

	/*limiariza com o limite guardado na variavel limite*/
	for (int n = 0; n < img.rows; n++) {
		for (int m = 0; m < img.cols; m++) {
			if (img.at<cv::Vec3b>(n, m)[0] >= limite) {
				img.at<cv::Vec3b>(n, m)[0] = 255;
			}
			else {
				img.at<cv::Vec3b>(n, m)[0] = 0;
			}
		}
	}
	YIQRGB(img);
}

/*QUESTAO 1.7*/

void filtroMedia(Mat img, int val) {

	int h = (val / 2);
	int soma1, soma2, soma3;
	float media1, media2, media3;
	Mat clone = img.clone();
	int i, j;
	int n = h, m = h;
	int x = img.rows - h;
	int y = img.cols - h;
	for (n = h; n < x; n++) {
		for (m = h; m < y; m++) {
			int a = -h, b = -h;
			soma1 = 0, soma2 = 0, soma3 = 0;
			for (a = -h; a < (h + 1); a++) {
				for (b = -h; b < (h + 1); b++) {
					i = n + a;
					j = m + b;
					soma1 += img.at<cv::Vec3b>(i, j)[0];
					soma2 += img.at<cv::Vec3b>(i, j)[1];
					soma3 += img.at<cv::Vec3b>(i, j)[2];
				}
			}
			media1 = soma1 / (val * val);
			media2 = soma2 / (val * val);
			media3 = soma3 / (val * val);
			clone.at<cv::Vec3b>(n - 1, m - 1)[0] = (int)media1;
			clone.at<cv::Vec3b>(n - 1, m - 1)[1] = (int)media2;
			clone.at<cv::Vec3b>(n - 1, m - 1)[2] = (int)media3;
		}
	}
	for (int n = 0; n < img.rows; n++) {
		for (int m = 0; m < img.cols; m++) {
			img.at<cv::Vec3b>(n, m)[0] = clone.at<cv::Vec3b>(n, m)[0];
			img.at<cv::Vec3b>(n, m)[1] = clone.at<cv::Vec3b>(n, m)[1];
			img.at<cv::Vec3b>(n, m)[2] = clone.at<cv::Vec3b>(n, m)[2];
		}
	}
}

void filtroMediana(Mat img, int val)
{
	Mat clone = img.clone();
	int h = (val / 2);
	int i, j;
	int t;
	int a, b;
	clock_t begin = clock();
	for (int n = 0; n < img.rows; n++) {
		for (int m = 0; m < img.cols; m++) {
			int janelaB[1000];
			int janelaG[1000];
			int janelaR[1000];
			a = -h, b = -h;
			int k = 0;
			for (a = -h; a < (h + 1); a++) {
				for (b = -h; b < (h + 1); b++) {
					i = n + a;
					j = m + b;

					if (i < 0 || i >= img.rows || j < 0 || j >= img.cols) {}
					else {
						janelaB[k] = img.at<cv::Vec3b>(i, j)[0];
						janelaG[k] = img.at<cv::Vec3b>(i, j)[1];
						janelaR[k] = img.at<cv::Vec3b>(i, j)[2];
						k++;
					}
				}
			}

			quickSort(janelaB, 0, k);
			quickSort(janelaR, 0, k);
			quickSort(janelaG, 0, k);

			if (k % 2 == 0) {
				t = k / 2;
				clone.at<cv::Vec3b>(n, m)[0] = janelaB[t];
				clone.at<cv::Vec3b>(n, m)[1] = janelaG[t];
				clone.at<cv::Vec3b>(n, m)[2] = janelaR[t];
			}
			else {
				int media1, media2, media3;
				t = k / 2;
				media1 = (int)((janelaB[t] + janelaB[t + 1]) / 2);
				media2 = (int)((janelaG[t] + janelaG[t + 1]) / 2);
				media3 = (int)((janelaR[t] + janelaR[t + 1]) / 2);

				clone.at<cv::Vec3b>(n, m)[0] = media1;
				clone.at<cv::Vec3b>(n, m)[1] = media2;
				clone.at<cv::Vec3b>(n, m)[2] = media3;
			}
		}
	}

	clock_t end = clock();

	for (int n = 0; n < img.rows; n++) {
		for (int m = 0; m < img.cols; m++) {
			img.at<cv::Vec3b>(n, m)[0] = clone.at<cv::Vec3b>(n, m)[0];
			img.at<cv::Vec3b>(n, m)[1] = clone.at<cv::Vec3b>(n, m)[1];
			img.at<cv::Vec3b>(n, m)[2] = clone.at<cv::Vec3b>(n, m)[2];
		}
	}

	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

	std::cout << elapsed_secs << std::endl;

}

/*QUESTAO 1.8*/

void Sobel(Mat img)
{
	Gray(img);
	Mat clone = img.clone();

	int sobx[3][3] = { { -1, 0, 1 },
						{ -2, 0, 2 },
						{ -1, 0, 1 } };

	int soby[3][3] = { { -1, -2, -1 },
						{ 0,  0,  0 },
						{ 1,  2,  1 } };

	int x = img.rows - 1;
	int y = img.cols - 1;

	for (int n = 1; n < x; n++) {
		for (int m = 1; m < y; m++) {
			int a = -1, b = -1;
			int somay = 0, somax = 0;
			for (a = -1; a < 2; a++) {
				for (b = -1; b < 2; b++) {
					int i = n + a;
					int j = m + b;
					somax += (img.at<cv::Vec3b>(i, j)[0] * sobx[a + 1][b + 1]) / 4;
					somay += (img.at<cv::Vec3b>(i, j)[0] * soby[a + 1][b + 1]) / 4;
				}
			}
			if (somax > 255)
				somax = 255;
			if (somax < 0)
				somax = 0;
			if (somay > 255)
				somay = 255;
			if (somay < 0)
				somay = 0;
			clone.at<cv::Vec3b>(n - 1, m - 1)[0] = sqrt(pow(somax, 2) + pow(somay, 2));
			clone.at<cv::Vec3b>(n - 1, m - 1)[1] = clone.at<cv::Vec3b>(n, m)[0];
			clone.at<cv::Vec3b>(n - 1, m - 1)[2] = clone.at<cv::Vec3b>(n, m)[0];
		}
	}

	for (int n = 0; n < img.rows; n++) {
		for (int m = 0; m < img.cols; m++) {
			img.at<cv::Vec3b>(n, m)[0] = clone.at<cv::Vec3b>(n, m)[0];
			img.at<cv::Vec3b>(n, m)[1] = clone.at<cv::Vec3b>(n, m)[0];
			img.at<cv::Vec3b>(n, m)[2] = clone.at<cv::Vec3b>(n, m)[0];
		}
	}
	MBrilho(img, 2);
}

void SobelnoOutroSentido(Mat img)
{
	Gray(img);
	Mat clone = img.clone();

	int sobx[3][3] = { { 1, 0, -1 },
	{ 2, 0, -2 },
	{ 1, 0, -1 } };

	int soby[3][3] = { { 1, 2, 1 },
	{ 0,  0,  0 },
	{ -1,  -2,  -1 } };

	int x = img.rows - 1;
	int y = img.cols - 1;

	for (int n = 1; n < x; n++) {
		for (int m = 1; m < y; m++) {
			int a = -1, b = -1;
			int somay = 0, somax = 0;
			for (a = -1; a < 2; a++) {
				for (b = -1; b < 2; b++) {
					int i = n + a;
					int j = m + b;
					somax += (img.at<cv::Vec3b>(i, j)[0] * sobx[a + 1][b + 1]) / 4;
					somay += (img.at<cv::Vec3b>(i, j)[0] * soby[a + 1][b + 1]) / 4;
				}
			}
			if (somax > 255)
				somax = 255;
			if (somax < 0)
				somax = 0;
			if (somay > 255)
				somay = 255;
			if (somay < 0)
				somay = 0;
			clone.at<cv::Vec3b>(n - 1, m - 1)[0] = sqrt(pow(somax, 2) + pow(somay, 2));
			clone.at<cv::Vec3b>(n - 1, m - 1)[1] = clone.at<cv::Vec3b>(n, m)[0];
			clone.at<cv::Vec3b>(n - 1, m - 1)[2] = clone.at<cv::Vec3b>(n, m)[0];
		}
	}

	for (int n = 0; n < img.rows; n++) {
		for (int m = 0; m < img.cols; m++) {
			img.at<cv::Vec3b>(n, m)[0] = clone.at<cv::Vec3b>(n, m)[0];
			img.at<cv::Vec3b>(n, m)[1] = clone.at<cv::Vec3b>(n, m)[0];
			img.at<cv::Vec3b>(n, m)[2] = clone.at<cv::Vec3b>(n, m)[0];
		}
	}
	MBrilho(img, 2);
}

void Laplace(Mat img)
{
	Gray(img);
	Mat clone = img.clone();

	int	lap[3][3] = { { 0, -1, 0 },
					  { -1, 4, -1 },
					  { 0, -1, 0 } };

	int x = img.rows - 1;
	int y = img.cols - 1;

	for (int n = 1; n < x; n++) {
		for (int m = 1; m < y; m++) {
			int a = -1, b = -1;
			int soma = 0;
			for (a = -1; a < 2; a++) {
				for (b = -1; b < 2; b++) {
					int i = n + a;
					int j = m + b;
					soma += (img.at<cv::Vec3b>(i, j)[0] * lap[a + 1][b + 1]);
				}
			}
			if (soma > 255)
				soma = 255;
			if (soma < 0)
				soma = 0;
			clone.at<cv::Vec3b>(n - 1, m - 1)[0] = soma;
		}
	}

	for (int n = 0; n < img.rows; n++) {
		for (int m = 0; m < img.cols; m++) {
			img.at<cv::Vec3b>(n, m)[0] = clone.at<cv::Vec3b>(n, m)[0];
			img.at<cv::Vec3b>(n, m)[1] = clone.at<cv::Vec3b>(n, m)[0];
			img.at<cv::Vec3b>(n, m)[2] = clone.at<cv::Vec3b>(n, m)[0];
		}
	}
}

/*QUESTAO 1.9*/

void Sharpen(Mat img)
{
	Mat clone = img.clone();

	int sha[3][3] = { { 0, -1, 0 },
					{ -1, 5, -1 },
					{ 0, -1, 0 } };

	int x = img.rows - 1;
	int y = img.cols - 1;

	for (int n = 1; n < x; n++) {
		for (int m = 1; m < y; m++) {
			int a = -1, b = -1;
			int soma1 = 0, soma2 = 0, soma3 = 0;
			for (a = -1; a < 2; a++) {
				for (b = -1; b < 2; b++) {
					int i = n + a;
					int j = m + b;
					soma1 += (img.at<cv::Vec3b>(i, j)[0] * sha[a + 1][b + 1]);
					soma2 += (img.at<cv::Vec3b>(i, j)[1] * sha[a + 1][b + 1]);
					soma3 += (img.at<cv::Vec3b>(i, j)[2] * sha[a + 1][b + 1]);
				}
			}
			if (soma1 > 255)
				soma1 = 255;
			if (soma1 < 0)
				soma1 = 0;
			if (soma2 > 255)
				soma2 = 255;
			if (soma2 < 0)
				soma2 = 0;
			if (soma3 > 255)
				soma3 = 255;
			if (soma3 < 0)
				soma3 = 0;
			clone.at<cv::Vec3b>(n - 1, m - 1)[0] = soma1;
			clone.at<cv::Vec3b>(n - 1, m - 1)[1] = soma2;
			clone.at<cv::Vec3b>(n - 1, m - 1)[2] = soma3;
		}
	}

	for (int n = 0; n < img.rows; n++) {
		for (int m = 0; m < img.cols; m++) {
			img.at<cv::Vec3b>(n, m)[0] = clone.at<cv::Vec3b>(n, m)[0];
			img.at<cv::Vec3b>(n, m)[1] = clone.at<cv::Vec3b>(n, m)[1];
			img.at<cv::Vec3b>(n, m)[2] = clone.at<cv::Vec3b>(n, m)[2];
		}
	}
}

void Bordas(Mat img)
{
	Gray(img);
	Mat clone = img.clone();

	int sha[3][3] = { { 0, 0, 0 },
						{ 0, 1, 0 },
						{0, 0, -1 } };

	int x = img.rows - 1;
	int y = img.cols - 1;

	for (int n = 1; n < x; n++) {
		for (int m = 1; m < y; m++) {
			int a = -1, b = -1;
			int soma1 = 0, soma2 = 0, soma3 = 0;
			for (a = -1; a < 2; a++) {
				for (b = -1; b < 2; b++) {
					int i = n + a;
					int j = m + b;
					soma1 += (img.at<cv::Vec3b>(i, j)[0] * sha[a + 1][b + 1]);
					soma2 += (img.at<cv::Vec3b>(i, j)[1] * sha[a + 1][b + 1]);
					soma3 += (img.at<cv::Vec3b>(i, j)[2] * sha[a + 1][b + 1]);
				}
			}
			if (soma1 > 255)
				soma1 = 255;
			if (soma1 < 0)
				soma1 = 0;
			if (soma2 > 255)
				soma2 = 255;
			if (soma2 < 0)
				soma2 = 0;
			if (soma3 > 255)
				soma3 = 255;
			if (soma3 < 0)
				soma3 = 0;
			clone.at<cv::Vec3b>(n - 1, m - 1)[0] = soma1;
			clone.at<cv::Vec3b>(n - 1, m - 1)[1] = soma2;
			clone.at<cv::Vec3b>(n - 1, m - 1)[2] = soma3;
		}
	}

	for (int n = 0; n < img.rows; n++) {
		for (int m = 0; m < img.cols; m++) {
			img.at<cv::Vec3b>(n, m)[0] = clone.at<cv::Vec3b>(n, m)[0];
			img.at<cv::Vec3b>(n, m)[1] = clone.at<cv::Vec3b>(n, m)[1];
			img.at<cv::Vec3b>(n, m)[2] = clone.at<cv::Vec3b>(n, m)[2];
		}
	}
}

/*SEGUNDO TRABALHO*/

int partition(int* input, int p, int r)
{
	int pivot = input[r];

	while (p < r)
	{
		while (input[p] < pivot)
			p++;

		while (input[r] > pivot)
			r--;

		if (input[p] == input[r])
			p++;
		else if (p < r) {
			int tmp = input[p];
			input[p] = input[r];
			input[r] = tmp;
		}
	}

	return r;
}

int quick_select(int* input, int p, int r, int k)
{
	if (p == r) return input[p];
	int j = partition(input, p, r);
	int length = j - p + 1;
	if (length == k) return input[j];
	else if (k < length) return quick_select(input, p, j - 1, k);
	else  return quick_select(input, j + 1, r, k - length);
}

void ExpandeHistograma(Mat img) {
	Gray(img);
	float histo[256];
	int val, delta, min, max, saida;
	float presaida, temp;

	for (int n = 0; n < 256; n++) {
		histo[n] = 0.0f;
	}

	for (int n = 0; n < img.rows; n++) {
		for (int m = 0; m < img.cols; m++) {
			val = img.at<cv::Vec3b>(n, m)[0];
			histo[val]++;
		}
	}

	for (int n = 0; n < 256; n++) {
		if (histo[n] > 0) {
			min = n;
			break;
		}
	}
	for (int n = 255; n >= 0; n--) {
		if (histo[n] > 0) {
			max = n;
			break;
		}
	}

	delta = max - min;

	for (int n = 0; n < img.rows; n++) {
		for (int m = 0; m < img.cols; m++) {
			presaida = (img.at<cv::Vec3b>(n, m)[0] - min) * (255 / delta);

			saida = (int)presaida;
			temp = presaida - saida;
			if (temp > 0.5) {
				saida++;
			}

			if (saida > 255)
				saida = 255;
			if (saida < 0)
				saida = 0;

			img.at<cv::Vec3b>(n, m)[0] = saida;
			img.at<cv::Vec3b>(n, m)[1] = saida;
			img.at<cv::Vec3b>(n, m)[2] = saida;
		}
	}
}

void meuExpandeHistograma(Mat img) {
	Mat clone = img.clone();
	Gray(clone);
	float histo[256];
	int val, delta, min, max, saida1, saida2, saida3;
	float presaida1, presaida2, presaida3, temp1, temp2, temp3;

	for (int n = 0; n < 256; n++) {
		histo[n] = 0.0f;
	}

	for (int n = 0; n < img.rows; n++) {
		for (int m = 0; m < img.cols; m++) {
			val = clone.at<cv::Vec3b>(n, m)[0];
			histo[val]++;
		}
	}

	for (int n = 0; n < 256; n++) {
		if (histo[n] > 0) {
			min = n;
			break;
		}
	}
	for (int n = 255; n >= 0; n--) {
		if (histo[n] > 0) {
			max = n;
			break;
		}
	}

	delta = max - min;
	int k = 255;

	for (int n = 0; n < img.rows; n++) {
		for (int m = 0; m < img.cols; m++) {
			presaida1 = (img.at<cv::Vec3b>(n, m)[0] - min) * (k / delta);
			presaida2 = (img.at<cv::Vec3b>(n, m)[1] - min) * (k / delta);
			presaida3 = (img.at<cv::Vec3b>(n, m)[2] - min) * (k / delta);

			saida1 = (int)presaida1;
			saida2 = (int)presaida2;
			saida3 = (int)presaida3;

			temp1 = presaida1 - saida1;
			temp2 = presaida2 - saida2;
			temp3 = presaida3 - saida3;

			if (temp1 > 0.5) {
				saida1++;
			}
			if (temp2 > 0.5) {
				saida2++;
			}
			if (temp3 > 0.5) {
				saida3++;
			}

			if (saida1 > 255)
				saida1 = 255;
			if (saida1 < 0)
				saida1 = 0;
			if (saida2 > 255)
				saida2 = 255;
			if (saida2 < 0)
				saida2 = 0;
			if (saida3 > 255)
				saida3 = 255;
			if (saida3 < 0)
				saida3 = 0;

			img.at<cv::Vec3b>(n, m)[0] = saida1;
			img.at<cv::Vec3b>(n, m)[1] = saida2;
			img.at<cv::Vec3b>(n, m)[2] = saida3;
		}
	}
}

void EqualizaHistograma(Mat img) {
	Gray(img);
	float histo[256], Rsomatorio[256], Rsaida[256];
	int val, temp, saida, atual;
	float presaida, RC;

	RC = img.cols * img.rows;

	for (int n = 0; n < 256; n++) {
		histo[n] = 0.0f;
		Rsomatorio[n] = 0.0f;
		Rsaida[0] = 0.0f;
	}

	for (int n = 0; n < img.rows; n++) {
		for (int m = 0; m < img.cols; m++) {
			val = img.at<cv::Vec3b>(n, m)[0];
			histo[val]++;
		}
	}

	for (int a = 0; a < 256; a++) {
		if (a == 0) {
			Rsomatorio[a] = histo[0];
		}
		else {
			Rsomatorio[a] = Rsomatorio[a - 1] + histo[a];
		}
	}

	for (int n = 0; n < 256; n++) {
		presaida = (Rsomatorio[n] * 255) / RC;
		temp = (int)presaida;
		if (presaida - temp > 0.5) {
			temp++;
		}

		if (temp > 255)
			temp = 255;
		if (temp < 0)
			temp = 0;

		Rsaida[n] = temp;
	}

	for (int n = 0; n < img.rows; n++) {
		for (int m = 0; m < img.cols; m++) {
			atual = img.at<cv::Vec3b>(n, m)[0];
			saida = Rsaida[atual];
			img.at<cv::Vec3b>(n, m)[0] = saida;
			img.at<cv::Vec3b>(n, m)[1] = saida;
			img.at<cv::Vec3b>(n, m)[2] = saida;
		}
	}
}

void filtroMedia2(Mat img, int vX, int vY) {

	int soma1, soma2, soma3;
	float media1, media2, media3;
	Mat clone = img.clone();
	int i, j;
	int raioN, raioM;
	int ps1, ps2, ps3;

	raioN = (vY / 2);
	raioM = (vX / 2);

	clock_t begin = clock();

	for (int n = 0; n < img.rows; n++) {
		for (int m = 0; m < img.cols; m++) {
			soma1 = 0, soma2 = 0, soma3 = 0;
			ps1 = 0, ps2 = 0, ps3 = 0;
			
			for (int a = -raioN; a < (raioN + 1); a++) {
				for (int b = -raioM; b < (raioM + 1); b++) {
					i = n + a;
					j = m + b;

					if (i < 0 || i >= img.rows || j < 0 || j >= img.cols) {}
					else{
						soma1 += img.at<cv::Vec3b>(i, j)[0];
						soma2 += img.at<cv::Vec3b>(i, j)[1];
						soma3 += img.at<cv::Vec3b>(i, j)[2];
						ps1++;
						ps2++;
						ps3++;
					}
				}
			}

			media1 = soma1 / (ps1);
			media2 = soma2 / (ps2);
			media3 = soma3 / (ps3);
			clone.at<cv::Vec3b>(n, m)[0] = (int)media1;
			clone.at<cv::Vec3b>(n, m)[1] = (int)media2;
			clone.at<cv::Vec3b>(n, m)[2] = (int)media3;
		}
	}

	clock_t end = clock();

	for (int n = 0; n < img.rows; n++) {
		for (int m = 0; m < img.cols; m++) {
			img.at<cv::Vec3b>(n, m)[0] = clone.at<cv::Vec3b>(n, m)[0];
			img.at<cv::Vec3b>(n, m)[1] = clone.at<cv::Vec3b>(n, m)[1];
			img.at<cv::Vec3b>(n, m)[2] = clone.at<cv::Vec3b>(n, m)[2];
		}
	}

	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

	std::cout << elapsed_secs << std::endl;
}

void filtroMediana2(Mat img, int val)
{
	Mat clone = img.clone();
	int h = (val / 2);
	int i, j;
	int t;
	int a, b;
	clock_t begin = clock();
	for (int n = 0; n < img.rows; n++) {
		for (int m = 0; m < img.cols; m++) {
			int janelaB[1000];
			int janelaG[1000];
			int janelaR[1000];
			a = -h, b = -h;
			int k = 0;
			for (a = -h; a < (h + 1); a++) {
				for (b = -h; b < (h + 1); b++) {
					i = n + a;
					j = m + b;

					if (i < 0 || i >= img.rows || j < 0 || j >= img.cols) {}
					else {
						janelaB[k] = img.at<cv::Vec3b>(i, j)[0];
						janelaG[k] = img.at<cv::Vec3b>(i, j)[1];
						janelaR[k] = img.at<cv::Vec3b>(i, j)[2];
						k++;
					}
				}
			}
			t = (k+1) / 2;
			clone.at<cv::Vec3b>(n, m)[0] = quick_select(janelaB, 0, k, t);
			clone.at<cv::Vec3b>(n, m)[1] = quick_select(janelaG, 0, k, t);
			clone.at<cv::Vec3b>(n, m)[2] = quick_select(janelaR, 0, k, t);
		}
	}

	clock_t end = clock();

	for (int n = 0; n < img.rows; n++) {
		for (int m = 0; m < img.cols; m++) {
			img.at<cv::Vec3b>(n, m)[0] = clone.at<cv::Vec3b>(n, m)[0];
			img.at<cv::Vec3b>(n, m)[1] = clone.at<cv::Vec3b>(n, m)[1];
			img.at<cv::Vec3b>(n, m)[2] = clone.at<cv::Vec3b>(n, m)[2];
		}
	}

	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

	std::cout << elapsed_secs << std::endl;

}

Mat fundoMedia(Mat img[], int quant) {
	
	Mat clone = img[0].clone();
	int soma1, soma2, soma3;
	float media1, media2, media3;
	int y = quant;

	for (int n = 0; n < clone.rows; n++) {
		for (int m = 0; m < clone.cols; m++) {
			soma1 = 0, soma2 = 0, soma3 = 0;
			for (int x = 0; x < y; x++) {
				soma1 += img[x].at<cv::Vec3b>(n, m)[0];
				soma2 += img[x].at<cv::Vec3b>(n, m)[1];
				soma3 += img[x].at<cv::Vec3b>(n, m)[2];
			}
			media1 = soma1 / y;
			media2 = soma2 / y;
			media3 = soma3 / y;

			clone.at<cv::Vec3b>(n, m)[0] = (int)media1;
			clone.at<cv::Vec3b>(n, m)[1] = (int)media2;
			clone.at<cv::Vec3b>(n, m)[2] = (int)media3;
		}
	}

	return clone;
}

Mat fundoMediana(Mat img[], int quant) {

	Mat clone = img[0].clone();
	int y = quant;

	int PB[15], PG[15], PR[15];

	for (int n = 0; n < clone.rows; n++) {
		for (int m = 0; m < clone.cols; m++) {
			for (int x = 0; x < y; x++) {
				PB[x] = img[x].at<cv::Vec3b>(n, m)[0];
				PG[x] = img[x].at<cv::Vec3b>(n, m)[1];
				PR[x] = img[x].at<cv::Vec3b>(n, m)[2];
			}
			quickSort(PB, 0, y - 1);
			quickSort(PG, 0, y - 1);
			quickSort(PR, 0, y - 1);

			clone.at<cv::Vec3b>(n, m)[0] = PB[8];
			clone.at<cv::Vec3b>(n, m)[1] = PG[8];
			clone.at<cv::Vec3b>(n, m)[2] = PR[8];
		}
	}

	return clone;
}

bool AddGaussianNoise_Opencv(const Mat mSrc, Mat &mDst, double Mean = 0.0, double StdDev = 10.0)
{
	if (mSrc.empty())
	{
		cout << "[Error]! Input Image Empty!";
		return 0;
	}
	Mat mSrc_16SC;
	Mat mGaussian_noise = Mat(mSrc.size(), CV_16SC3);
	randn(mGaussian_noise, Scalar::all(Mean), Scalar::all(StdDev));

	mSrc.convertTo(mSrc_16SC, CV_16SC3);
	addWeighted(mSrc_16SC, 1.0, mGaussian_noise, 1.0, 0.0, mSrc_16SC);
	mSrc_16SC.convertTo(mDst, mSrc.type());

	return true;
}

/*##########################
  TRABALHO FINAL COMECA AKI
##########################*/

int RetornoDoMaior(int a, int b, int c, int d) {
	int x, y;
	int n, m;

	if((a == 0 && b == 0 && c == 0 && d == 0)) {
		return 4;
	}

	if (a > b){
		x = a;
		n = 0;
	} else {
		x = b;
		n = 1;
	}
	if (c > d){
		y = c;
		m = 2;
	} else {
		y = d;
		m = 3;
	}
	if (x > y)
		return n;
	else
		return m;
}

void GrayBrilho(Mat img, float valor) {	
		int R, G, B;
		if (valor < 0)
			valor *= -1;
		for (int n = 0; n < img.rows; n++) {
			for (int m = 0; m < img.cols; m++) {
				B = img.at<uchar>(n, m);

				B *= valor;

				if (B > 255) B = 255;

				if (B < 0) B = 0;

				img.at<uchar>(n, m) = B;
			}
		}	
}

void GrayMediana(Mat img, int val) {
	Mat clone = img.clone();
	int h = (val / 2);
	int i, j;
	int t;
	int a, b;
	for (int n = 0; n < img.rows; n++) {
		for (int m = 0; m < img.cols; m++) {
			int janela[26];
			a = -h, b = -h;
			int k = 0;
			for (a = -h; a < (h + 1); a++) {
				for (b = -h; b < (h + 1); b++) {
					i = n + a;
					j = m + b;
					if (i < 0 || i >= img.rows || j < 0 || j >= img.cols) {}
					else {
						janela[k] = img.at<uchar>(i, j);
						k++;
					}
				}
			}
			quickSort(janela, 0, k);

			t = k / 2;
			clone.at<uchar>(n, m) = janela[t];		
		}
	}

	for (int n = 0; n < img.rows; n++) {
		for (int m = 0; m < img.cols; m++) {
			img.at<uchar>(n, m) = clone.at<uchar>(n, m);
		}
	}
}

void GraySharpen(Mat img) {
	Mat clone = img.clone();

	int sobx[3][3] = { { 0, -1, 0 },
						{ -1, 5, -1 },
						{ 0, -1, 0 } };

	int x = img.rows - 1;
	int y = img.cols - 1;

	for (int n = 1; n < x; n++) {
		for (int m = 1; m < y; m++) {
			int a = -1, b = -1;
			int somay = 0, somax = 0;
			for (a = -1; a < 2; a++) {
				for (b = -1; b < 2; b++) {
					int i = n + a;
					int j = m + b;
					somax += (img.at<uchar>(i, j) * sobx[a + 1][b + 1]);
				}
			}
			if (somax > 255)
				somax = 255;
			if (somax < 0)
				somax = 0;
			if (somay > 255)
				somay = 255;
			if (somay < 0)
				somay = 0;
			clone.at<uchar>(n, m) = sqrt(pow(somax, 2));
		}
	}

	for (int n = 0; n < img.rows; n++) {
		for (int m = 0; m < img.cols; m++) {
			img.at<uchar>(n, m) = clone.at<uchar>(n, m);
		}
	}
	Limiarizacao(img, 40);
}

void GrayBordasVerticais(Mat img) {
	Mat clone = img.clone();

	int sobx[3][3] = { { -1, -1, -1 },
					   { 2, 2, 2 },
					   { -1, -1, -1 } };

	int x = img.rows - 1;
	int y = img.cols - 1;

	for (int n = 1; n < x; n++) {
		for (int m = 1; m < y; m++) {
			int a = -1, b = -1;
			int somay = 0, somax = 0;
			for (a = -1; a < 2; a++) {
				for (b = -1; b < 2; b++) {
					int i = n + a;
					int j = m + b;
					somax += (img.at<uchar>(i, j) * sobx[a + 1][b + 1]);
				}
			}
			if (somax > 255)
				somax = 255;
			if (somax < 0)
				somax = 0;
			if (somay > 255)
				somay = 255;
			if (somay < 0)
				somay = 0;
			clone.at<uchar>(n, m) = sqrt(pow(somax, 2));
		}
	}

	for (int n = 0; n < img.rows; n++) {
		for (int m = 0; m < img.cols; m++) {
			img.at<uchar>(n, m) = clone.at<uchar>(n, m);
		}
	}
	Limiarizacao(img, 40);
}

void GrayBordasHorizontais(Mat img) {
	Mat clone = img.clone();

	int sobx[3][3] = { { -1, 2, -1 },
						{ -1, 2, -1 },
						{ -1, 2, -1 } };

	int x = img.rows - 1;
	int y = img.cols - 1;

	for (int n = 1; n < x; n++) {
		for (int m = 1; m < y; m++) {
			int a = -1, b = -1;
			int somay = 0, somax = 0;
			for (a = -1; a < 2; a++) {
				for (b = -1; b < 2; b++) {
					int i = n + a;
					int j = m + b;
					somax += (img.at<uchar>(i, j) * sobx[a + 1][b + 1]);
				}
			}
			if (somax > 255)
				somax = 255;
			if (somax < 0)
				somax = 0;
			if (somay > 255)
				somay = 255;
			if (somay < 0)
				somay = 0;
			clone.at<uchar>(n, m) = sqrt(pow(somax, 2));
		}
	}

	for (int n = 0; n < img.rows; n++) {
		for (int m = 0; m < img.cols; m++) {
			img.at<uchar>(n, m) = clone.at<uchar>(n, m);
		}
	}
	Limiarizacao(img, 40);
}

void GrayBordasDiagonaisNegativas(Mat img) {
	Mat clone = img.clone();

	int sobx[3][3] = { { -1, -1, 2 },
					   { -1, 2, -1 },
					   { 2, -1, -1 } };

	int x = img.rows - 1;
	int y = img.cols - 1;

	for (int n = 1; n < x; n++) {
		for (int m = 1; m < y; m++) {
			int a = -1, b = -1;
			int somay = 0, somax = 0;
			for (a = -1; a < 2; a++) {
				for (b = -1; b < 2; b++) {
					int i = n + a;
					int j = m + b;
					somax += (img.at<uchar>(i, j) * sobx[a + 1][b + 1]);
				}
			}
			if (somax > 255)
				somax = 255;
			if (somax < 0)
				somax = 0;
			if (somay > 255)
				somay = 255;
			if (somay < 0)
				somay = 0;
			clone.at<uchar>(n, m) = sqrt(pow(somax, 2));
		}
	}

	for (int n = 0; n < img.rows; n++) {
		for (int m = 0; m < img.cols; m++) {
			img.at<uchar>(n, m) = clone.at<uchar>(n, m);
		}
	}
	Limiarizacao(img, 40);
}

void GrayBordasDiagonaisPositivas(Mat img) {
	Mat clone = img.clone();

	int sobx[3][3] = { { 2, -1, -1 },
					   { -1, 2, -1 },
					   { -1, -1, 2 } };

	int x = img.rows - 1;
	int y = img.cols - 1;

	for (int n = 1; n < x; n++) {
		for (int m = 1; m < y; m++) {
			int a = -1, b = -1;
			int somay = 0, somax = 0;
			for (a = -1; a < 2; a++) {
				for (b = -1; b < 2; b++) {
					int i = n + a;
					int j = m + b;
					somax += (img.at<uchar>(i, j) * sobx[a + 1][b + 1]);
				}
			}
			if (somax > 255)
				somax = 255;
			if (somax < 0)
				somax = 0;
			if (somay > 255)
				somay = 255;
			if (somay < 0)
				somay = 0;
			clone.at<uchar>(n, m) = sqrt(pow(somax, 2));
		}
	}

	for (int n = 0; n < img.rows; n++) {
		for (int m = 0; m < img.cols; m++) {
			img.at<uchar>(n, m) = clone.at<uchar>(n, m);
		}
	}
	Limiarizacao(img, 40);
}

void writeMatToFile(Mat img, const char* filename) {
	char ASCII[5];
	char ASCII2[10];
	int a;
	float b;
	int x, y, i, c, l;

	ASCII[4] = ' ';
	ASCII[3] = '/';
	ASCII[2] = '\\';
	ASCII[1] = '-';
	ASCII[0] = '|';

	ASCII2[9] = ' ';
	ASCII2[8] = '.';
	ASCII2[7] = ',';
	ASCII2[6] = ':';
	ASCII2[5] = ';';
	ASCII2[4] = 'o';
	ASCII2[3] = 'x';
	ASCII2[2] = '%';
	ASCII2[1] = '#';
	ASCII2[0] = '@';	

	Mat img1 = img.clone();
	Mat img2 = img.clone();
	Mat img3 = img.clone();
	Mat img4 = img.clone();
	Mat img5 = img.clone();

	GrayBordasHorizontais(img1); // 0
	GrayBordasVerticais(img2); //1
	GrayBordasDiagonaisPositivas(img3); //2
	GrayBordasDiagonaisNegativas(img4); //3
	img5 = 0.25*(img1 + img2 + img3 + img4);

	ofstream fout(filename);
	for (int n = 0; n < img.rows; n++) {
		for (int m = 0; m < img.cols; m++) {
			if (img5.at<uchar>(n, m) != 0) {
				int l = RetornoDoMaior(img1.at<uchar>(n, m), img2.at<uchar>(n, m), img3.at<uchar>(n, m), img4.at<uchar>(n, m));
				fout << ASCII[l];
			}
			else {
				b = img.at<uchar>(n, m) / 25;
				a = (int)b;			
				fout << ASCII2[a];
			}
		}
		fout << endl;
	}
	fout.close();
}

void writeMatToFile2(Mat img, const char* filename) {
	char ASCII[5];

	ASCII[4] = ' ';
	ASCII[3] = '/';
	ASCII[2] = '\\';
	ASCII[1] = '-';
	ASCII[0] = '|';

	Mat img1 = img.clone();
	Mat img2 = img.clone();
	Mat img3 = img.clone();
	Mat img4 = img.clone();

	GrayBordasHorizontais(img1); // 0
	GrayBordasVerticais(img2); //1
	GrayBordasDiagonaisPositivas(img3); //2
	GrayBordasDiagonaisNegativas(img4); //3

	ofstream fout(filename);
	for (int n = 0; n < img.rows; n++) {
		for (int m = 0; m < img.cols; m++) {
			int l = RetornoDoMaior(img1.at<uchar>(n,m), img2.at<uchar>(n, m), img3.at<uchar>(n, m), img4.at<uchar>(n, m));
			fout << ASCII[l];
		}
		fout << endl;
	}
	fout.close();
}

void writeMatToFile3(Mat img, const char* filename) {
	char ASCII[5];
	char ASCII2[10];
	int a;
	float b;

	Mat im = img.clone();
	GrayMediana(im, 5);

	ASCII[4] = ' ';
	ASCII[3] = '/';
	ASCII[2] = '\\';
	ASCII[1] = '-';
	ASCII[0] = '|';

	ASCII2[9] = ' ';
	ASCII2[8] = '.';
	ASCII2[7] = ',';
	ASCII2[6] = ':';
	ASCII2[5] = ';';
	ASCII2[4] = 'o';
	ASCII2[3] = 'x';
	ASCII2[2] = '%';
	ASCII2[1] = '#';
	ASCII2[0] = '@';

	Mat img1 = im.clone();
	Mat img2 = im.clone();
	Mat img3 = im.clone();
	Mat img4 = im.clone();
	Mat img5 = im.clone();

	GrayBordasHorizontais(img1); // 0
	GrayBordasVerticais(img2); //1
	GrayBordasDiagonaisPositivas(img3); //2
	GrayBordasDiagonaisNegativas(img4); //3
	img5 = 0.25*(img1 + img2 + img3 + img4);

	ofstream fout(filename);
	for (int n = 0; n < img.rows; n++) {
		for (int m = 0; m < img.cols; m++) {
			if (img5.at<uchar>(n, m) != 0) {
				int l = RetornoDoMaior(img1.at<uchar>(n, m), img2.at<uchar>(n, m), img3.at<uchar>(n, m), img4.at<uchar>(n, m));
				fout << ASCII[l];
			}
			else {
				b = img.at<uchar>(n, m) / 25;
				a = (int)b;
				fout << ASCII2[a];
			}
		}
		fout << endl;
	}
	fout.close();
}

void writeMatToFile4(Mat img, const char* filename) {
	char ASCII[5];

	ASCII[4] = ' ';
	ASCII[3] = '/';
	ASCII[2] = '\\';
	ASCII[1] = '-';
	ASCII[0] = '|';

	Mat im = img.clone();
	GrayMediana(im, 3);

	Mat img1 = im.clone();
	Mat img2 = im.clone();
	Mat img3 = im.clone();
	Mat img4 = im.clone();

	GrayBordasHorizontais(img1); // 0
	GrayBordasVerticais(img2); //1
	GrayBordasDiagonaisPositivas(img3); //2
	GrayBordasDiagonaisNegativas(img4); //3

	ofstream fout(filename);
	for (int n = 0; n < img.rows; n++) {
		for (int m = 0; m < img.cols; m++) {
			int l = RetornoDoMaior(img1.at<uchar>(n, m), img2.at<uchar>(n, m), img3.at<uchar>(n, m), img4.at<uchar>(n, m));
			fout << ASCII[l];
		}
		fout << endl;
	}
	fout.close();
}

#endif 