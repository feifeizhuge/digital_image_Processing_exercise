//
//  spatialFilter.hpp
//  cv_convolution
//
//  Created by Wanting on 2018/1/5.
//  Copyright © 2018年 Wanting. All rights reserved.
//
//  写成class的形式，可以定义多个函数在一个类里面，这样不用每个函数，都写一个cpp和hpp，很方便
#ifndef spatialFilter_hpp
#define spatialFilter_hpp

#include <iostream>
#include <algorithm>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class spatialFilter{
    public:
    //constructor
    spatialFilter(void){};
    //destructor
    ~spatialFilter(void){};
    //function run
    void run(void);
    
    private:
    
    Mat spatialConvolution(Mat& src, Mat& kernel);
    Mat averageFilter(Mat& src, int kSize);
    Mat laplaceFilter(Mat& src);
    Mat medianFilter(Mat& src, int kSize);
    Mat bilateralFilter(Mat& src, int kSize, double sigma);
    
};

#endif /* spatialFilter_hpp */
