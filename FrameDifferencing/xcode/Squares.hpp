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
using namespace ci;
using namespace ci::app;

class Squares{
private:
    int windowWidth;
    int windowHeight;
    int N;
    vector<vector<double>> allPos;

    
public:
    Squares();
    Squares(int numberSquares);
    
    void _config();
    void setWindowWidth(int x){this->windowWidth = x;}
    void setWindowHeight(int y){this->windowHeight = y;}
    vector<vector<double>>getVector();

};
#endif /* Squares_hpp */


// function implementation starts from here:
Squares::Squares(){
    
}


Squares::Squares(int numberSquares){
    this->N = numberSquares;
    this->setWindowWidth(getWindowWidth());
    this->setWindowHeight(getWindowHeight());
    this->_config();
}


void Squares::_config(){
    
    float l = this->windowWidth/this->N;
    float l2 = this->windowHeight/this->N;
    
    for (int row = 0; row < N; row++){
        for (int col = 0; col < N; col++){
//          cout<<"("<<col * l <<","<< row * l2 <<")  ("<< col*l+l <<","<< row * l2+l2<<")"<<endl;
            vector<double> currentPos;
            currentPos.push_back(col*l);
            currentPos.push_back(row*l2);
            currentPos.push_back(col*l+l);
            currentPos.push_back(row*l2+l2);
            this->allPos.push_back(currentPos);
        }
    }
}


vector<vector<double>> Squares::getVector(){
    return this->allPos;
}
