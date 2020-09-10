//
//  Squares.hpp
//  FrameDifferencing
//  Created by Eric Miao on 9/3/20.
//

#ifndef Squares_hpp
#define Squares_hpp

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/tracking.hpp>

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include <stdio.h>
#include <iostream>
#include <vector>

using namespace std;

class Squares{
private:
    int windowWidth;
    int windowHeight;
    int N;
    vector<vector<double>> allPos;

    
public:
    Squares();
    Squares(int numberSquares);
    void _config(int numberSquares);
    void setWindowWidth(int x){this->windowWidth = x;}
    void setWindowHeight(int y){this->windowHeight = y;}
    void setN(int n){this->N = n;}
    vector<vector<double>>getVector();
    int getNval();

};
#endif /* Squares_hpp */


