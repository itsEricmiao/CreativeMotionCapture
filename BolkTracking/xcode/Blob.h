//
//  Blob.h
//  BolkTracking
//
//  Created by Eric Miao on 10/15/20.
//


#ifndef Blob_h
#define Blob_h

#include <sstream>

class Blob
{
protected:
    cv::KeyPoint keyPoint; //where is the blob? & how big is it?
    int b_id; //what is the id?
//    int lastIndex; //used for tracking
public:
    Blob(cv::KeyPoint pt, int _id)
    {
        update(pt);
        b_id = _id;
//        lastIndex = -1;
    }
    
//changes the location of the blob -- useful for tracking
    void update(cv::KeyPoint pt)
    {
        keyPoint = pt;
    }
    
//draws the blob
    void draw()
    {
        ci::gl::color( 0.65, 0.5, 0.65, 0.5 );
        ci::gl::drawSolidCircle( ci::fromOcv( keyPoint.pt ), keyPoint.size );
        ci::gl::color( 1, 1, 1, 1);
        ci::gl::draw(drawText(),  ci::fromOcv( keyPoint.pt ) );
    };
    
//draws text to label the blobs
    ci::gl::Texture2dRef drawText()
    {
        ci::TextLayout simple;
        simple.setColor( ci::Color( 1, 0, 0.1f ) );
        
        std::stringstream str; str << b_id;
        simple.addLine( str.str() );
        return ci::gl::Texture2d::create( simple.render( true, false ) );
    };
    
//This function is helpful when tracking blobs
//    int getLastIndex()
//    {
//        return lastIndex;
//    }
    
};


#endif /* Blob_h */
