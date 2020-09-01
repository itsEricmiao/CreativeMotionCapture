#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class FrameDifferencingApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void FrameDifferencingApp::setup()
{
    
}

void FrameDifferencingApp::mouseDown( MouseEvent event )
{
}

void FrameDifferencingApp::update()
{
}

void FrameDifferencingApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( FrameDifferencingApp, RendererGl )
