//
//  GaussianSmooth.cpp
//  test_frequence_ex3
//
//  Created by Wanting on 2018/1/17.
//  Copyright © 2018年 Wanting. All rights reserved.
//

#include "GaussianSmooth.hpp"

// use Gaussian Kernel to smooth the image
// this function contains two functions:
//                          1) creatGaussianKernel
//                          2) spatialconvolution
// put GaussianKernel into spatialconvolution, then get the smoothed image


Mat GaussianSmooth(Mat& in, int size){
    
    
    Mat kernel = creatGaussianKernel(size);
    Mat result = spatialConvolution(in, kernel);
    return result;
}

Mat creatGaussianKernel(int KSize){
    // define Pi constant
    double const PI = 3.14159;
    // define sum of kernel's points
    double sum = 0;
    // define Gaussian sigma, 经验的
    double sigma = KSize / 5;
    
    Mat result = Mat::zeros(KSize, KSize, CV_32FC1);
    int centre = KSize / 2;
    
    for(int i=0; i<KSize; i++){
        for(int j=0; j<KSize; j++){
            int x = i - centre; int y = j - centre;
            double scale = 1 / ( 2 * PI * sigma * sigma);
            double e = -0.5 * (pow(x/sigma, 2.0) + pow(y/sigma, 2.0));
            result.at<float>(i,j) = scale * exp(e) ;
            sum += result.at<float>(i,j);
        }
    }
    return result / sum;
}

Mat spatialConvolution(Mat& src, Mat& kernel){
    
    Mat flip_kernel(kernel.rows, kernel.cols, CV_32FC1);//32 bit float bits and channel one
    Mat result = Mat::zeros(src.rows, src.cols, CV_32FC1);
    //src.convertTo(src, CV_32FC1);
    //Mat result = src;
    
    //******step-one: Flip filter kernel****************
    
    //the second way
    for (int i=0; i<kernel.rows; i++){
        for (int j=0; j<kernel.cols; j++){
            flip_kernel.at<float>(i,j) = kernel.at<float>(kernel.rows-i-1,kernel.cols-j-1);
        }
    }
    
    //the first way
//    flip(kernel, flip_kernel, -1);
    
    //*******step-two: Shift(re-centre), Multiply and Integrate
    //the first way to void that kernel'size if out of img
    int bound = kernel.rows/2;
    int row = 0;
    int col = 0;
    
    for(int i=bound; i<src.rows-bound; i++){
        for(int j=bound; j<src.cols-bound; j++){
            //float sum = 0.0;
            for(int k=0; k<kernel.rows; k++){
                for(int l=0; l<kernel.cols; l++){
                    row = -bound+k+i;
                    col = -bound+l+j;
                    result.at<float>(i,j) += flip_kernel.at<float>(k,l)*src.at<float>(row,col);
                    //sum += flip_kernel.at<float>(k,l)*src.at<float>(row,col);
                    //result.at<float>(i,j) = sum;
                }
            }
        }
    }
    
    return result;
}
