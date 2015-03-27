//
//  StaticManipulator.h
//  Relief2
//
//  Created by Daniel Windham on 3/18/15.
//
//

#ifndef __Relief2__StaticManipulator__
#define __Relief2__StaticManipulator__

#include "RenderableObject.h"
#include "RWell.h"
#include "RRectangle.h"
#include "RRamp.h"
#include "RBitmap.h"
#include "KinectTracker.h"
#include "ofFbo.h"


enum Direction {UP, DOWN, LEFT, RIGHT};


class StaticManipulator : public RenderableObject {
public:
    
    StaticManipulator(KinectTracker *tracker);
    
    void drawHeightMap();
    void drawGraphics();
    void update(float dt);
    
    void keyPressed(int key);

private:
    KinectTracker *kinectTracker;
    ofFbo pinHeightMapImage; //FBO where we render height map

    Direction currentDirection;

    void drawDirectionMover();
    void drawFence();

};

#endif /* defined(__Relief2__StaticManipulator__) */