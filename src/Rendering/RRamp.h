//
//  RRamp.h
//  Relief2
//
//  Created by Daniel Leithinger on 3/24/13.
//
//

#ifndef __Relief2__RRamp__
#define __Relief2__RRamp__

#include "RenderableObject.h"

class RRamp : public RenderableObject {
public:
    bool renderRampRails;
    
    ofxAnimatableOfPoint highPoint;
    ofxAnimatableOfPoint lowPoint;
    
    RRamp();
    void drawHeightMap();
    void drawGraphics();
    void update(float dt);
};

#endif /* defined(__Relief2__RRamp__) */
