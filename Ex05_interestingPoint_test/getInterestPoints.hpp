//
//  getInterestPoints.hpp
//  test_findInterestPoint
//
//  Created by HeCHen2 on 2018/1/24.
//  Copyright © 2018年 Wanting. All rights reserved.
//

#ifndef getInterestPoints_hpp
#define getInterestPoints_hpp

#include "DirectionalGradients.hpp"

void getInterestPoints(Mat& img, double sigma, vector<KeyPoint>&points);
void showImage(Mat& img, const char* win, int wait, bool show, bool save);
Mat nonMaxSuppression(Mat& img);

#endif /* getInterestPoints_hpp */
