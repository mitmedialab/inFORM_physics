//
//  RFunction.h
//  Relief2
//
//  Created by TMG on 11/9/13.
//
//

#ifndef __Relief2__RFunction__
#define __Relief2__RFunction__

#include "RenderableObject.h"
#include "Constants.h"

class RFunction : public RenderableObject {
    float prev[RELIEF_SIZE_X][RELIEF_SIZE_Y];
    float curr[RELIEF_SIZE_X][RELIEF_SIZE_Y];
    float next[RELIEF_SIZE_X][RELIEF_SIZE_Y];
    unsigned short currFunc;
    vector<ofImage> images;
    ofxAnimatableFloat anim;
    
public:
    RFunction();
    
    virtual void drawHeightMap();
    virtual void drawGraphics();
    virtual void drawEquation();
    virtual void update(float dt);
    
    void nextFunction();
};

#endif /* defined(__Relief2__RFunction__) */
