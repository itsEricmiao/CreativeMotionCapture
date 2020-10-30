
/*
 
 Programmer: Courtney Brown
 Date: c2020
 Notes: Demonstration of blob detection
 Purpose/Description:
 
 This program demonstrates simple blob detection.
 
 Uses:
 
 See brief tutorial here:
 https://www.learnopencv.com/blob-detection-using-opencv-python-c/

 Output/Drawing:
 Draws the result of simple blob detection.
 
 Instructions:
 Copy and paste this code into your cpp file.
 
 Run. Observe the results. Change some of the parameters.
 
 */

//#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/tracking.hpp>
#include "Osc.h" //add to send OSC

//includes for background subtraction
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/video.hpp>
#include "opencv2/features2d.hpp" //new include for this project
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Capture.h" //needed for capture
#include "cinder/Log.h" //needed to log errors
#include "CinderOpenCV.h"
#include "Blob.h"

#define SAMPLE_WINDOW_MOD 300
#define MAX_FEATURES 300
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define MINDIS 80

using namespace ci;
using namespace ci::app;
using namespace std;

//for networking
#define LOCALPORT 8887 //we just bind here to send.
#define DESTHOST "127.0.0.1" //this is sending to our OWN computer's IP address
#define DESTPORT 8888 //this is the port we are sending to -- take note. This will have to match in the next code.

//set up our OSC addresses
#define WHERE_OSC_ADDRESS "/MakeItArt/Where"
#define DOWN_OSC_ADDRESS "/MakeItArt/Down"
#define BLOB_OSC_ADDRESS "/MakeItArt/Blobs"
#define SIZE_OSC_ADDRESS "/MakeItArt/Size"
#define PREVB_OSC_ADDRESS "/MakeItArt/PrevBlobs"
#define MAP_OSC_ADDRESS "/MakeItArt/Map"
#define SPEED_OSC_ADDRESS "/MakeItArt/Speed"

class BlobTrackingApp : public App {
public:
    void setup() override;
    void blobTracking();
    void updateBlobList();
    void update() override;
    
    void keyDown( KeyEvent event ) override;
    void mouseDrag( MouseEvent event) override;
    void mouseDown( MouseEvent event) override;
    void mouseUp( MouseEvent event) override;
    
    void draw() override;
    BlobTrackingApp();
    void printPrevAndCurKeys();
    ci::vec2 curMousePosLastDown;
    bool isMouseDown;


protected:
    CaptureRef                 mCapture; //the camera capture object
    gl::TextureRef             mTexture; //current camera frame in opengl format
    
    SurfaceRef                  mSurface; //current camera frame in cinder format
    
    cv::Ptr<cv::SimpleBlobDetector>     mBlobDetector; //the object that does the blob detection
    std::vector<cv::KeyPoint>   mKeyPoints; //the center points of the blobs (current, previous, and before that) & how big they are.
    std::vector<int> mMapPrevToCurKeypoints; // maps where points in (mKeypoints) to where they were in (mPrevKeypoints)
    

    
    cv::Mat                     mCurFrame, mBackgroundSubtracted; //current frame & frame with background subtracted in OCV format
    cv::Mat                     mSavedFrame; //the frame saved for use for simple background subtraction
    
    cv::Ptr<cv::BackgroundSubtractor> mBackgroundSubtract; //the background subtraction object that does subtractin'
    
    std::vector<Blob>           mBlobs; ///the blobs found in the current frame
    std::vector<cv::KeyPoint>   mPrevKeypoints;
    
    enum BackgroundSubtractionState {NONE=0, OPENCV=2, SAVEDFRAME=3};
    BackgroundSubtractionState mUseBackgroundSubtraction;
    
    void blobDetection(BackgroundSubtractionState useBackground); //does our blob detection
    void createBlobs(); //creates the blob objects for each keypoint
    int newBlobID; //the id to assign a new blob.
    void getSpeed();
    float speed;
    
    osc::SenderUdp                mSender; //sends the OSC via the UDP protocol
    void sendOSC(std::string addr, float x, float y); //sending the OSC values
    void sendOSC(std::string addr, float down);//just one -- this is inelegant but effective for now
    void sendOSC(std::string addr, Blob blob); // sending the blobs
    void sendOSC(std::string addr, int size); // sending the size
    void sendOSC(std::string addr, cv::KeyPoint key);
    void sendOSC(std::string addr, vector<int> map);
};

BlobTrackingApp::BlobTrackingApp() : mSender(LOCALPORT, DESTHOST, DESTPORT) //initializing our class variables
{
    
}

//sets the mouse drag values
void BlobTrackingApp::mouseDrag( MouseEvent event)
{
    curMousePosLastDown = event.getPos();
    isMouseDown = true;
    
}
//sets the mouse down values
void BlobTrackingApp::mouseDown( MouseEvent event)
{
    curMousePosLastDown = event.getPos();
    isMouseDown = true;
}

//sets the mouse up values
void BlobTrackingApp::mouseUp( MouseEvent event)
{
    isMouseDown = false;
}

void BlobTrackingApp::sendOSC(std::string addr, float x, float y) //sending the OSC values
{
    osc::Message msg;
    msg.setAddress(addr); //sets the address
    msg.append(x);
    msg.append(y);
    cout<<"sending x,y:"<<x<<"|"<<y<<endl;
    mSender.send(msg);
}

void BlobTrackingApp::sendOSC(std::string addr, float down) //sending the OSC values
{
    osc::Message msg;
    msg.setAddress(addr); //sets the address
    msg.append(down);
    cout<<"sending mouse down:"<<down<<endl;
    mSender.send(msg);
}

void BlobTrackingApp::sendOSC(std::string addr, Blob blob) //sending the OSC values
{
    osc::Message msg;
    msg.setAddress(addr); //sets the address
    cv::KeyPoint pos = blob.getKeyPoint();
    float id = blob.getID();
    float x = pos.pt.x;
    float y = pos.pt.y;
    msg.append(id);
    msg.append(x);
    msg.append(y);
    mSender.send(msg);
}

void BlobTrackingApp::sendOSC(std::string addr, int size){
    osc::Message msg;
    msg.setAddress(addr); //sets the address
    msg.append(size);
    mSender.send(msg);
}

void BlobTrackingApp::sendOSC(std::string addr, cv::KeyPoint key){
    osc::Message msg;
    msg.setAddress(addr); //sets the address
    float x = key.pt.x;
    float y = key.pt.y;
    msg.append(x);
    msg.append(y);
    mSender.send(msg);
}


void BlobTrackingApp::sendOSC(std::string addr, vector<int> map){
    osc::Message msg;
    msg.setAddress(addr); //sets the address
    for(int i = 0; i < map.size(); i++){
        msg.append(map[i]);
    }
    mSender.send(msg);
}

void BlobTrackingApp::setup()
{
    
    //set up our OSC sender and bind it to our local port
    try{
        mSender.bind();
    }
    catch( osc::Exception &e){
        CI_LOG_E( "Error binding" << e.what() << " val: " << e.value() );
        quit();
    }
    
    //set up our camera
    try {
        mCapture = Capture::create(WINDOW_WIDTH, WINDOW_HEIGHT); //first default camera
        mCapture->start();
    }
    catch( ci::Exception &exc)
    {
        CI_LOG_EXCEPTION( "Failed to init capture ", exc );
    }
    
    //setup the blob detector
    // Setup SimpleBlobDetector parameters.
    cv::SimpleBlobDetector::Params params;
    
    // Change thresholds
    //    params.minThreshold = 10;
    //    params.maxThreshold = 200;
    
    // Filter by Circularity - how circular
    params.filterByCircularity = false;
    params.maxCircularity = 0.2;
    
    // Filter by Convexity -- how convex
    params.filterByConvexity = false;
    params.minConvexity = 0.87;
    
    // Filter by Inertia ?
    params.filterByInertia = false;
    params.minInertiaRatio = 0.01;
    
    params.minDistBetweenBlobs = 300.0f;
    
    params.filterByColor = false;
    
    params.filterByArea = true;
    params.minArea = 200.0f;
    params.maxArea = 1000.0f;
    
    //create the blob detector with the above parameters
    mBlobDetector = cv::SimpleBlobDetector::create(params);
    
    //our first available id is 0
    newBlobID = 0;
    
    //use MOG2 -- guassian mixture algorithm to do background subtraction
    mBackgroundSubtract = cv::createBackgroundSubtractorMOG2();
    
    mUseBackgroundSubtraction = BackgroundSubtractionState::NONE;
    mBackgroundSubtracted.data = NULL;
    
    
}

void BlobTrackingApp::keyDown( KeyEvent event )
{
    if( event.getChar() == '1')
    {
        mUseBackgroundSubtraction = BackgroundSubtractionState::NONE;
    }
    if( event.getChar() == '2')
    {
        mUseBackgroundSubtraction = BackgroundSubtractionState::OPENCV;
    }
    if( event.getChar() == '3')
    {
        mUseBackgroundSubtraction = BackgroundSubtractionState::SAVEDFRAME;
        std::cout << "Saving current frame as background!\n";
        mSavedFrame = mCurFrame;
    }
}

//this function detects the blobs.
void BlobTrackingApp::blobDetection(BackgroundSubtractionState useBackground = BackgroundSubtractionState::NONE)
{
    if(!mSurface) return;
    
    cv::Mat frame;
    
    //using the openCV background subtraction
    if(useBackground == BackgroundSubtractionState::OPENCV)
    {
        mBackgroundSubtract->apply(mCurFrame, mBackgroundSubtracted);
        frame = mBackgroundSubtracted;
    }
    else if( useBackground == BackgroundSubtractionState::SAVEDFRAME )
    {
        if(mSavedFrame.data)
        {
            cv::Mat outFrame;
            
            //use frame-differencing to subtract the background
            cv::GaussianBlur(mCurFrame, outFrame, cv::Size(11,11), 0);
            cv::GaussianBlur(mSavedFrame, mBackgroundSubtracted, cv::Size(11,11), 0);
            cv::absdiff(outFrame, mBackgroundSubtracted, mBackgroundSubtracted);
            
            cv::threshold(mBackgroundSubtracted, mBackgroundSubtracted, 25, 255, cv::THRESH_BINARY);
            
            frame = mBackgroundSubtracted;
        }
        else{
            std::cerr << "No background frame has been saved!\n"; //the way the program is designed, this should happen
        }
    }
    else
    {
        frame = mCurFrame;
    }
    
    //note the parameters: the frame that you would like to detect the blobs in - an input frame
    //& 2nd, the output -- a vector of points, the center points of each blob.
    
    mPrevKeypoints = mKeyPoints; // save the previous mKeyPoints into a variable
    
    mBlobDetector->detect(frame, mKeyPoints);
   
    
//
}

void BlobTrackingApp::update()
{
    if(mCapture && mCapture->checkNewFrame()) //is there a new frame???? (& did camera get created?)
    {
        mSurface = mCapture->getSurface();
        
        if(! mTexture)
            mTexture = gl::Texture::create(*mSurface);
        else
            mTexture->update(*mSurface);
    }
    if(!mSurface) return; //we do nothing if there is no new frame
    
    mCurFrame = toOcv(Channel(*mSurface));
    
    

    sendOSC(WHERE_OSC_ADDRESS, (float)curMousePosLastDown.x/(float)getWindowWidth(),(float)curMousePosLastDown.y/(float)getWindowHeight());
    sendOSC(DOWN_OSC_ADDRESS, isMouseDown);
    int size = mKeyPoints.size();
    sendOSC(SIZE_OSC_ADDRESS, size);
    
    for(int i = 0; i < mPrevKeypoints.size(); i++){
        sendOSC(PREVB_OSC_ADDRESS, mPrevKeypoints[i]);
    }
    
    for(int i = 0; i < mBlobs.size(); i++){
        sendOSC(BLOB_OSC_ADDRESS, mBlobs[i]);
    }
    sendOSC(MAP_OSC_ADDRESS, mMapPrevToCurKeypoints);
    sendOSC(SPEED_OSC_ADDRESS, speed);
    
    //update all our blob info
    blobDetection(mUseBackgroundSubtraction);
    
    //create blob objects from keypoints
    createBlobs();
    mMapPrevToCurKeypoints.clear();
    blobTracking();
    getSpeed();
    updateBlobList();
    
    
}


void BlobTrackingApp::blobTracking(){

//  Find the closest point (point with minimum distance) in mPrevKeypoints for each keypoint in mKeypoints.
    for(int i = 0; i < mKeyPoints.size(); i++){
        int index = -1;
        int min = 10000;
        for(int j = 0; j < mPrevKeypoints.size(); j++){
            cv::KeyPoint curPos = mKeyPoints[i];
            cv::KeyPoint prevPos = mPrevKeypoints[j];
            float distance = ci::distance(fromOcv(curPos.pt), fromOcv(prevPos.pt));
            if(distance <= MINDIS && distance < min){
                min = distance;
                index = i;
            }
        }
        if(index != -1){
            cout<<"Match: "<<mKeyPoints[i].pt<<"With: "<<mPrevKeypoints[index].pt<<endl;
        }
        mMapPrevToCurKeypoints.push_back(index);
    }
    
    
        std::cout << "*********************\n";
        std::cout << "mMapPrevToCurKeypoints: " << mMapPrevToCurKeypoints.size() << std::endl ;
        for(int i=0; i<mMapPrevToCurKeypoints.size(); i++){
            std::cout <<mMapPrevToCurKeypoints[i] <<"-";
        }
        std::cout << std::endl;
        std::cout << "mKeyPoints: "  ;
        for(int k=0; k<mKeyPoints.size(); k++){
            std::cout <<mKeyPoints[k].pt.x << "," <<mKeyPoints[k].pt.y << "   ";
        }
        
        std::cout << std::endl;
        std::cout << "mPrevKeyPoints: "  ;
        for(int k=0; k<mPrevKeypoints.size(); k++){
            std::cout  << mPrevKeypoints[k].pt.x << "," <<mPrevKeypoints[k].pt.y << "  ";
        }
        std::cout << std::endl;
        std::cout << " KeyPoints: " << mKeyPoints.size() << std::endl;
        std::cout << " PrevKeyPoints: " << mPrevKeypoints.size() << std::endl;
        std::cout << "*********************\n";
}

void BlobTrackingApp::updateBlobList(){
    std::vector<Blob> prevBlobs;
    prevBlobs = mBlobs;
    mBlobs.clear();
    for(int i = 0; i < mMapPrevToCurKeypoints.size(); i++){
        if (mMapPrevToCurKeypoints[i] == -1){
            //  then create a new Blob and add to mBlobs
            mBlobs.push_back(Blob(mKeyPoints[i], newBlobID));
            newBlobID++;
        }else{
            // access from the prevBlobs using the value from mMapPrevToCurKeypoints.
            // update that old blob with the new keypoint from mKeyPoints
            // Then add to mBlobs.
            for(int j = 0; j < prevBlobs.size(); j++){
                if(prevBlobs[j].getID() == mMapPrevToCurKeypoints[i]){
                    prevBlobs[j].update(mKeyPoints[i]);
                    mBlobs.push_back(Blob(prevBlobs[j].getKeyPoint(), newBlobID));
                    newBlobID++;
                }
            }
        }
    }
    
}

void BlobTrackingApp::createBlobs()
{
    mBlobs.clear(); //create a new list of blobs each time
    newBlobID = 0; //reset here - since we're not keeping track of blobs yet!
    
    for(int i=0; i<mKeyPoints.size(); i++)
    {
        mBlobs.push_back(Blob(mKeyPoints[i], newBlobID));
        newBlobID++;
    }
}

void BlobTrackingApp::getSpeed(){
    for(int i=0; i< mMapPrevToCurKeypoints.size(); i++){
        speed =0;
        int ind=mMapPrevToCurKeypoints[i];
        //if value is -1, calculate velocity
        if(ind!=-1)
        {
            int x1= mKeyPoints[i].pt.x;
            int x2=  mPrevKeypoints[i].pt.x;
            int y1= mKeyPoints[i].pt.y;
            int y2=  mPrevKeypoints[i].pt.y;
            int xs=(x1-x2);
            int ys=(y1-y2);
            //calculate velocity
            speed =(sqrt((pow(xs, 2)+(pow(ys, 2)))));
        }
    }
}

void BlobTrackingApp::draw()
{
    gl::clear( Color( 0, 0, 0 ) );
    
    gl::color( 1, 1, 1, 1 );

    //draw what image we are detecting the blobs in
    if( mBackgroundSubtracted.data && mUseBackgroundSubtraction )
    {
        gl::draw( gl::Texture::create(fromOcv(mBackgroundSubtracted)) );
    }
    else if( mTexture )
    {
        gl::draw(mTexture);
    }
    
    //draw the blobs
    for(int i=0; i<mBlobs.size(); i++)
    {
        mBlobs[i].draw();
    }
}


void BlobTrackingApp::printPrevAndCurKeys(){
    //--------------------------------- Test -----------------------------------
        cout<<"prev keypoints: "<<endl;
        for(int i = 0; i < mPrevKeypoints.size(); i++)
        {
            cout<<mPrevKeypoints[i].pt<<endl;
        }
        cout<<"current keypoints: "<<endl;
        for(int j = 0; j < mPrevKeypoints.size(); j++)
        {
            cout<<mKeyPoints[j].pt<<endl;
        }
        cout<<endl;
        cout<<endl;
    //---------------------------------------------------------------------------
}

CINDER_APP( BlobTrackingApp, RendererGl,
           []( BlobTrackingApp::Settings *settings ) //note: this part is to fix the display after updating OS X 1/15/18
           {
               settings->setHighDensityDisplayEnabled( true );
               settings->setTitle("Blob Tracking Example");
               settings->setWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
               //               settings->setFrameRate(FRAMERATE); //set fastest framerate
               })




