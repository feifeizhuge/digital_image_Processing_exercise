//
//  main.cpp
//  test_frequence_ex3
//
//  Created by Wanting on 2018/1/17.
//  Copyright © 2018年 Wanting. All rights reserved.
//

#include <iostream>
#include "GaussianSmooth.hpp"
#include "unsharpMasking.hpp"
#include "frequenceFilter.hpp"

int main(int argc, const char * argv[]) {
//*
//*   run for unsharp mask function
//*
//    Mat src = imread("original.jpg",0);
//    src.convertTo(src, CV_32FC1);
//    // check img is loaded
//    if(src.empty()){
//        cout << "Error! image is not loaded!" << endl;
//    }
//
//    Mat smooth_src = GaussianSmooth(src, 5);
//    Mat sharp_src = usm(src, 0, 5, 10, 2);
//    imwrite("smooth_img.jpg", smooth_src);
//    imwrite("sharped_img.jpg", sharp_src);
    
//*
//*   run for cirShift function
//*
//    Mat gaussianKernel = creatGaussianKernel(125) * 2000000;
//    imwrite("gaussianKernel_125.jpg", gaussianKernel);
//    Mat shift_kernel = circShift(gaussianKernel,-256,-256);
//    imwrite("shift_kernel.jpg", shift_kernel);
    
//*
//*   run for cirShift function
//*
//    Mat src = Mat::zeros(1024, 1024, CV_32FC1);
//    Mat gaussianKernel = creatGaussianKernel(128) * 2000000;
//    imwrite("gaussianKernel_125.jpg", gaussianKernel);
//    gaussianKernel.copyTo(src(Rect(0, 0, 128, 128)));
//    Mat shift_kernel = circShift(src,-64,-64);
//    imwrite("shift_kernel_1024.jpg", shift_kernel);
    
    
//*
//*   run for frequence convolution
//*
//    Mat src = imread("original.jpg",0);
//    src.convertTo(src, CV_32FC1);
//    // check img is loaded
//    if(src.empty()){
//    cout << "Error! image is not loaded!" << endl;
//    }
//
//    Mat kernel = creatGaussianKernel(11);
//    Mat result = frequencyConvolution(src, kernel);
//    Mat sharp_11 = usm(src, 0, 11, 10, 2);
//
//    imwrite("frequenceConvolution_img_11.jpg", result);
//    imwrite("sharp_img_11.jpg", sharp_11);
    
    Mat rec = Mat::zeros(512, 512, CV_32FC1);
    Mat rec1 = Mat::ones(64, 64, CV_32FC1) * 255;
    rec1.copyTo(rec(Rect(192, 192, 64, 64)));
    imwrite("rec.jpg", rec);
    imwrite("rec1.jpg", rec1);
    return 0;
    
}
