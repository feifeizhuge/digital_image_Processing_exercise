

#include "Dip4.h"

// Performes a circular shift in (dx,dy) direction
/*
in       :  input matrix
dx       :  shift in x-direction
dy       :  shift in y-direction
return   :  circular shifted matrix
*/
Mat Dip4::circShift(Mat& in, int dx, int dy){
    
    // define dst Mat
    Mat shift = in.clone();
    // records the src's row and col
    int row = in.rows;
    int col = in.cols;
    // compute the actul shift value
    dx = dx % row;
    dy = dy % col;
    
    // define the shifted coordinate in the dst image
    int newx = 0;
    int newy = 0;
    
    for(int i=0; i<row; i++){
        for(int j=0; j<col; j++){
            // the map relationship
            newx = (i + dx) % row;
            newy = (j + dy) % col;
            
            // if the dx and dy is negative, it needs shifts the image
            newx = (newx<0) ? (newx + row) : newx;
            newy = (newy<0) ? (newy + col) : newy;
            
            shift.at<float>(newx,newy) = in.at<float>(i,j);
        }
    }
    return shift;
}

// Function applies inverse filter to restorate a degraded image
/*
degraded :  degraded input image
filter   :  filter which caused degradation
return   :  restorated output image
*/
Mat Dip4::inverseFilter(Mat& degraded, Mat& filter){
    
    //*
    //* step one: degraded image's dft
    //*
    Mat dft_img = Mat::zeros(degraded.rows, degraded.cols, CV_32FC2);
    vector<Mat> i_cmpl;// it will contain real part and imaginary part
    i_cmpl.push_back(degraded.clone());
    i_cmpl.push_back(Mat::zeros(degraded.rows, degraded.cols, CV_32FC1));
    merge(i_cmpl, dft_img);// dft_img is a two channel img
    dft(dft_img, dft_img, DFT_COMPLEX_OUTPUT);
    // i_cmpl[0] is the real part
    // i_cmpl[1] is the imaginary part
    split(dft_img, i_cmpl);
    
    //*
    //* step two: kernel's dft
    //*           1) expand kernel to the same size as degraded image
    //*           2) use ciclshift to get shifted kernel
    //*           3) get it's real part and imaginary part
    //*
    
    // 1) expand kernel to the same size as degraded image
    vector<Mat> f_cmpl;
    Mat tmp = Mat::zeros(degraded.rows, degraded.cols, CV_32FC1);
    Mat dft_kernel = Mat::zeros(degraded.rows, degraded.cols, CV_32FC2);
    Mat rect = tmp(Rect(0, 0, filter.rows, filter.cols));
    filter.copyTo(rect);
    // 2) use ciclshift to get shifted kernel
    tmp = circShift(tmp, -filter.rows/2, -filter.cols/2);
    f_cmpl.push_back(tmp);
    f_cmpl.push_back(Mat::zeros(degraded.rows, degraded.cols, CV_32FC1));
    merge(f_cmpl, dft_kernel);
    // 3) get it's real part and imaginary part
    dft(dft_kernel, dft_kernel, DFT_COMPLEX_OUTPUT);
    // f_cmpl[0] is the real part
    // f_cmpl[1] is the imaginary part
    split(dft_kernel, f_cmpl);
    
    //*
    //* step three: replace the inverse filter Pij by Qij
    //*             1)define the |Pij| = (real^2+imaginary^2)^0.5
    //*             2)threshold the P(kernel's DFT value)
    //*
    Mat amp; // amplitude
    Mat phase; // phase
    double eps = 0.05; // coefficient
    double minAmp, maxAmp; // min & max value of amplitude
    //* 1) define the |Pij| = (real^2+imaginary^2)^0.5
    cartToPolar(f_cmpl[0], f_cmpl[1], amp, phase);
    //* 2)threshold the P(kernel's DFT value)
    minMaxLoc(amp, &minAmp, &maxAmp);
    amp.setTo(1, amp < eps * maxAmp);
    phase.setTo(0, phase < eps * maxAmp);
    // transform back to cart-coordinate
    polarToCart(amp, phase, f_cmpl[0], f_cmpl[1]);
    
    //*
    //* step four: do Oij = Sij/Pij in the frequence domain
    //*
    for(int y=0; y<degraded.rows; y++){
        for(int x=0; x<degraded.cols; x++){
            
            double a = i_cmpl[0].at<float>(y,x);
            double b = i_cmpl[1].at<float>(y,x);
            double c = f_cmpl[0].at<float>(y,x);
            double d = f_cmpl[1].at<float>(y,x);
            
            i_cmpl[0].at<float>(y,x) = (a*c + b*d) / (c*c + d*d);
            i_cmpl[1].at<float>(y,x) = (b*c - a*d) / (c*c + d*d);
        }
    }
    merge(i_cmpl, dft_img);
    Mat out;
    dft(dft_img, out, DFT_INVERSE + DFT_REAL_OUTPUT + DFT_SCALE);
   return out;
}

// Function applies wiener filter to restorate a degraded image
/*
degraded :  degraded input image
filter   :  filter which caused degradation
snr      :  signal to noise ratio of the input image
return   :   restorated output image
*/
Mat Dip4::wienerFilter(Mat& degraded, Mat& filter, double snr){

    //*
    //* step one: degraded image's dft
    //*
    Mat dft_img = Mat::zeros(degraded.rows, degraded.cols, CV_32FC2);
    vector<Mat> i_cmpl;// it will contain real part and imaginary part
    i_cmpl.push_back(degraded.clone());
    i_cmpl.push_back(Mat::zeros(degraded.rows, degraded.cols, CV_32FC1));
    merge(i_cmpl, dft_img);// dft_img is a two channel img
    dft(dft_img, dft_img, DFT_COMPLEX_OUTPUT);
    // i_cmpl[0] is the real part
    // i_cmpl[1] is the imaginary part
    split(dft_img, i_cmpl);
    
    //*
    //* step two: kernel's dft
    //*           1) expand kernel to the same size as degraded image
    //*           2) use ciclshift to get shifted kernel
    //*           3) get it's real part and imaginary part
    //*
    
    // 1) expand kernel to the same size as degraded image
    vector<Mat> f_cmpl;
    Mat tmp = Mat::zeros(degraded.rows, degraded.cols, CV_32FC1);
    Mat dft_kernel = Mat::zeros(degraded.rows, degraded.cols, CV_32FC2);
    Mat rect = tmp(Rect(0, 0, filter.rows, filter.cols));
    filter.copyTo(rect);
    // 2) use ciclshift to get shifted kernel
    tmp = circShift(tmp, -filter.rows/2, -filter.cols/2);
    f_cmpl.push_back(tmp);
    f_cmpl.push_back(Mat::zeros(degraded.rows, degraded.cols, CV_32FC1));
    merge(f_cmpl, dft_kernel);
    // 3) get it's real part and imaginary part
    dft(dft_kernel, dft_kernel, DFT_COMPLEX_OUTPUT);
    // f_cmpl[0] is the real part
    // f_cmpl[1] is the imaginary part
    split(dft_kernel, f_cmpl);
    
    //*
    //* step three: replace the inverse filter Pij by Qij
    //*             1)define the |Pij| = (real^2+imaginary^2)^0.5
    //*             2)threshold the P(kernel's DFT value)
    //*
    Mat amp; // amplitude
    Mat phase; // phase
    //* 1) define the |Pij| = (real^2+imaginary^2)^0.5
    cartToPolar(f_cmpl[0], f_cmpl[1], amp, phase);
    amp = amp.mul(amp);
    //* 2) Qk = Pk*/(|PK|^2 + 1/SNR^2)
    for(int y=0; y<degraded.rows; y++){
        for(int x=0; x<degraded.cols; x++){
            float real = f_cmpl[0].at<float>(y,x);
            float imagi = f_cmpl[1].at<float>(y,x);
            f_cmpl[0].at<float>(y,x) =  real / (amp.at<float>(y,x) + 1 / (snr*snr));
            f_cmpl[1].at<float>(y,x) = - imagi / (amp.at<float>(y,x) + 1 / (snr*snr));
        }
    }
    
    //*
    //* step four: do Oij = Sij/Pij in the frequence domain
    //*
    for(int y=0; y<degraded.rows; y++){
        for(int x=0; x<degraded.cols; x++){
            
            double a = i_cmpl[0].at<float>(y,x);
            double b = i_cmpl[1].at<float>(y,x);
            double c = f_cmpl[0].at<float>(y,x);
            double d = f_cmpl[1].at<float>(y,x);
            
            i_cmpl[0].at<float>(y,x) = a*c - b*d;
            i_cmpl[1].at<float>(y,x) = b*c + a*d;
        }
    }
    merge(i_cmpl, dft_img);
    Mat out;
    dft(dft_img, out, DFT_INVERSE + DFT_REAL_OUTPUT + DFT_SCALE);
    return out;
}

/* *****************************
  GIVEN FUNCTIONS
***************************** */

// function calls processing function
/*
in                   :  input image
restorationType     :  integer defining which restoration function is used
kernel               :  kernel used during restoration
snr                  :  signal-to-noise ratio (only used by wieder filter)
return               :  restorated image
*/
Mat Dip4::run(Mat& in, string restorationType, Mat& kernel, double snr){

   if (restorationType.compare("wiener")==0){
      return wienerFilter(in, kernel, snr);
   }else{
      return inverseFilter(in, kernel);
   }

}

// function degrades the given image with gaussian blur and additive gaussian noise
/*
img         :  input image
degradedImg :  degraded output image
filterDev   :  standard deviation of kernel for gaussian blur
snr         :  signal to noise ratio for additive gaussian noise
return      :  the used gaussian kernel
*/
Mat Dip4::degradeImage(Mat& img, Mat& degradedImg, double filterDev, double snr){

    int kSize = round(filterDev*3)*2 - 1;
   
    Mat gaussKernel = getGaussianKernel(kSize, filterDev, CV_32FC1);
    gaussKernel = gaussKernel * gaussKernel.t();

    Mat imgs = img.clone();
    dft( imgs, imgs, CV_DXT_FORWARD, img.rows);
    Mat kernels = Mat::zeros( img.rows, img.cols, CV_32FC1);
    int dx, dy; dx = dy = (kSize-1)/2.;
    for(int i=0; i<kSize; i++) for(int j=0; j<kSize; j++) kernels.at<float>((i - dy + img.rows) % img.rows,(j - dx + img.cols) % img.cols) = gaussKernel.at<float>(i,j);
	dft( kernels, kernels, CV_DXT_FORWARD );
	mulSpectrums( imgs, kernels, imgs, 0 );
	dft( imgs, degradedImg, CV_DXT_INV_SCALE, img.rows );
	
    Mat mean, stddev;
    meanStdDev(img, mean, stddev);

    Mat noise = Mat::zeros(img.rows, img.cols, CV_32FC1);
    randn(noise, 0, stddev.at<double>(0)/snr);
    degradedImg = degradedImg + noise;
    threshold(degradedImg, degradedImg, 255, 255, CV_THRESH_TRUNC);
    threshold(degradedImg, degradedImg, 0, 0, CV_THRESH_TOZERO);

    return gaussKernel;
}

// Function displays image (after proper normalization)
/*
win   :  Window name
img   :  Image that shall be displayed
cut   :  whether to cut or scale values outside of [0,255] range
*/
void Dip4::showImage(const char* win, Mat img, bool cut){

   Mat tmp = img.clone();

   if (tmp.channels() == 1){
      if (cut){
         threshold(tmp, tmp, 255, 255, CV_THRESH_TRUNC);
         threshold(tmp, tmp, 0, 0, CV_THRESH_TOZERO);
      }else
         normalize(tmp, tmp, 0, 255, CV_MINMAX);
         
      tmp.convertTo(tmp, CV_8UC1);
   }else{
      tmp.convertTo(tmp, CV_8UC3);
   }
   imshow(win, tmp);
}

// function calls basic testing routines to test individual functions for correctness
void Dip4::test(void){

   test_circShift();
   cout << "Press enter to continue"  << endl;
   cin.get();

}

void Dip4::test_circShift(void){
   
   Mat in = Mat::zeros(3,3,CV_32FC1);
   in.at<float>(0,0) = 1;
   in.at<float>(0,1) = 2;
   in.at<float>(1,0) = 3;
   in.at<float>(1,1) = 4;
   Mat ref = Mat::zeros(3,3,CV_32FC1);
   ref.at<float>(0,0) = 4;
   ref.at<float>(0,2) = 3;
   ref.at<float>(2,0) = 2;
   ref.at<float>(2,2) = 1;
   
   if (sum((circShift(in, -1, -1) == ref)).val[0]/255 != 9){
      cout << "ERROR: Dip4::circShift(): Result of circshift seems to be wrong!" << endl;
      return;
   }
   cout << "Message: Dip4::circShift() seems to be correct" << endl;
}
