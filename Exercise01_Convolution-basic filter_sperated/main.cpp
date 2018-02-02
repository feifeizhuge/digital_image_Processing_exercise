//
//  mian.cpp
//  cv_convolution
//
//  Created by Wanting on 2018/1/2.
//  Copyright © 2018年 Wanting. All rights reserved.
//
#include <iostream>
#include "spatialFilter.hpp"

using namespace cv;
using namespace std;
int main( int argc, char** argv ) {



    // construct processing object
    spatialFilter spatialfilter;
    
    spatialfilter.run();
    
    
    /*
    int array[] = {23,4,55,42,-10,30,12};
    int elements = sizeof(array) / sizeof(array[0]);
    std::sort(array,array+elements);
    for(int i=0;i < elements; ++i)
        std::cout << array[i] << ' ';
     
     for(int i=-kSize/2; i<(kSize/2+1); i++){
     for (int j=-kSize/2; j<(kSize/2+1); j++){
     spatialTable[index] = (double)exp(-(i*i+j*j)/(2*sigma1*sigma1));
     index++;
     }
     }

     */

}
