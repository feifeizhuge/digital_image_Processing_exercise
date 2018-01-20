//
//  GaussianSmooth.hpp
//  test_frequence_ex3
//
//  Created by Wanting on 2018/1/17.
//  Copyright © 2018年 Wanting. All rights reserved.
//

#ifndef GaussianSmooth_hpp
#define GaussianSmooth_hpp

#include <opencv2/opencv.hpp>
#include <istream>
using namespace std;
using namespace cv;

Mat spatialConvolution(Mat& src, Mat& kernel);
Mat GaussianSmooth(Mat& in, int size);
Mat creatGaussianKernel(int KSize);

#endif /* GaussianSmooth_hpp */
