//
//  FrameDifferencing.hpp
//  FeaturesTracking
//
//  Created by Eric Miao on 9/21/20.
//

#ifndef FrameDifferencing_hpp
#define FrameDifferencing_hpp
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/tracking.hpp>
#include "CinderOpenCV.h"
#include "Generator.hpp"
#include "Square.hpp"
#include <stdio.h>

using namespace ci;
using namespace ci::app;
using namespace std;

class FrameDifferencing : public Generator{
public:
    
    
    void countPixels(cv::Mat myMat){ // Couting the pixels in each squares
        int sum = 0;
        //go through each square in the vector
        for (int i = 0; i < allSquares.size(); i++)
        {
            sum = 0;
            allSquares[i].setFeatures(0);
            for(int m = allSquares[i].getX1(); m < allSquares[i].getX2(); m++)
            {
                for(int n = allSquares[i].getY1(); n < allSquares[i].getY2(); n++)
                {
                    sum = sum + myMat.at<u_int8_t>(n, m);
                }
            }
            allSquares[i].setFeatures(sum);
        }
    }
};

#endif /* FrameDifferencing_hpp */

