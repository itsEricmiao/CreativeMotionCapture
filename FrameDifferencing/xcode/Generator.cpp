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
    N = n;
}

void Generator::createSquares(cv::Mat mat){
    allSquares._config(N);
    cv::Mat myMat = mat;
    vector<vector<double>> allPos = allSquares.getVector();
    for(int i = 0; i < allPos.size(); i++){
        double x1, y1, x2, y2;
        x1 = allPos[i][0];
        y1 = allPos[i][1];
        x2 = allPos[i][2];
        y2 = allPos[i][3];
        int sum = 0;
        for (int a = x1; a < x2; a++){
            for (int b = y1; b < y2; b++){
                sum = sum + myMat.at<uint8_t>(b,a);
            }
        }

        if (sum > 300){
            Rectf curSquare = Rectf(allPos[i][0],allPos[i][1],allPos[i][2],allPos[i][3]);
            gl::color( 1, 1, 1, 0.3);
            gl::drawSolidRect(curSquare);
        }
    }
}
