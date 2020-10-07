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
//    cv::Mat myMat;
    
public:
    Generator() {} // constructor
    
    Generator(int n) {N = n;} // constructor
    
    void _setMode(char mode){_mode = mode;} //setter for mode : either f or d
    void _setN(int n){N = n;} // setter for number of squares on the screen
    
    // config function for creating all the squares
    void configuration(char mode, int n){
        this->_setMode(mode);
        this->_setN(n);
        
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
        
        // when in frame differencing mode:
        // we use numPixels/255*w*h to normalize the color of the square
        if (_mode == 'd'){
            for (int i = 0; i < allSquares.size(); i++)
            {
                int widthOfSquare = allSquares[i].getWidth();
                int heightOfSquare = allSquares[i].getHeight();
                
                // modifier normzalizes the color value
                float modifier = allSquares[i].getFeatures()/float((255 * widthOfSquare * heightOfSquare));
                if (allSquares[i].getFeatures() > 100)
                {
                    //draw the square
                    // the transparency changes based on the number of pixels in a square
                    gl::color(modifier, modifier, 0,  3);
                    Rectf curSquare = Rectf( allSquares[i].getX1(), allSquares[i].getY1(), allSquares[i].getX2(),allSquares[i].getY2());
                    gl::drawSolidRect(curSquare);
                }
            }
        }
        
        // when in optical flow mode:
        // we use a norm value to normalize the color of the square.
        if (_mode == 'f'){
            int norm = 5;
            for (int i = 0; i < allSquares.size(); i++)
            {
                if (allSquares[i].getFeatures() > 0)
                {
                    //draw the square
                    gl::color(0, 0, 1,  (float)allSquares[i].getFeatures() / norm);
                    Rectf curSquare = Rectf( allSquares[i].getX1(), allSquares[i].getY1(), allSquares[i].getX2(),allSquares[i].getY2());
                    gl::drawSolidRect(curSquare);
                }
            }
        }
    }
    
    // mainly for testing, not used in the actual program
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
    
    vector<Square> getSquares(){
        return this->allSquares;
    }

};
#endif /* Generator_hpp */

