
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

//-----------------------------------【全局变量声明部分】--------------------------------------  
//            描述：全局变量声明  
//-----------------------------------------------------------------------------------------------  
Mat g_srcImage, g_srcImage2, g_dstImage1,g_dstImage2,g_dstImage3,g_dstImage4,g_dstImage5, I, out;
int g_nBoxFilterValue=6;  //方框滤波内核值  
int g_nMeanBlurValue=10;  //均值滤波内核值  
int g_nGaussianBlurValue=6;  //高斯滤波内核值  
int g_nMedianBlurValue=10;  //中值滤波参数值  
int g_nBilateralFilterValue=10;  //双边滤波参数值  
   
   
//-----------------------------------【全局函数声明部分】--------------------------------------  
//            描述：全局函数声明  
//-----------------------------------------------------------------------------------------------  
//轨迹条回调函数  
static void on_BoxFilter(int, void *);            //方框滤波  
static void on_MeanBlur(int, void *);           //均值块滤波器  
static void on_GaussianBlur(int, void *);                    //高斯滤波器  
static void on_MedianBlur(int, void *);               //中值滤波器  
static void on_BilateralFilter(int, void*);                    //双边滤波器  
   
   
   
//-----------------------------------【main( )函数】--------------------------------------------  
//            描述：控制台应用程序的入口函数，我们的程序从这里开始  
//-----------------------------------------------------------------------------------------------  
int main(  )  
{  
       system("color 5E");   
   
       //载入原图  
       g_srcImage= imread( "momo.jpg", 1 );  
       if(!g_srcImage.data ) { printf("Oh，no，读取srcImage错误~！\n"); return false; }  
   
	   //添加噪音 二选一for test
	   //--------------椒盐噪音
	   //g_srcImage = addSaltNoise(g_srcImage, 3000);
	  // imwrite("H:/4...final/code-master/Chapter1_AndroidCartoonifier/Cartoonifier_Desktop/Cartoonifier_Desktop/salt.png", g_srcImage);
	   //--------------高斯噪音
	  // g_srcImage2 = g_srcImage.clone();
	   //g_srcImage2 = addGaussianNoise(g_srcImage2);
	   //imwrite("H:/4...final/code-master/Chapter1_AndroidCartoonifier/Cartoonifier_Desktop/Cartoonifier_Desktop/gaunoise.png", g_srcImage2);
	   //cvtColor(g_srcImage, g_srcImage, CV_BGR2GRAY);
	
       //克隆原图到四个Mat类型中  
       g_dstImage1= g_srcImage.clone( );  
       g_dstImage2= g_srcImage.clone( );  
       g_dstImage3= g_srcImage.clone( );  
       g_dstImage4= g_srcImage.clone( );  
       g_dstImage5= g_srcImage.clone( );  
   
       //显示原图  
       namedWindow("【<0>原图窗口】", 1);  
       imshow("【<0>原图窗口】",g_srcImage);  
   

	 //  I = imread("23.png",1);
	 //  out = anisodiff(I, 20, 20, 0.15, 1);
	//   imshow("【<0.5> 各项异性扩散】",out / 255);

   
       //=================【<1>方框滤波】=========================  
       //创建窗口  
       namedWindow("【<1>方框滤波】", 1);  
       //创建轨迹条  
       createTrackbar("内核值：", "【<1>方框滤波】",&g_nBoxFilterValue, 50,on_BoxFilter );  
       on_MeanBlur(g_nBoxFilterValue,0);  
       imshow("【<1>方框滤波】", g_dstImage1);  
       //=====================================================  
   
   
       //=================【<2>均值滤波】==========================  
       //创建窗口  
       namedWindow("【<2>均值滤波】", 1);  
       //创建轨迹条  
       createTrackbar("内核值：", "【<2>均值滤波】",&g_nMeanBlurValue, 50,on_MeanBlur );  
       on_MeanBlur(g_nMeanBlurValue,0);  
       //======================================================  
   
   
       //=================【<3>高斯滤波】===========================  
       //创建窗口  
       namedWindow("【<3>高斯滤波】", 1);  
       //创建轨迹条  
       createTrackbar("内核值：", "【<3>高斯滤波】",&g_nGaussianBlurValue, 50,on_GaussianBlur );  
       on_GaussianBlur(g_nGaussianBlurValue,0);  
       //=======================================================  
   
   
       //=================【<4>中值滤波】===========================  
       //创建窗口  
       namedWindow("【<4>中值滤波】", 1);  
       //创建轨迹条  
       createTrackbar("参数值：", "【<4>中值滤波】",&g_nMedianBlurValue, 50,on_MedianBlur );  
       on_MedianBlur(g_nMedianBlurValue,0);  
       //=======================================================  
   
   
       //=================【<5>双边滤波】===========================  
       //创建窗口  
       namedWindow("【<5>双边滤波】", 1);  
       //创建轨迹条  
       createTrackbar("参数值：", "【<5>双边滤波】",&g_nBilateralFilterValue, 50,on_BilateralFilter);  
       on_BilateralFilter(g_nBilateralFilterValue,0);  
       //=======================================================  
   
   
       //输出一些帮助信息  
       cout<<endl<<"\t嗯。好了，请调整滚动条观察图像效果~\n\n"  
              <<"\t按下“q”键时，程序退出~!\n"  
              <<"\n\n\t\t\t\tby浅墨";  
       while(char(waitKey(1))!= 'q') {}  
   

	   //----------------【保存图片】-----------------
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
   
//-----------------------------【on_BoxFilter( )函数】------------------------------------  
//            描述：方框滤波操作的回调函数  
//-----------------------------------------------------------------------------------------------  
static void on_BoxFilter(int, void *)  
{  
       //方框滤波操作  
       boxFilter(g_srcImage, g_dstImage1, -1,Size( g_nBoxFilterValue+1, g_nBoxFilterValue+1));  
       //显示窗口  
       imshow("【<1>方框滤波】", g_dstImage1);  
	   imwrite("H:/4...final/code-master/Chapter1_AndroidCartoonifier/Cartoonifier_Desktop/Cartoonifier_Desktop/box.png", g_dstImage1);
}  
   
//-----------------------------【on_MeanBlur( )函数】------------------------------------  
//            描述：均值滤波操作的回调函数  
//-----------------------------------------------------------------------------------------------  
static void on_MeanBlur(int, void *)  
{  
       blur(g_srcImage, g_dstImage2, Size( g_nMeanBlurValue+1, g_nMeanBlurValue+1),Point(-1,-1));  
       imshow("【<2>均值滤波】", g_dstImage2);  
	   imwrite("H:/4...final/code-master/Chapter1_AndroidCartoonifier/Cartoonifier_Desktop/Cartoonifier_Desktop/mean.png", g_dstImage2);
}  
   
//-----------------------------【on_GaussianBlur( )函数】------------------------------------  
//            描述：高斯滤波操作的回调函数  
//-----------------------------------------------------------------------------------------------  
static void on_GaussianBlur(int, void *)  
{  
       GaussianBlur(g_srcImage, g_dstImage3, Size( g_nGaussianBlurValue*2+1,g_nGaussianBlurValue*2+1 ), 0, 0);  
       imshow("【<3>高斯滤波】", g_dstImage3);  
	   imwrite("H:/4...final/code-master/Chapter1_AndroidCartoonifier/Cartoonifier_Desktop/Cartoonifier_Desktop/gau.png", g_dstImage3);
}  
   
   
//-----------------------------【on_MedianBlur( )函数】------------------------------------  
//            描述：中值滤波操作的回调函数  
//-----------------------------------------------------------------------------------------------  
static void on_MedianBlur(int, void *)  
{  
       medianBlur( g_srcImage, g_dstImage4, g_nMedianBlurValue*2+1 );  
       imshow("【<4>中值滤波】", g_dstImage4);  
	   imwrite("H:/4...final/code-master/Chapter1_AndroidCartoonifier/Cartoonifier_Desktop/Cartoonifier_Desktop/median.png", g_dstImage4);
}  
   
   
//-----------------------------【on_BilateralFilter( )函数】------------------------------------  
//            描述：双边滤波操作的回调函数  
//-----------------------------------------------------------------------------------------------  
static void on_BilateralFilter(int, void *)  
{  
       bilateralFilter( g_srcImage, g_dstImage5, g_nBilateralFilterValue, g_nBilateralFilterValue*2,g_nBilateralFilterValue/2 );  
       imshow("【<5>双边滤波】", g_dstImage5);  
	   imwrite("H:/4...final/code-master/Chapter1_AndroidCartoonifier/Cartoonifier_Desktop/Cartoonifier_Desktop/bil.png", g_dstImage5);
}  

//----------------------------------【椒盐噪音】-----------------------------------------
//-----------------------------------------------------------------------------------------
Mat addSaltNoise(const Mat srcImage, int n)
{
	Mat dstImage = srcImage.clone();
	for (int k = 0; k < n; k++)
	{
		//随机取值行列  
		int i = rand() % dstImage.rows;
		int j = rand() % dstImage.cols;
		//图像通道判定  
		if (dstImage.channels() == 1)
		{
			dstImage.at<uchar>(i, j) = 255;       //盐噪声  
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
		//随机取值行列  
		int i = rand() % dstImage.rows;
		int j = rand() % dstImage.cols;
		//图像通道判定  
		if (dstImage.channels() == 1)
		{
			dstImage.at<uchar>(i, j) = 0;     //椒噪声  
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

//----------------------------【生成高斯噪声】-------------------------------  
double generateGaussianNoise(double mu, double sigma)
{
	//定义小值  
	const double epsilon = numeric_limits<double>::min();
	static double z0, z1;
	static bool flag = false;
	flag = !flag;
	//flag为假构造高斯随机变量X  
	if (!flag)
		return z1 * sigma + mu;
	double u1, u2;
	//构造随机变量  
	do
	{
		u1 = rand() * (1.0 / RAND_MAX);
		u2 = rand() * (1.0 / RAND_MAX);
	} while (u1 <= epsilon);
	//flag为真构造高斯随机变量  
	z0 = sqrt(-2.0*log(u1))*cos(2 * CV_PI*u2);
	z1 = sqrt(-2.0*log(u1))*sin(2 * CV_PI*u2);
	return z0*sigma + mu;
}

//为图像添加高斯噪声  
Mat addGaussianNoise(Mat &srcImag)
{
	Mat dstImage = srcImag.clone();
	int channels = dstImage.channels();
	int rowsNumber = dstImage.rows;
	int colsNumber = dstImage.cols*channels;
	//判断图像的连续性  
	if (dstImage.isContinuous())
	{
		colsNumber *= rowsNumber;
		rowsNumber = 1;
	}
	for (int i = 0; i < rowsNumber; i++)
	{
		for (int j = 0; j < colsNumber; j++)
		{
			//添加高斯噪声  
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