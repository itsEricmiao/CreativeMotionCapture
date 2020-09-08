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

Generator::Generator(int N){
    this->allSquares._config(N);
}

void Generator::createSquares(){
    vector<vector<double>> allPos= this->allSquares.getVector();
    for(int i = 0; i < allPos.size(); i++){
        Rectf curSquare = Rectf(allPos[i][0],allPos[i][1],allPos[i][2],allPos[i][3]);
        double r1 = ((double) rand() / (RAND_MAX));
        double r2 = ((double) rand() / (RAND_MAX));
        double r3 = ((double) rand() / (RAND_MAX));
        gl::color( r1, r2, r3, 0.5 );
        gl::drawSolidRect(curSquare);
    }
}
