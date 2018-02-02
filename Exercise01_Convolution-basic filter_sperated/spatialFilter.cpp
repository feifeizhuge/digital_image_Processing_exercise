//
//  spatialFilter.cpp
//  cv_convolution
//
//  Created by Wanting on 2018/1/5.
//  Copyright © 2018年 Wanting. All rights reserved.
//

#include "spatialFilter.hpp"
#include <iostream>
#include <algorithm>

// convolution in spatial domain
/*
 src:     input image
 kernel:  filter kernel
 return:  convolution result
 */
Mat spatialFilter::spatialConvolution(Mat& src, Mat& kernel){
    
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
    flip(kernel, flip_kernel, -1);
    
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

/*
************************************************************************************
 apply average filter to get smoothed image
 src:     input image
 kSize:   window size used by local average
 return:  filtered image
************************************************************************************
 */

Mat spatialFilter::averageFilter(Mat& src, int kSize){
    //creat a average kernel with k-size, k is a odd number
    Mat average_kernel = Mat(kSize,kSize,CV_32FC1,1./(kSize*kSize));
    Mat result = Mat::zeros(src.rows, src.cols, CV_32FC1);
    
    result = spatialConvolution(src,average_kernel);
    
    return result;
}

/*
 ************************************************************************************
 apply laplace kernel to get edge
 ************************************************************************************
 */

Mat spatialFilter::laplaceFilter(Mat& src){
    
    Mat laplace_kernel = ( Mat_<float>(3,3)<<0,1,0,1,-4,1,0,1,0 );
    Mat result = Mat::zeros(src.rows, src.cols, CV_32FC1);
    
    result = spatialConvolution(src,laplace_kernel);
    return result;
}
/*
 ************************************************************************************
 apply median to suppression the shot noise
 ************************************************************************************
 */
Mat spatialFilter::medianFilter(Mat& src, int kSize){
    
    Mat result = Mat::zeros(src.rows, src.cols, CV_32FC1);
    int bound = kSize/2;
    int size = kSize*kSize;
    int row = 0;
    int col = 0;

    //vector<float> kernel_vector = {};
    
    for(int i=bound; i<src.rows-bound; i++){
        for(int j=bound; j<src.cols-bound; j++){
            
            vector<float> kernel_vector = {};
            for(int k=0; k<kSize; k++){
                for(int l=0; l<kSize; l++){
                    row = -bound+k+i;
                    col = -bound+l+j;
                    kernel_vector.push_back(src.at<float>(row,col));
                }
            }
        std::sort (kernel_vector.begin(),kernel_vector.end());
        result.at<float>(i,j) = kernel_vector[size/2];
        }
    }
    
    return result;
    
}

/*
************************************************************************************
 the bilateral filter

 src:     input image
 kSize:   size of the kernel --> used to compute std-dev of spatial kernel
 sigma:   standard-deviation of the radiometric kernel
 return:  filtered image
************************************************************************************
 */

Mat spatialFilter::bilateralFilter(Mat& src, int kSize, double sigma){
    
    Mat result = Mat::zeros(src.rows, src.cols, CV_32FC1);
    int size = kSize*kSize;
    int index = 0;
    double spatialTable[size];
    double sigma1 = kSize/5;
    double spatialWeight;
    double radioweight;
    double diff;
    int bound = kSize/2;
    int row = 0;
    int col = 0;
    
//creat a spatial gauss kernel table
    for(int i=kSize/2; i<-kSize/2+1; i--){
        for (int j=kSize/2; j<-kSize/2+1; j--){
            spatialTable[index] = exp(-(i*i+j*j)/2*sigma1*sigma1) * 500000;
            index++;
        }
    }
    
//calculate the point value
    
    for(int i=bound; i<src.rows-bound; i++){
        for(int j=bound; j<src.cols-bound; j++){
            int count = 0;
            double sum_index = 0;
            double sum_value = 0;
            vector<float> kernel_vector = {};
            for(int k=0; k<kSize; k++){
                for(int l=0; l<kSize; l++){
                    row = -bound+k+i;
                    col = -bound+l+j;
                    diff = src.at<float>(i,j)-src.at<float>(row,col);
                    count ++;
                    spatialWeight = spatialTable[count];
                    radioweight = exp(-(diff*diff)/(2*sigma*sigma)) * 500000;
                    sum_index += radioweight * spatialWeight ;
                    sum_value += src.at<float>(row,col) * radioweight * spatialWeight;
                    if(count >= size-1){
                        result.at<float>(i,j) = sum_value / sum_index ;
                        }
                    }
                }
            }
        }
    
    return result ;
    
}
// function loads input image, calls processing function, and saves result

void spatialFilter::run(void){
    
    cout << "loading images" << endl;
    Mat src = imread("./original.jpg",0);
    Mat src_shot = imread("./noiseType_1.jpg",0);
    Mat src_gauss = imread("./noiseType_2.jpg",0);
    Mat src_baboon = imread("mandril_color.tif",0);
    src.convertTo(src, CV_32FC1);
    src_shot.convertTo(src_shot, CV_32FC1);
    src_gauss.convertTo(src_gauss, CV_32FC1);
    src_baboon.convertTo(src_baboon, CV_32FC1);
    // check whether the img is loaded
    if( src.empty()&src_shot.empty()&src_gauss.empty() ) {
        cout << "Error! not load img" << endl;
    }
    else{
        cout << "Done! images are loaded" << endl;
        cout << "Press enter to continue"  << endl;
        cin.get();
    }
    
    Mat result_stand;
    Mat result_average = averageFilter(src,3);
    Mat result_laplace = laplaceFilter(src);
    Mat result_median = medianFilter(src_shot,5);
    Mat result_bilateral = bilateralFilter(src,3,1);
    //cv::bilateralFilter(src, result_stand, 9, 100, 7);
    imwrite("restorated_average.jpg", result_average);
    imwrite("restorated_laplace.jpg", result_laplace);
    imwrite("restorated_median.jpg", result_median);
    imwrite("restorated_bilateral.jpg", result_bilateral);
    //imwrite("restorated_bilateral_stand.jpg", result_stand);
    cout << "Done!" << endl;
    
}

