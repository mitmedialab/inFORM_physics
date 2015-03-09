//
//  RColorPicker.h
//  Relief2
//
//  Created by TMG on 10/1/13.
//
//

#ifndef __Relief2__RColorPicker__
#define __Relief2__RColorPicker__

#include <iostream>

#include "RenderableObject.h"

class RColorPicker: public RenderableObject {
    bool currColor;
    bool nextDirection;
    
    ofRectangle regButton;
    ofRectangle regPicker;
    
    ofVec3f geomButton[11][11];
    ofVec3f geomPicker[11][11];
    ofVec3f geomBlended[11][11];
    
    ofImage imgButton1;
    ofImage imgPicker;
    ofImage imgButton2;
    
    ofxAnimatableFloat anim;
    
    void setup();
public:
    RColorPicker();

    virtual void drawHeightMap();
    virtual void drawGraphics();
    virtual void update(float dt);
    
    void Go();

};


#endif /* defined(__Relief2__RColorPicker__) */
