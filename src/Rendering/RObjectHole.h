//
//  RObjectHole
//  Relief2
//
//  Created by Daniel Leithinger on 3/16/13.
//
//

#ifndef __Relief2__RObjectHole__
#define __Relief2__RObjectHole__

#include "RenderableObject.h"

class RObjectHole : public RenderableObject {
public:
    void drawHeightMap();
    void drawGraphics();
    void update(float dt);
    void objectTest(vector<ofPoint> & objectCentroids);
};

#endif /* defined(__Relief2__RObjectHole__) */
