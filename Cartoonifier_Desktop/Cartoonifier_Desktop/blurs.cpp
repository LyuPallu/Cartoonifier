
//#include "StdAfx.h"
#include <stdio.h>
#include "stdlib.h"
#include <tchar.h>
//#include <cv.h>
#include "C:/opencv/build/include/opencv/cv.h"
#include "C:/opencv/build/include/opencv2/opencv.hpp"
#include <C:/opencv/build/include/opencv/highgui.h>
#include <C:/opencv/build/include/opencv2/highgui.hpp>
#include <math.h>

#include "cartoon.h"
#include "ImageUtils.h"

using namespace std;  
using namespace cv;  

Mat addSaltNoise(const Mat srcImage, int n);
double generateGaussianNoise(double m, double sigma);
Mat addGaussianNoise(Mat &srcImag);

//-----------------------------------��ȫ�ֱ����������֡�--------------------------------------  
//            ������ȫ�ֱ�������  
//-----------------------------------------------------------------------------------------------  
Mat g_srcImage, g_srcImage2, g_dstImage1,g_dstImage2,g_dstImage3,g_dstImage4,g_dstImage5, I, out;
int g_nBoxFilterValue=6;  //�����˲��ں�ֵ  
int g_nMeanBlurValue=10;  //��ֵ�˲��ں�ֵ  
int g_nGaussianBlurValue=6;  //��˹�˲��ں�ֵ  
int g_nMedianBlurValue=10;  //��ֵ�˲�����ֵ  
int g_nBilateralFilterValue=10;  //˫���˲�����ֵ  
   
   
//-----------------------------------��ȫ�ֺ����������֡�--------------------------------------  
//            ������ȫ�ֺ�������  
//-----------------------------------------------------------------------------------------------  
//�켣���ص�����  
static void on_BoxFilter(int, void *);            //�����˲�  
static void on_MeanBlur(int, void *);           //��ֵ���˲���  
static void on_GaussianBlur(int, void *);                    //��˹�˲���  
static void on_MedianBlur(int, void *);               //��ֵ�˲���  
static void on_BilateralFilter(int, void*);                    //˫���˲���  
   
   
   
//-----------------------------------��main( )������--------------------------------------------  
//            ����������̨Ӧ�ó������ں��������ǵĳ�������￪ʼ  
//-----------------------------------------------------------------------------------------------  
int main(  )  
{  
       system("color 5E");   
   
       //����ԭͼ  
       g_srcImage= imread( "momo.jpg", 1 );  
       if(!g_srcImage.data ) { printf("Oh��no����ȡsrcImage����~��\n"); return false; }  
   
	   //������� ��ѡһfor test
	   //--------------��������
	   //g_srcImage = addSaltNoise(g_srcImage, 3000);
	  // imwrite("H:/4...final/code-master/Chapter1_AndroidCartoonifier/Cartoonifier_Desktop/Cartoonifier_Desktop/salt.png", g_srcImage);
	   //--------------��˹����
	  // g_srcImage2 = g_srcImage.clone();
	   //g_srcImage2 = addGaussianNoise(g_srcImage2);
	   //imwrite("H:/4...final/code-master/Chapter1_AndroidCartoonifier/Cartoonifier_Desktop/Cartoonifier_Desktop/gaunoise.png", g_srcImage2);
	   //cvtColor(g_srcImage, g_srcImage, CV_BGR2GRAY);
	
       //��¡ԭͼ���ĸ�Mat������  
       g_dstImage1= g_srcImage.clone( );  
       g_dstImage2= g_srcImage.clone( );  
       g_dstImage3= g_srcImage.clone( );  
       g_dstImage4= g_srcImage.clone( );  
       g_dstImage5= g_srcImage.clone( );  
   
       //��ʾԭͼ  
       namedWindow("��<0>ԭͼ���ڡ�", 1);  
       imshow("��<0>ԭͼ���ڡ�",g_srcImage);  
   

	 //  I = imread("23.png",1);
	 //  out = anisodiff(I, 20, 20, 0.15, 1);
	//   imshow("��<0.5> ����������ɢ��",out / 255);

   
       //=================��<1>�����˲���=========================  
       //��������  
       namedWindow("��<1>�����˲���", 1);  
       //�����켣��  
       createTrackbar("�ں�ֵ��", "��<1>�����˲���",&g_nBoxFilterValue, 50,on_BoxFilter );  
       on_MeanBlur(g_nBoxFilterValue,0);  
       imshow("��<1>�����˲���", g_dstImage1);  
       //=====================================================  
   
   
       //=================��<2>��ֵ�˲���==========================  
       //��������  
       namedWindow("��<2>��ֵ�˲���", 1);  
       //�����켣��  
       createTrackbar("�ں�ֵ��", "��<2>��ֵ�˲���",&g_nMeanBlurValue, 50,on_MeanBlur );  
       on_MeanBlur(g_nMeanBlurValue,0);  
       //======================================================  
   
   
       //=================��<3>��˹�˲���===========================  
       //��������  
       namedWindow("��<3>��˹�˲���", 1);  
       //�����켣��  
       createTrackbar("�ں�ֵ��", "��<3>��˹�˲���",&g_nGaussianBlurValue, 50,on_GaussianBlur );  
       on_GaussianBlur(g_nGaussianBlurValue,0);  
       //=======================================================  
   
   
       //=================��<4>��ֵ�˲���===========================  
       //��������  
       namedWindow("��<4>��ֵ�˲���", 1);  
       //�����켣��  
       createTrackbar("����ֵ��", "��<4>��ֵ�˲���",&g_nMedianBlurValue, 50,on_MedianBlur );  
       on_MedianBlur(g_nMedianBlurValue,0);  
       //=======================================================  
   
   
       //=================��<5>˫���˲���===========================  
       //��������  
       namedWindow("��<5>˫���˲���", 1);  
       //�����켣��  
       createTrackbar("����ֵ��", "��<5>˫���˲���",&g_nBilateralFilterValue, 50,on_BilateralFilter);  
       on_BilateralFilter(g_nBilateralFilterValue,0);  
       //=======================================================  
   
   
       //���һЩ������Ϣ  
       cout<<endl<<"\t�š����ˣ�������������۲�ͼ��Ч��~\n\n"  
              <<"\t���¡�q����ʱ�������˳�~!\n"  
              <<"\n\n\t\t\t\tbyǳī";  
       while(char(waitKey(1))!= 'q') {}  
   

	   //----------------������ͼƬ��-----------------
	  /**
	   imwrite("mean.png", g_dstImage2);
	   imwrite("gau.png", g_dstImage3);
	   imwrite("median.png", g_dstImage4);
	   imwrite("bil.png", g_dstImage5);
	   **/

	   //cvWaitKey(0);
	   system("pause");
	   return 0;
         
}  
   
//-----------------------------��on_BoxFilter( )������------------------------------------  
//            �����������˲������Ļص�����  
//-----------------------------------------------------------------------------------------------  
static void on_BoxFilter(int, void *)  
{  
       //�����˲�����  
       boxFilter(g_srcImage, g_dstImage1, -1,Size( g_nBoxFilterValue+1, g_nBoxFilterValue+1));  
       //��ʾ����  
       imshow("��<1>�����˲���", g_dstImage1);  
	   imwrite("H:/4...final/code-master/Chapter1_AndroidCartoonifier/Cartoonifier_Desktop/Cartoonifier_Desktop/box.png", g_dstImage1);
}  
   
//-----------------------------��on_MeanBlur( )������------------------------------------  
//            ��������ֵ�˲������Ļص�����  
//-----------------------------------------------------------------------------------------------  
static void on_MeanBlur(int, void *)  
{  
       blur(g_srcImage, g_dstImage2, Size( g_nMeanBlurValue+1, g_nMeanBlurValue+1),Point(-1,-1));  
       imshow("��<2>��ֵ�˲���", g_dstImage2);  
	   imwrite("H:/4...final/code-master/Chapter1_AndroidCartoonifier/Cartoonifier_Desktop/Cartoonifier_Desktop/mean.png", g_dstImage2);
}  
   
//-----------------------------��on_GaussianBlur( )������------------------------------------  
//            ��������˹�˲������Ļص�����  
//-----------------------------------------------------------------------------------------------  
static void on_GaussianBlur(int, void *)  
{  
       GaussianBlur(g_srcImage, g_dstImage3, Size( g_nGaussianBlurValue*2+1,g_nGaussianBlurValue*2+1 ), 0, 0);  
       imshow("��<3>��˹�˲���", g_dstImage3);  
	   imwrite("H:/4...final/code-master/Chapter1_AndroidCartoonifier/Cartoonifier_Desktop/Cartoonifier_Desktop/gau.png", g_dstImage3);
}  
   
   
//-----------------------------��on_MedianBlur( )������------------------------------------  
//            ��������ֵ�˲������Ļص�����  
//-----------------------------------------------------------------------------------------------  
static void on_MedianBlur(int, void *)  
{  
       medianBlur( g_srcImage, g_dstImage4, g_nMedianBlurValue*2+1 );  
       imshow("��<4>��ֵ�˲���", g_dstImage4);  
	   imwrite("H:/4...final/code-master/Chapter1_AndroidCartoonifier/Cartoonifier_Desktop/Cartoonifier_Desktop/median.png", g_dstImage4);
}  
   
   
//-----------------------------��on_BilateralFilter( )������------------------------------------  
//            ������˫���˲������Ļص�����  
//-----------------------------------------------------------------------------------------------  
static void on_BilateralFilter(int, void *)  
{  
       bilateralFilter( g_srcImage, g_dstImage5, g_nBilateralFilterValue, g_nBilateralFilterValue*2,g_nBilateralFilterValue/2 );  
       imshow("��<5>˫���˲���", g_dstImage5);  
	   imwrite("H:/4...final/code-master/Chapter1_AndroidCartoonifier/Cartoonifier_Desktop/Cartoonifier_Desktop/bil.png", g_dstImage5);
}  

//----------------------------------������������-----------------------------------------
//-----------------------------------------------------------------------------------------
Mat addSaltNoise(const Mat srcImage, int n)
{
	Mat dstImage = srcImage.clone();
	for (int k = 0; k < n; k++)
	{
		//���ȡֵ����  
		int i = rand() % dstImage.rows;
		int j = rand() % dstImage.cols;
		//ͼ��ͨ���ж�  
		if (dstImage.channels() == 1)
		{
			dstImage.at<uchar>(i, j) = 255;       //������  
		}
		else
		{
			dstImage.at<Vec3b>(i, j)[0] = 255;
			dstImage.at<Vec3b>(i, j)[1] = 255;
			dstImage.at<Vec3b>(i, j)[2] = 255;
		}
	}
	for (int k = 0; k < n; k++)
	{
		//���ȡֵ����  
		int i = rand() % dstImage.rows;
		int j = rand() % dstImage.cols;
		//ͼ��ͨ���ж�  
		if (dstImage.channels() == 1)
		{
			dstImage.at<uchar>(i, j) = 0;     //������  
		}
		else
		{
			dstImage.at<Vec3b>(i, j)[0] = 0;
			dstImage.at<Vec3b>(i, j)[1] = 0;
			dstImage.at<Vec3b>(i, j)[2] = 0;
		}
	}
	return dstImage;
}

//----------------------------�����ɸ�˹������-------------------------------  
double generateGaussianNoise(double mu, double sigma)
{
	//����Сֵ  
	const double epsilon = numeric_limits<double>::min();
	static double z0, z1;
	static bool flag = false;
	flag = !flag;
	//flagΪ�ٹ����˹�������X  
	if (!flag)
		return z1 * sigma + mu;
	double u1, u2;
	//�����������  
	do
	{
		u1 = rand() * (1.0 / RAND_MAX);
		u2 = rand() * (1.0 / RAND_MAX);
	} while (u1 <= epsilon);
	//flagΪ�湹���˹�������  
	z0 = sqrt(-2.0*log(u1))*cos(2 * CV_PI*u2);
	z1 = sqrt(-2.0*log(u1))*sin(2 * CV_PI*u2);
	return z0*sigma + mu;
}

//Ϊͼ����Ӹ�˹����  
Mat addGaussianNoise(Mat &srcImag)
{
	Mat dstImage = srcImag.clone();
	int channels = dstImage.channels();
	int rowsNumber = dstImage.rows;
	int colsNumber = dstImage.cols*channels;
	//�ж�ͼ���������  
	if (dstImage.isContinuous())
	{
		colsNumber *= rowsNumber;
		rowsNumber = 1;
	}
	for (int i = 0; i < rowsNumber; i++)
	{
		for (int j = 0; j < colsNumber; j++)
		{
			//��Ӹ�˹����  
			int val = dstImage.ptr<uchar>(i)[j] +
				generateGaussianNoise(2, 0.8) * 32;
			if (val < 0)
				val = 0;
			if (val>255)
				val = 255;
			dstImage.ptr<uchar>(i)[j] = (uchar)val;
		}
	}
	return dstImage;
}