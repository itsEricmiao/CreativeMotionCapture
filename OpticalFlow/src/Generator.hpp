//
//  Generator.hpp
//  FeaturesTracking
//
//  Created by Eric Miao on 9/21/20.
//

#ifndef Generator_hpp
#define Generator_hpp

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/tracking.hpp>

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "Square.hpp"
#include <vector>
#include <stdio.h>

using namespace ci;
using namespace ci::app;
using namespace std;

class Generator{
protected:
    int N;
    char _mode; //'d' for frameDiff and 'f' for featuresSub
    vector <Square> allSquares; // contains all the square objects
    cv::Mat myMat;
    
public:
    Generator() {} // constructor
    
    Generator(int n) {N = n;} // constructor
    
    // config function for creating all the squares
    void configuration(){
        int length = getWindowWidth()/N;
        int height = getWindowHeight()/N;
        
        for (int row = 0; row < N; row++){
            for (int col = 0; col < N; col++){
                int x1 = col*length;
                int y1 = row*height;
                int x2 = col*length+length;
                int y2 = row*height+height;
                
                Square square(x1, y1, x2, y2, length, height);
                allSquares.push_back(square);
            }
        }
    }
    
    // display function for displaying all squares on the screen based on mode
    void display(){
        int norm = 0; //normalizing variable for color/transparecy
        if (_mode == 'f')
        {
            norm = 50;
        }
        else if (_mode == 'd')
        {
            norm = 50000;
        }
        
        //squareFeatureProperties();
        for (int i = 0; i < allSquares.size(); i++)
        {
            
            gl::color(0, 0, 1,  (float)allSquares[i].getFeatures() / norm); //set color based on features
            
            if (allSquares[i].getFeatures() > 0) //draw the square
            {
                gl::drawSolidRect( Rectf( allSquares[i].getX1(), allSquares[i].getY1(), allSquares[i].getX2(),allSquares[i].getY2()));
            }
        }
    }
    
    void setN(int n){N = n;}
    
    void setMode(char mode){_mode = mode;}
    
    void print(){
        for(int i = 0; i < allSquares.size(); i++){
            int x1, y1, x2, y2;
            x1 = allSquares[i].getX1();
            y1 = allSquares[i].getY1();
            x2 = allSquares[i].getX2();
            y2 = allSquares[i].getY2();
            cout<<x1<<" "<<y1<<" "<<x2<<" "<<y2<<endl;
        }
    }

};
#endif /* Generator_hpp */

