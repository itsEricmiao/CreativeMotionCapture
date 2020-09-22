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
    
    
    void countPixels(cv::Mat outputImg){
        int pixelAddition = 0;
        for (int i = 0; i < allSquares.size(); i++) //cycle through square vector
        {
            pixelAddition = 0;
            allSquares[i].setFeatures(0);
            for(int a = allSquares[i].getX1(); a < allSquares[i].getX2(); a++)
            {
                for(int b = allSquares[i].getY1(); b < allSquares[i].getY2(); b++)
                {
                    pixelAddition = pixelAddition + outputImg.at<u_int8_t>(b, a);
                }
            }
            allSquares[i].setFeatures(pixelAddition);
        }
    }
};

#endif /* FrameDifferencing_hpp */

