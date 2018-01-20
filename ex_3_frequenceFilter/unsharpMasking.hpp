//
//  unsharpMasking.hpp
//  test_frequence_ex3
//
//  Created by Wanting on 2018/1/17.
//  Copyright © 2018年 Wanting. All rights reserved.
//

#ifndef unsharpMasking_hpp
#define unsharpMasking_hpp

#include <opencv2/opencv.hpp>
#include <istream>
using namespace std;
using namespace cv;


Mat usm(Mat& in, int type, int size, double thresh, double scale);
Mat spatialConvolution(Mat& in, Mat& kernel);
#endif /* unsharpMasking_hpp */
