//
//  frequenceFilter.cpp
//  test_frequence_ex3
//
//  Created by Wanting on 2018/1/19.
//  Copyright © 2018年 Wanting. All rights reserved.
//

#include "frequenceFilter.hpp"

// Performes a circular shift in (dx,dy) direction
/*
 in       input matrix
 dx       shift in x-direction
 dy       shift in y-direction
 return   circular shifted matrix
 */
Mat circShift(Mat& in, int dx, int dy){
    
    // define dst Mat
    Mat shift = in.clone();
    // records the src's row and col
    int row = in.rows;
    int col = in.cols;
    // compute the actul shift value
    dx = dx % row;
    dy = dy % col;
    
    // define the shifted coordinate in the dst image
    int newx = 0;
    int newy = 0;
    
    for(int i=0; i<row; i++){
        for(int j=0; j<col; j++){
            // the map relationship
            newx = (i + dx) % row;
            newy = (j + dy) % col;
            
            // if the dx and dy is negative, it needs shifts the image
            newx = (newx<0) ? (newx + row) : newx;
            newy = (newy<0) ? (newy + col) : newy;
            
            shift.at<float>(newx,newy) = in.at<float>(i,j);
        }
    }
    return shift;
}

Mat frequencyConvolution(Mat& in, Mat& kernel){
    
    // create a same size Mat as dst for src
    Mat result = in.clone();
    Mat in_frequence = in.clone();
    Mat kernel_frequence = in.clone();
    Mat kernel_expand = Mat::zeros(in.rows, in.cols, CV_32FC1);
    
    // forward transform
    dft(in, in_frequence,0);
    // expand kernel to the same size with src
    kernel.copyTo(kernel_expand(Rect(0, 0, kernel.rows, kernel.cols)));
    // shift kernel into four corners
    kernel_expand = circShift(kernel_expand, - kernel.rows / 2, - kernel.cols / 2);
    dft(kernel_expand, kernel_frequence);
    
    // frequence convolution
    mulSpectrums(in_frequence, kernel_frequence, result, 0);
    
    // inserve transform
    dft(result,result, DFT_INVERSE + DFT_SCALE);
    
    return result;
}
