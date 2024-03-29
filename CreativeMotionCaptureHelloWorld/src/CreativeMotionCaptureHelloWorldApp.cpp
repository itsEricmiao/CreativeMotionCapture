#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"


#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/tracking.hpp>

#include "CinderOpenCV.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class CreativeMotionCaptureHelloWorldApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void CreativeMotionCaptureHelloWorldApp::setup()
{
}

void CreativeMotionCaptureHelloWorldApp::mouseDown( MouseEvent event )
{
}

void CreativeMotionCaptureHelloWorldApp::update()
{
}

void CreativeMotionCaptureHelloWorldApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( CreativeMotionCaptureHelloWorldApp, RendererGl )
