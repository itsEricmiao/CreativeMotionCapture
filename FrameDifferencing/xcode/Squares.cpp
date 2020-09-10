//
//  Squares.cpp
//  FrameDifferencing
//
//  Created by Eric Miao on 9/7/20.
//
#include "Squares.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/tracking.hpp>

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include <iostream>
#include <vector>
#include <stdio.h>
using namespace std;
// function implementation starts from here:
Squares::Squares(){
    
}


Squares::Squares(int numberSquares){
    this->N = numberSquares;
    this->setWindowWidth(cinder::app::getWindowWidth());
    this->setWindowHeight(cinder::app::getWindowHeight());
    this->_config(numberSquares);
}


void Squares::_config(int numberSquares){
    this->setN(numberSquares);
    this->setWindowWidth(cinder::app::getWindowWidth());
    this->setWindowHeight(cinder::app::getWindowHeight());
    
    float l = this->windowWidth/this->N;
    float l2 = this->windowHeight/this->N;
    
    cout<<l<<" "<<l2<<endl;
    for (int row = 0; row < N; row++){
        for (int col = 0; col < N; col++){
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

int Squares::getNval(){
    return this->N;
}
