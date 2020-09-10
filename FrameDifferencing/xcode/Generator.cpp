//
//  Generator.cpp
//  FrameDifferencing
//
//  Created by Eric Miao on 9/7/20.
//

#include <stdio.h>
#include "Generator.hpp"

Generator::Generator(){
    
}

Generator::Generator(int n){
    // set the N value
    N = n;
}

void Generator::createSquares(cv::Mat mat, int color){
    // we call the config function to create all squares
    allSquares._config(N);
    cv::Mat myMat = mat;
    float widthOfSquare = getWindowWidth()/this->N;
    float heightOfSquare = getWindowHeight()/this->N;
    // allPos vector contains all the positions for each squares
    vector<vector<double>> allPos = allSquares.getVector();
    for(int i = 0; i < allPos.size(); i++){
        // allPost[i] has 4 values -> (x1, x2, y1, y1)
        double x1, y1, x2, y2;
        x1 = allPos[i][0];
        y1 = allPos[i][1];
        x2 = allPos[i][2];
        y2 = allPos[i][3];
        
        // calculate the total white pixels in each square
        int sum = 0;
        for (int a = x1; a < x2; a++){
            for (int b = y1; b < y2; b++){
                sum = sum + myMat.at<uint8_t>(b,a);
            }
        }
        
        // draw squares here
        // might need an algorithm here instead of hard coding a value 100
        float modifier = sum/float((255 * widthOfSquare * heightOfSquare));
        if (sum > 100){
            Rectf curSquare = Rectf(allPos[i][0],allPos[i][1],allPos[i][2],allPos[i][3]);
            gl::color(modifier, modifier, 0, 3);
            gl::drawSolidRect(curSquare);
        }
    }
}
