//
//  getInterestPoints.cpp
//  test_findInterestPoint
//
//  Created by HeCHen2 on 2018/1/24.
//  Copyright © 2018年 Wanting. All rights reserved.
//

#include "getInterestPoints.hpp"

void getInterestPoints(Mat& img, double sigma, vector<KeyPoint>&points){
    //*
    //* step 1: get gradient in x- and y direction
    //*
    Mat gx = img.clone();
    filter2D(img, gx, img.depth(), creatGradientsKernel_x(sigma));
    Mat gy = img.clone();
    filter2D(img, gy, img.depth(), creatGradientsKernel_y(sigma));
    
    //*
    //* step 2: get gx*gx; gy*gy; gx*gy
    //*
    Mat A = gx.mul(gx);
    Mat C = gy.mul(gy);
    Mat B = gx.mul(gy);
    
    //*
    //* step 3: average (Gaussian Window)
    //*
    GaussianBlur(A, A, Size(3,3), 2);
    GaussianBlur(B, B, Size(3,3), 2);
    GaussianBlur(C, C, Size(3,3), 2);
    showImage(A, "A", 1, 1, 1);
    showImage(B, "B", 1, 1, 1);
    showImage(C, "C", 1, 1, 1);
    
    //*
    //* step 4: Trace & Determinant of structure tensor
    //*
    Mat det = A.mul(C) - B.mul(B);
    Mat trace = A + C;
    showImage(trace, "trace image", 1, 1, 1);
    
    //*
    //* step 5: weight calculation
    //*
    Mat w = trace.clone();
    divide(det, trace, w);
    w = nonMaxSuppression(w);
    float w_mean = mean(w)[0];
    
    for(int i=0;i<w.rows;i++){
        for(int j=0;j<w.cols;j++){
            if(w.at<float>(i,j) > 1.5 * w_mean){
                points.push_back(KeyPoint(i, j, 5));
            }
        }
    }
    
    //*
    //* step 6: weight calculation
    //*
    Mat q = trace.clone();
    divide(4*det, trace.mul(trace), q);
    q = nonMaxSuppression(q);
    //showImage(q, "q", 1, 1, 1);
    for(int i=0;i<q.rows;i++){
        for(int j=0;j<q.cols;j++){
            if(q.at<float>(i,j) > 1.5){
                points.push_back(KeyPoint(i, j, 5));
            }
        }
    }
    
   cout << "Number of detected interest points:\t" << points.size() << endl;
   // Mat img_show = img.clone();
   drawKeypoints(img, points, img, Scalar(0,0,255), DrawMatchesFlags::DRAW_OVER_OUTIMG + DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    showImage(img, "keypoints_2", 0, true, true);
    
    
}

void showImage(Mat& img, const char* win, int wait, bool show, bool save){
    
    Mat aux = img.clone();
    
    // scale and convert
    if (img.channels() == 1)
        normalize(aux, aux, 0, 255, CV_MINMAX);
    aux.convertTo(aux, CV_8UC1);
    // show
    if (show){
        imshow( win, aux);
        waitKey(wait);
    }
    // save
    if (save)
        imwrite( (string(win)+string(".png")).c_str(), aux);
}

Mat nonMaxSuppression(Mat& img){
    
    Mat out = img.clone();
    
    for(int x=1; x<out.cols-1; x++){
        for(int y=1; y<out.rows-1; y++){
            if ( img.at<float>(y-1, x) >= img.at<float>(y, x) ){
                out.at<float>(y, x) = 0;
                continue;
            }
            if ( img.at<float>(y, x-1) >= img.at<float>(y, x) ){
                out.at<float>(y, x) = 0;
                continue;
            }
            if ( img.at<float>(y, x+1) >= img.at<float>(y, x) ){
                out.at<float>(y, x) = 0;
                continue;
            }
            if ( img.at<float>( y+1, x) >= img.at<float>(y, x) ){
                out.at<float>(y, x) = 0;
                continue;
            }
        }
    }
    return out;
}
