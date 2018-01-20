//
//  unsharpMasking.cpp
//  test_frequence_ex3
//
//  Created by Wanting on 2018/1/17.
//  Copyright © 2018年 Wanting. All rights reserved.
//

#include "unsharpMasking.hpp"
#include "GaussianSmooth.hpp"

// Performs UnSharp Masking to enhance fine image structures
/*
 in       the input image
 type     integer defining how convolution for smoothing operation is done
 0 <==> spatial domain; 1 <==> frequency domain; 2 <==> seperable filter; 3 <==> integral image
 size     size of used smoothing kernel
 thresh   minimal intensity difference to perform operation
 scale    scaling of edge enhancement
 return   enhanced image
 */
Mat usm(Mat& in, int type, int size, double thresh, double scale){
    
    //*
    //* step one: get filted image( i(x,y) * Mk )
    //*
    // some temporary images
    Mat tmp(in.rows, in.cols, CV_32FC1);
    switch(type){
        case 0:
            tmp = GaussianSmooth(in, size);
            break;
    }
    //*
    //* step two: get difference image: d(x,y) = i(x,y) - g(x,y)
    //*
    // tmp is gaussian smoothed image, diff is the difference image
    Mat diff = in - tmp;
    
    //*
    //* step three: enhance image
    //*
    threshold( diff * scale, diff, thresh, 0, CV_THRESH_TOZERO);
    Mat result = diff + in;
    return result;
    
}
