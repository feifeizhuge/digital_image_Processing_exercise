//
//  DirectionalGradients.cpp
//  test_findInterestPoint
//
//  Created by Wanting on 2018/1/20.
//  Copyright © 2018年 Wanting. All rights reserved.
//

#include "DirectionalGradients.hpp"

Mat creatGradientsKernel_x(double sigma){
    // define Pi constant
    double const PI = 3.14159;
    // define sum of kernel's points
    double sum = 0;
    // define Gaussian sigma, 经验的
    int KSize = 3 ;
    
    Mat result = Mat::zeros(KSize, KSize, CV_32FC1);
    int centre = KSize / 2;
    
    for(int i=0; i<KSize; i++){
        for(int j=0; j<KSize; j++){
            int x = i - centre; int y = j - centre;
            double scale = 1 / ( 2 * PI * sigma * sigma);
            double e = -0.5 * (pow(x/sigma, 2.0) + pow(y/sigma, 2.0));
            result.at<float>(i,j) = - x * scale * exp(e) / pow(sigma, 2.0) ;
            sum += abs(result.at<float>(i,j));
        }
    }
    return result / sum ;
}

Mat creatGradientsKernel_y(double sigma){
    // define Pi constant
    double const PI = 3.14159;
    // define sum of kernel's points
    double sum = 0;
    // define Gaussian sigma, 经验的
    int KSize = 3;
    
    Mat result = Mat::zeros(KSize, KSize, CV_32FC1);
    int centre = KSize / 2;
    
    for(int i=0; i<KSize; i++){
        for(int j=0; j<KSize; j++){
            int x = i - centre; int y = j - centre;
            double scale = 1 / ( 2 * PI * sigma * sigma);
            double e = -0.5 * (pow(x/sigma, 2.0) + pow(y/sigma, 2.0));
            result.at<float>(i,j) = - y * scale * exp(e) / pow(sigma, 2.0) ;
            sum += abs(result.at<float>(i,j));
        }
    }
    return result / sum ;
}
