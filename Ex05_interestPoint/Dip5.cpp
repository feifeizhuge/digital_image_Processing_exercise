//============================================================================
// Name        : Dip5.cpp
// Author      : Ronny Haensch
// Version     : 2.0
// Copyright   : -
// Description : 
//============================================================================

#include "Dip5.h"

// uses structure tensor to define interest points (foerstner)
void Dip5::getInterestPoints(Mat& img, double sigma, vector<KeyPoint>& points){
    //*
    //* step 1: get gradient in x- and y direction
    //*
    Mat gx = img.clone();
    filter2D(img, gx, img.depth(), createFstDevKernel(sigma));
    Mat gy = img.clone();
    filter2D(img, gy, img.depth(), createFstDevKernel(sigma).t());
    
    //*
    //* step 2: get gx*gx; gy*gy; gx*gy
    //*
    Mat A = gx.mul(gx);
    Mat C = gy.mul(gy);
    Mat B = gx.mul(gy);
    
    //*
    //* step 3: average (Gaussian Window)
    //*
    GaussianBlur(A, A, Size(3,3), 3);
    GaussianBlur(B, B, Size(3,3), 3);
    GaussianBlur(C, C, Size(3,3), 3);
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
                points.push_back(KeyPoint(j, i, 5));
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
                points.push_back(KeyPoint(j, i, 5));
            }
        }
    }
}

// creates kernel representing fst derivative of a Gaussian kernel in x-direction
/*
sigma	standard deviation of the Gaussian kernel
return	the calculated kernel
*/
Mat Dip5::createFstDevKernel(double sigma){
    // define Pi constant
    double const PI = 3.14159;
    // define sum of kernel's points
    double sum = 0;
    // define Gaussian sigma, 经验的
    int KSize = 5;
    
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

/* *****************************
  GIVEN FUNCTIONS
***************************** */

// function calls processing function
/*
in		:  input image
points	:	detected keypoints
*/
void Dip5::run(Mat& in, vector<KeyPoint>& points){
   this->getInterestPoints(in, this->sigma, points);
}

// non-maxima suppression
// if any of the pixel at the 4-neighborhood is greater than current pixel, set it to zero
Mat Dip5::nonMaxSuppression(Mat& img){

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

// Function displays image (after proper normalization)
/*
win   :  Window name
img   :  Image that shall be displayed
cut   :  whether to cut or scale values outside of [0,255] range
*/
void Dip5::showImage(Mat& img, const char* win, int wait, bool show, bool save){
  
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
