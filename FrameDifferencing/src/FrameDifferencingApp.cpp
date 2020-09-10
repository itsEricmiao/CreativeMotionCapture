#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/tracking.hpp>

//includes for background subtraction
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/video.hpp>


#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Capture.h" //needed for capture
#include "cinder/Log.h" //needed to log errors

#include "Osc.h" //add to send OSC

#include <math.h>

#include "CinderOpenCV.h"
#include "Squares.hpp"
#include "Generator.hpp"
#include <vector>


using namespace ci;
using namespace ci::app;
using namespace std;

class FrameDifferencingApp : public App {
  public:
    char keyPressed;
    void setup() override;
    void keyDown( KeyEvent event ) override;
    void update() override;
    void draw() override;
    
  protected:
    CaptureRef                 mCapture;
    gl::TextureRef             mTexture;
    
    cv::Mat                    mPrevFrame;
    cv::Mat                    mFrameDifference;
    ci::SurfaceRef             mSurface;

    void frameDifference(cv::Mat &outputImg);
};




//initialization
void FrameDifferencingApp::setup()
{
    //set up our camera
    try {
        mCapture = Capture::create(640, 480); //first default camera
        mCapture->start();
    }
    catch( ci::Exception &exc)
    {
        CI_LOG_EXCEPTION( "Failed to init capture ", exc );
    }

    mPrevFrame.data = NULL;
    mFrameDifference.data = NULL;
    mCapture = Capture::create(640, 480); //first default camera
    mCapture->start();
    cout<<"Press a to display 10x10"<<endl;
    cout<<"Press b to display 20x20"<<endl;
    cout<<"Press c to display 48x48"<<endl;
    cout<<"Press d to display 48x48 with random color"<<endl;
}

void FrameDifferencingApp::keyDown( KeyEvent event )
{
    //TODO: save the current frame as the background image when user hits a key
    if(event.getChar() == 'a')
    {
        keyPressed = 'a';
    }
    
    if(event.getChar() == 'b')
    {
        keyPressed = 'b';
    }
    
    if(event.getChar() == 'c')
    {
        keyPressed = 'c';
    }
    if(event.getChar() == 'd')
    {
        keyPressed = 'd';
    }

}

void FrameDifferencingApp::update()
{
    if(mCapture && mCapture->checkNewFrame()) //is there a new frame???? (& did camera get created?)
    {
        mSurface = mCapture->getSurface();

        if(! mTexture)
            mTexture = gl::Texture::create(*mSurface);
        else
            mTexture->update(*mSurface);
    }
    
    //do the frame-differencing
    frameDifference(mFrameDifference);
//    mFrameDifference.at<uint8_t>();
}


//find the difference between 2 frames + some useful image processing
void FrameDifferencingApp::frameDifference(cv::Mat &outputImg)
{
    outputImg.data = NULL;
    if(!mSurface) return;

    //the current surface or frame in cv::Mat format
    cv::Mat curFrame = toOcv(Channel(*mSurface));
    
    if(mPrevFrame.data){
        
        cv::GaussianBlur(curFrame, curFrame, cv::Size(3,3), 0);
        cv::absdiff(curFrame, mPrevFrame, outputImg);
        cv::threshold(outputImg, outputImg, 25, 255, cv::THRESH_BINARY);
    }
    
    mPrevFrame = curFrame;
}



void FrameDifferencingApp::draw()
{
    gl::clear( Color( 0, 0, 0 ) );
    gl::color( 1, 1, 1, 1);

    if (keyPressed == 'a'){
        Generator x(10);
        x.createSquares(mFrameDifference);
    }
    if(keyPressed == 'b'){
        Generator x(20);
        x.createSquares(mFrameDifference);
    }
    if(keyPressed == 'c'){
        Generator x(48);
        x.createSquares(mFrameDifference);
    }
    if(keyPressed == 'd'){
        Generator x(48);
        x.createSquares(mFrameDifference,1);
    }
    
//    if( mTexture )
//    {
//        gl::draw( mTexture );
//    }
//    if the frame difference isn't null, draw it.
//    if( mFrameDifference.data )
//    {
//        gl::draw( gl::Texture::create(fromOcv(mFrameDifference) ));
//    }

    
}
CINDER_APP( FrameDifferencingApp, RendererGl )
