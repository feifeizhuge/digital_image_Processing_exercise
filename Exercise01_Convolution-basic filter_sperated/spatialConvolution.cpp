//
//  spatialConvolution.cpp
//  cv_convolution
//
//  Created by Wanting on 2018/1/3.
//  Copyright © 2018年 Wanting. All rights reserved.
//

#include "spatialConvolution.hpp"


// convolution in spatial domain
/*
 src:     input image
 kernel:  filter kernel
 return:  convolution result
*/
Mat spatialConvolution(Mat& src, Mat& kernel){
    
    Mat flip_kernel(kernel.rows, kernel.cols, CV_32FC1);//32 bit float bits and channel one
    Mat result = Mat::zeros(src.rows, src.cols, CV_32FC1);
    //src.convertTo(src, CV_32FC1);
    //Mat result = src;

    //******step-one: Flip filter kernel****************
        //the first way
    flip(kernel, flip_kernel, -1);
        //the second way
    for (int i=0; i<kernel.rows; i++){
        for (int j=0; j<kernel.cols; j++){
            flip_kernel.at<float>(i,j) = kernel.at<float>(kernel.rows-i-1,kernel.cols-j-1);
        }
    }
    
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
