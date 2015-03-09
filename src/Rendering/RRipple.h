//
//  RWell.h
//  Relief2
//
//  Created by Daniel Leithinger on 3/16/13.
//
//

#ifndef __Relief2__RRipple__
#define __Relief2__RRipple__

#define RIPPLE_STEPS 60

#include "RenderableObject.h"

class RRipple : public RenderableObject {
    float dx;
    float x;
    float tetha;
public:
    RRipple();
    
    void drawHeightMap();
    void drawGraphics();
    void update(float dt);
};

#endif /* defined(__Relief2__RRipple__) */
