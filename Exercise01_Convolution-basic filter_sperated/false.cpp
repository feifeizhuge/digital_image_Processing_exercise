//
//  main.cpp
//  cv_convolution
//
//  Created by Wanting on 2017/11/23.
//  Copyright © 2017年 Wanting. All rights reserved.
//

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;
int main(int argc, const char * argv[]) {
    
    int img_rows,img_colums;
    float sum=0.0;
    Mat img;
    Mat temp = Mat(3,3,CV_32FC1);
    Mat temp1 = Mat(3,3,CV_32FC1);
    img = imread("/Users/hechen/Documents/c++_code/cv_convolution/cv_convolution/noiseType_2.jpg",CV_32FC1);
    Mat filter_img(img.rows,img.cols,CV_32FC1);
    Mat M1 = (Mat_<float>(3,3) << 1/9,1/9,1/9,1/9,1/9,1/9,1/9,1/9,1/9);

    //get rows and colums
    
    img_rows = img.rows;
    img_colums = img.cols;
    //convolution step
    for(int i=1;i<img_rows;i++){
        
        for(int j=1;j<img_colums;j++){
            
            temp = (Mat_<float>(3,3) << img.at<float>(i,j));
                    /*img.at<float>(i-1,j-1),img.at<float>(i-1,j-1),img.at<float>(i,j-1),img.at<float>(i+1,j-1),img.at<float>(i-1,j),img.at<float>(i,j),img.at<float>(i+1,j),img.at<float>(i-1,j+1),img.at<float>(i,j+1),img.at<float>(i+1,j+1));*/
            
            cout << temp.at<float>(1,1) << endl;
            temp1 = temp.mul(M1);
            for (int i=0;i<3;i++){
                for(int j=0;j<3;j++){
                    sum += temp1.at<float>(i,j);
                    cout << sum << endl;
                }
            }
            
            
            
            filter_img.at<float>(i,j) = sum;
        }
    }
    
    namedWindow("filter_img");
    imshow("filter_img", filter_img);
    imwrite("filter.png", filter_img);
    waitKey();
    
    return 0;
}
