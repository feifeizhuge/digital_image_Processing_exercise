//
//  frequenceFilter.hpp
//  test_frequence_ex3
//
//  Created by Wanting on 2018/1/19.
//  Copyright © 2018年 Wanting. All rights reserved.
//

#ifndef frequenceFilter_hpp
#define frequenceFilter_hpp

#include <opencv2/opencv.hpp>
#include <istream>
using namespace std;
using namespace cv;

Mat circShift(Mat& in, int dx, int dy);
Mat frequencyConvolution(Mat& in, Mat& kernel);

#endif /* frequenceFilter_hpp */
