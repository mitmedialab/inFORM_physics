//
//  HybridTokens.h
//  Relief2
//
//  Created by Daniel Windham on 3/18/15.
//
//

#ifndef __Relief2__HybridTokens__
#define __Relief2__HybridTokens__

#include "RenderableObject.h"
#include "RWell.h"
#include "RRectangle.h"
#include "RRamp.h"
#include "RBitmap.h"
#include "KinectTracker.h"
#include "ofFbo.h"


class HybridTokens : public RenderableObject {
public:
    
    HybridTokens(KinectTracker *tracker);
    
    void drawHeightMap();
    void drawGraphics();
    void update(float dt);
    
    void keyPressed(int key);
    
private:
    KinectTracker *kinectTracker;
    ofFbo pinHeightMapImage; //FBO where we render height map
    
};

#endif /* defined(__Relief2__HybridTokens__) */