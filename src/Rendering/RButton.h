//
//  RButton.h
//  Relief2
//
//  Created by Daniel Leithinger on 3/16/13.
//
//

#ifndef __Relief2__RButton__
#define __Relief2__RButton__

#include "RenderableObject.h"

enum RButtonShapes {
    square, circle
};

class RButton : public RenderableObject {
public:
    RButton(RButtonShapes shape, bool animated);
    void drawHeightMap();
    void drawGraphics();
    void drawCircleHeightMap();
    void drawCircleGraphics();
    void drawSquareHeightMap();
    void drawSquareGraphics();
    
    void update(float dt);
    void touchTest(vector<ofPoint> & touchPoints);
    void singleTouchTest(vector<ofPoint> &touchPoints);
    void singleTouchTest2(vector<ofPoint> &touchPoints);
    void multiTouchTest(vector<ofPoint> &touchPoints);
    
    bool animated;
    
    //bool growOnHover = true;
    ofxAnimatableFloat scaleFactor;
    bool touchedLastFrame = false;
    bool hoveredLastFrame = false;
    float scaleFact;

    RButtonShapes myShape = circle;
    
    // storing a single touch point, in case we have none
    ofPoint touchPoint;
    
};

#endif /* defined(__Relief2__RButton__) */
