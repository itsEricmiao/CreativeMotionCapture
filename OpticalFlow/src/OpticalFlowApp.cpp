// Fall 2020
// CRCP-5350
// Programmer: Eric Miao



/*
 Programmer: Courtney Brown
 Date: c2019
 Notes: Modified from Cinder OpenCV sample on optical flow
 Purpose/Description:
 
 This program draws the optical flow of live video using feature detection.
 
 Uses:
 
 cv::goodFeaturesToTrack - in this case, the features that are "good to track" are simply corners/edges.
 
 cv::calcOpticalFlowPyrLK - creates the mFeatureStatuses array which is a map from current features (mFeatures) into the previous features (mPreviousFeatures).
 
 Output/Drawing:
 Previous Features are 50% transparent red (drawn first)
 Current Features are 50% transparent blue
 The optical flow or path from previous to current is drawn in green.
 
 Instructions:
 Copy and paste this code into your cpp file.
 Add the NSCameraUsageDescription key to your Info.plist file in order to get permission to use the camera.
 You will have to add a description of what you are using the camera for. This will show up in the permissions pop-up.
 
 Run. Observe the results.
 
 What do think optical flow is measuring? If you haven't read the OpenCV tutorial on Optical flow, here it is:
 https://docs.opencv.org/3.4/d4/dee/tutorial_optical_flow.html

 */

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

#include "CinderOpenCV.h"

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Capture.h" //add - needed for capture
#include "cinder/Log.h" //add - needed to log errors
#include "Osc.h" //add to send OSC
#include <math.h>

#include "Generator.hpp"
#include "FrameDifferencing.hpp"
#include "OpticalFlow.hpp"

#define SAMPLE_WINDOW_MOD 10 //how often we find new features -- that is 1/300 frames we will find some features
#define MAX_FEATURES 300 //The maximum number of features to track. Experiment with changing this number


using namespace cinder;
using namespace ci::app;
using namespace std;

class OpticalFlowApp : public App {
  public:
    
    bool ifbgSub = false; //boolean value for determining if uses bg substraction
    
    cv::Ptr<cv::BackgroundSubtractor> mBackgroundSubtract; // from openCV library
    
    char keyPressed;
    void setup() override;
    void mouseDown( MouseEvent event ) override;
    void keyDown( KeyEvent event ) override;
    void update() override;
    void draw() override;
    
protected:
    CaptureRef                 mCapture; //uses video camera to capture frames of data.
    gl::TextureRef             mTexture; //the current frame of visual data in OpenGL format.
    
    //for optical flow
    vector<cv::Point2f>        mPrevFeatures; //the features that we found in the last frame
    vector<cv::Point2f>        mFeatures; //the feature that we found in the current frame
    vector<uint8_t>            mFeatureStatuses; //a map of previous features to current features
    
    cv::Mat                    mPrevFrame; //the last frame
    cv::Mat                    mFrameDifference;
    cv::Mat                    mCurrFrame; // the current frame
    ci::SurfaceRef             mSurface; //the current frame of visual data in CInder format.
    
    
    void findOpticalFlow(); //finds the optical flow -- the visual or apparent motion of features (or persons or things or what you can detect/measure) through video
    void frameDifference(cv::Mat &outputImg); //finds the frame differencing

};



void OpticalFlowApp::setup()
{
    //set up our camera
    try {
        mCapture = Capture::create(640, 480); //first default camera
        mCapture->start();
    }
    catch( ci::Exception &exc)
    {
        CI_LOG_EXCEPTION( "Failed to init capture ", exc ); //oh no!!
    }
    mPrevFrame.data = NULL;
    mFrameDifference.data = NULL;
    mCapture = Capture::create(640, 480); //first default camera
    mCapture->start();
    
    // do the bg substraction
    mBackgroundSubtract = cv::createBackgroundSubtractorKNN();
    
    cout<<endl;
    cout<<"CRCP-5350 Project 2: Optical Flow"<<endl;
    cout<<"Press 1 to display Optical Flow 10x10"<<endl;
    cout<<"Press 2 to display Optical Flow 20x20"<<endl;
    cout<<"Press 3 to display Optical Flow 48x48"<<endl;
    cout<<"Press d to display Frame Differencing 20x20"<<endl;
    cout<<"Press f to display Optical Flow 20x20"<<endl;
    cout<<"Press space to show background substraction"<<endl;
    
}

void OpticalFlowApp::mouseDown( MouseEvent event ){}

void OpticalFlowApp::update()
{
    //update the current frame from camera
    if(mCapture && mCapture->checkNewFrame()) //is there a new frame???? (& did camera get created?)
    {
        mSurface = mCapture->getSurface(); //get the current frame and put in the Cinder datatype.
        
        //translate the cinder frame (Surface) into the OpenGL one (Texture)
        if(! mTexture)
            mTexture = gl::Texture::create(*mSurface);
        else
            mTexture->update(*mSurface);
    }

    // if keyPressed is 'd' or 'space', runs the framDifferencing function
    if(keyPressed == 'd'){
        frameDifference(mFrameDifference);
    }
    if(keyPressed == ' '){
        frameDifference(mFrameDifference);
    }
    
    // else: runs the framDifferencing function
    else{
        findOpticalFlow();
    }
    
}


void OpticalFlowApp::keyDown( KeyEvent event ){
    if (event.getChar() == 'f') {
        cout<<"Key 'f' is pressed"<<endl;
        keyPressed = 'f';
    }
    
    if (event.getChar() == 'd')
    {
        cout<<"Key 'd' is pressed"<<endl;
        keyPressed = 'd';
    }
    
    if (event.getChar() == ' ')
    {
        cout<<"Key 'space' is pressed"<<endl;
        ifbgSub = !ifbgSub;
    }
    
    if (event.getChar() == 'x') {
        cout<<"Key 'x' is pressed"<<endl;
        keyPressed = 'x';
    }
    
    if (event.getChar() == '1') {
        cout<<"Key '1' is pressed"<<endl;
        keyPressed = '1';
    }
    if (event.getChar() == '2') {
        cout<<"Key '2' is pressed"<<endl;
        keyPressed = '2';
    }
    if (event.getChar() == '3') {
        cout<<"Key '3' is pressed"<<endl;
        keyPressed = '3';
    }
}




//find the difference between 2 frames + some useful image processing
void OpticalFlowApp::frameDifference(cv::Mat &outputImg)
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


void OpticalFlowApp::findOpticalFlow()
{
    if(!mSurface) return; //don't go through with the rest if we can't get a camera frame!
    
    //convert gl::Texturer to the cv::Mat(rix) --> Channel() -- converts, makes sure it is 8-bit
    cv::Mat curFrame = toOcv(Channel(*mSurface));
    
    if (ifbgSub){
//        cv::Mat outputFrame; //the method will put the results (eg. the foreground mask) into the output frame
        mBackgroundSubtract->apply(curFrame, curFrame);
        mCurrFrame = curFrame;
    }
    
    //if we have a previous sample, then we can actually find the optical flow.
    
    if( mPrevFrame.data ) {
        
        // pick new features once every SAMPLE_WINDOW_MOD frames, or the first frame
        
        //note: this means we are abandoning all our previous features every SAMPLE_WINDOW_MOD frames that we
        //had updated and kept track of via our optical flow operations.
        
        if( mFeatures.empty() || getElapsedFrames() % SAMPLE_WINDOW_MOD == 0 ){
            
            /*
             parameters for the  call to cv::goodFeaturesToTrack:
             curFrame - img,
             mFeatures - output of corners,
             MAX_FEATURES - the max # of features,
             0.005 - quality level (percentage of best found),
             3.0 - min distance
             
             note: its terrible to use these hard-coded values for the last two parameters. perhaps you will fix your projects.
             
             note: remember we're finding corners/edges using these functions
             */
            cv::goodFeaturesToTrack( curFrame, mFeatures, MAX_FEATURES, 0.005, 3.0 );
        }
        
        vector<float> errors; //there could be errors whilst calculating optical flow
        
        mPrevFeatures = mFeatures; //save our current features as previous one
        
        //This operation will now update our mFeatures & mPrevFeatures based on calculated optical flow patterns between frames UNTIL we choose all new features again in the above operation every SAMPLE_WINDOW_MOD frames. We choose all new features every couple frames, because we lose features as they move in and out frames and become occluded, etc.
        if( ! mFeatures.empty() )
            cv::calcOpticalFlowPyrLK( mPrevFrame, curFrame, mPrevFeatures, mFeatures, mFeatureStatuses, errors );
        
    }
    //set previous frame
    mPrevFrame = curFrame;
    
}


void OpticalFlowApp::draw(){
    gl::clear( Color( 0, 0, 0 ) ); //color the camera frame normally
    gl::color( 1, 1, 1, 1);
    
    if (mCurrFrame.data != NULL && ifbgSub){
        gl::color(1,1,1,0.5);
        gl::draw(gl::Texture::create(fromOcv(mCurrFrame))); //drawing the currentFrame
    }
    
    // When pressed 'd', execute frameDifferencing app
    if(keyPressed == 'd'){
        FrameDifferencing frameDifferencing;
        frameDifferencing.configuration('d', 20);
        frameDifferencing.countPixels(mFrameDifference);
        frameDifferencing.display();
    }
    
    // When pressed 'f', execute opticalFlow app with N = 20
    if(keyPressed == 'f'){
        OpticalFlow opticalFlow;
        opticalFlow.configuration('f', 20);
        opticalFlow.countFeatures(mFeatures);
        opticalFlow.display();
    }
    
    // run opticalFlow app with N = 10
    if(keyPressed == '1'){
        OpticalFlow opticalFlow;
        opticalFlow.configuration('f',10);
        opticalFlow.countFeatures(mFeatures);
        opticalFlow.display();
        
    }
    
    // run opticalFlow app with N = 20
    if(keyPressed == '2'){
        OpticalFlow opticalFlow;
        opticalFlow.configuration('f',20);
        opticalFlow.countFeatures(mFeatures);
        opticalFlow.display();
        
    }
    
    // run opticalFlow app with N = 96
    if(keyPressed == '3'){
        OpticalFlow opticalFlow;
        opticalFlow.configuration('f',96);
        opticalFlow.countFeatures(mFeatures);
        opticalFlow.display();
    }
    
    
    //This should be the default code from Courtney
    if(keyPressed == 'x'){
        //draw the camera frame
        if( mTexture )
        {
            gl::draw( mTexture );
        }
    
        // draw all the old points @ 0.5 alpha (transparency) as a circle outline
        gl::color( 1, 0, 0, 0.55 );
        for( int i=0; i<mPrevFeatures.size(); i++ )
            gl::drawStrokedCircle( fromOcv( mPrevFeatures[i] ), 3 );
    
    
        // draw all the new points @ 0.5 alpha (transparency)
        gl::color( 0, 0, 1, 0.5f );
        for( int i=0; i<mFeatures.size(); i++ )
            gl::drawSolidCircle( fromOcv( mFeatures[i] ), 3 );
    
        //draw lines from the previous features to the new features
        //you will only see these lines if the current features are relatively far from the previous
        gl::color( 0, 1, 0, 0.5f );
        gl::begin( GL_LINES );
        for( size_t idx = 0; idx < mFeatures.size(); ++idx ) {
            if( mFeatureStatuses[idx] ) {
                gl::vertex( fromOcv( mFeatures[idx] ) );
                gl::vertex( fromOcv( mPrevFeatures[idx] ) );
            }
        }
        gl::end();
    }
    
    
    
}

CINDER_APP( OpticalFlowApp, RendererGl )
