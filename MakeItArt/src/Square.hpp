//
//  Square.hpp
//  FeaturesTracking
//
//  Created by Eric Miao on 9/21/20.
//

#ifndef Square_hpp
#define Square_hpp

using namespace ci;
using namespace ci::app;
using namespace std;

#include <stdio.h>

// This is the basic square class
class Square{
private:
    int x1;
    int y1;
    int x2;
    int y2;
    int length;
    int height;
    int num_features;

public:
    Square(){} // constructor
    Square(int a, int b, int c, int d, int l, int h){ // constructor
        x1 = a;
        y1 = b;
        x2 = c;
        y2 = d;
        length = l;
        height = h;
    }
    
    // setters and getters
    void setFeatures(int n){num_features = n;}
    int getX1(){return x1;}
    int getX2(){return x2;}
    int getY1(){return y1;}
    int getY2(){return y2;}
    int getHeight(){return height;}
    int getWidth(){return length;}
    int getFeatures(){return this->num_features;};
};

#endif /* Square_hpp */



