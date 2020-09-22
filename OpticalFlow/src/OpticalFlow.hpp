//
//  OpticalFlow.hpp
//  FeaturesTracking
//
//  Created by Eric Miao on 9/21/20.
//

#ifndef OpticalFlow_hpp
#define OpticalFlow_hpp
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

class OpticalFlow : public Generator{
public:
    void countFeatures(vector<cv::Point2f> points){
        for (int i = 0; i < allSquares.size(); i++) //checking each square area
        {
            allSquares[i].setFeatures(0);
            int count = 0;
            
            for (int j = 0; j < points.size(); j++) //get the number of features in the square area
            {
                if ( (points[j].x >= allSquares[i].getX1() && points[j].x < allSquares[i].getX2() )  &&
                     (points[j].y >= allSquares[i].getY1() && points[j].y < allSquares[i].getY2() ))
                {
                    count++;
                }
            }
            cout<<"setFeatures:"<<count<<endl;
            allSquares[i].setFeatures(count); //set the feature count as a property of the square
        }
    }
    void squareFeatureProperties() //cout testing function
    {
        
    }
};
#endif /* OpticalFlow_hpp */
