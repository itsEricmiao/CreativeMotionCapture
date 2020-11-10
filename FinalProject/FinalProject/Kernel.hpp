////
////  Kernel.h
////  CV_Project1
////
////  Created by Eric Miao on 10/10/20.
////
//
//#ifndef Kernel_h
//#define Kernel_h
//
//#include <opencv2/core/core.hpp>
//#include "opencv2/imgproc/imgproc.hpp"
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc.hpp>
//#include <stdio.h>
//#include <iostream>
//
//using namespace cv;
//using namespace std;
//Mat getKernel( int n, double sigma, int ktype )
//{
//    const int SMALL_GAUSSIAN_SIZE = 7;
//    static const float small_gaussian_tab[][SMALL_GAUSSIAN_SIZE] =
//    {
//        {1.f},
//        {0.25f, 0.5f, 0.25f},
//        {0.0625f, 0.25f, 0.375f, 0.25f, 0.0625f},
//        {0.03125f, 0.109375f, 0.21875f, 0.28125f, 0.21875f, 0.109375f, 0.03125f}
//    };
//
//    const float* fixed_kernel = n % 2 == 1 && n <= SMALL_GAUSSIAN_SIZE && sigma <= 0 ?
//        small_gaussian_tab[n>>1] : 0;
//
//    CV_Assert( ktype == CV_32F || ktype == CV_64F );
//    Mat kernel(n, 1, ktype);
//    float* cf = (float*)kernel.data;
//    double* cd = (double*)kernel.data;
//
//    double sigmaX = sigma > 0 ? sigma : ((n-1)*0.5 - 1)*0.3 + 0.8;
//    double scale2X = -0.5/(sigmaX*sigmaX);
//    double sum = 0;
//
//    int i;
//    for( i = 0; i < n; i++ )
//    {
//        double x = i - (n-1)*0.5;
//        double t = fixed_kernel ? (double)fixed_kernel[i] : std::exp(scale2X*x*x);
//        if( ktype == CV_32F )
//        {
//            cf[i] = (float)t;
//            sum += cf[i];
//        }
//        else
//        {
//            cd[i] = t;
//            sum += cd[i];
//        }
//    }
//
//    sum = 1./sum;
//    for( i = 0; i < n; i++ )
//    {
//        if( ktype == CV_32F )
//            cf[i] = (float)(cf[i]*sum);
//        else
//            cd[i] *= sum;
//    }
//    return kernel;
//}
//
//
//#endif /* Kernel_h */
