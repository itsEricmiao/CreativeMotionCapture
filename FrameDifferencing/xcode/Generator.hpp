//
//  Generator.hpp
//  FrameDifferencing
//
//  Created by Eric Miao on 9/7/20.
//

#ifndef Generator_hpp
#define Generator_hpp

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/tracking.hpp>

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include <stdio.h>
#include <iostream>
#include <vector>
#include "Squares.hpp"

using namespace std;
using namespace ci;
using namespace ci::app;


class Generator{
private:
    Squares allSquares;
    int N;
    
public:
    Generator();
    Generator(int n);
    void createSquares(cv::Mat mat);
};
#endif /* Generator_hpp */

