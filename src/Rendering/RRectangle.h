//
//  RRectangle.h
//  Relief2
//
//  Created by Daniel Leithinger on 3/18/13.
//
//

#ifndef __Relief2__RRectangle__
#define __Relief2__RRectangle__

#include "RenderableObject.h"
#include <iostream>

class RRectangle : public RenderableObject {
public:
    void drawHeightMap();
    void drawGraphics();
    void update(float dt);
};

#endif /* defined(__Relief2__RRectangle__) */
