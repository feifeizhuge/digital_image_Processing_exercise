//
//  DirectionalGradients.hpp
//  test_findInterestPoint
//
//  Created by Wanting on 2018/1/20.
//  Copyright © 2018年 Wanting. All rights reserved.
//

#ifndef DirectionalGradients_hpp
#define DirectionalGradients_hpp

#include <iostream>
#include <opencv2/opencv.hpp>
#include<vector>

using namespace cv;
using namespace std;

Mat creatGradientsKernel_x(double sigma);
Mat creatGradientsKernel_y(double sigma);

#endif /* DirectionalGradients_hpp */
