//
//  main.cpp
//  test_findInterestPoint
//
//  Created by Wanting on 2018/1/20.
//  Copyright © 2018年 Wanting. All rights reserved.
//

#include <iostream>
#include "DirectionalGradients.hpp"
#include "getInterestPoints.hpp"

int main(int argc, const char * argv[]) {
    
//*
//* check your gradient image
//*
    Mat gradients_x = creatGradientsKernel_x(0.5);
    Mat gradients_y = creatGradientsKernel_y(0.5);
//    for(int i=0;i<gradients_x.rows;i++){
//        for(int j=0;j<gradients_x.cols;j++){
//            cout << gradients_x.at<float>(i,j) <<endl;
//        }
//    }
//    Mat src = imread("rect.png");
//    Mat src_x = src.clone();
//    Mat src_y = src.clone();
//    Mat src_xy = src.clone();
//    if(src.empty()){
//        cout << "ERROR! not load the image" << endl;
//    }
//
//    filter2D(src, src_x,src.depth(), gradients_x);
//    filter2D(src, src_y,src.depth(), gradients_y);
//    src_xy = src_x.mul(src_y);
//    imwrite("gradients_x.jpg", src_x);
//    imwrite("gradients_y.jpg", src_y);
//    imwrite("gradients_xy.jpg", src_xy);
//
//    Mat src = imread("chat.JPG",0);
//    src.convertTo(src, CV_32FC1);
//    if (src.empty()){
//        cout << "EEROR! not loaded image" << endl;
//    }
//    vector<KeyPoint> points = {};
//    getInterestPoints(src,0.5,points);

//    Mat src = Mat::zeros(512, 512, CV_8UC1);
////    line(src, Point(60,30), Point(60,300), Scalar(255,0,0), 1, 8, 0);
////    line(src, Point(30,230), Point(200,230), Scalar(255,0,0), 1, 8, 0);
//    rectangle(src,Point(300,300),Point(400,400),Scalar(255,0,0));
//    rectangle(src,Point(100,100),Point(250,250),Scalar(255,0,0));
//    Mat rec1 = Mat::ones(100, 100, CV_8UC1) * 255;
//    rec1.copyTo(src(Rect(100, 300, 100, 100)));
//    imwrite("pattern.jpg", src);
    
}
