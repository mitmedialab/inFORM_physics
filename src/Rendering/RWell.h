//
//  RWell.h
//  Relief2
//
//  Created by Daniel Leithinger on 3/16/13.
//
//

#ifndef __Relief2__RWell__
#define __Relief2__RWell__

#define WELL_RENDER_STEPS 5

#include "RenderableObject.h"

class RWell : public RenderableObject {
public:
    int  wellRenderSteps;
    
    RWell();
    void drawHeightMap();
    void drawGraphics();
    void update(float dt);

};

#endif /* defined(__Relief2__RWell__) */
