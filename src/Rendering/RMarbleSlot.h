//
//  RMarbleSlot.h
//  Relief2
//
//  Created by Daniel Leithinger on 3/26/13.
//
//

#ifndef __Relief2__RMarbleSlot__
#define __Relief2__RMarbleSlot__

#include "RenderableObject.h"
#include "RRectangle.h"
#include "RRamp.h"
#include "RWell.h"
#include <iostream>

class RMarbleSlot : public RenderableObject {
public:
    
    RRectangle * playMessageSlot;
    //RWell * playMoveWell;
    
    RRamp * playMoveRamp1;
    RRamp * playMoveRamp2;
    
    RMarbleSlot(ofPoint position, ofPoint size);
    void drawHeightMap();
    void drawGraphics();
    void update(float dt);
    
    void expandPlaySlot(float delay, float duration);
    void retractPlaySlot(float delay, float duration);
    void showStartWell(float delay, float duration);
    void startPlayBack(float delay, float duration, float startPercentPosition);
};

#endif /* defined(__Relief2__RMarbleSlot__) */
