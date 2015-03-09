//
//  MarbleMachine.h
//  Relief2
//
//  Created by Daniel Leithinger on 3/23/13.
//
//

#ifndef __Relief2__MarbleMachine__
#define __Relief2__MarbleMachine__

#include "RenderableObject.h"
#include "RWell.h"
#include "RRectangle.h"
#include "RRamp.h"
#include "RBitmap.h"


class MarbleMachine : public RenderableObject {
public:
    
    MarbleMachine();
    
    void drawHeightMap();
    void drawGraphics();
    void update(float dt);
    
    void moveToNextState();
    
    void keyPressed(int key);
    
private:
    RBitmap * mamBase;
    
    vector< RenderableObject *> myMamElements;
    RWell * marbleMoveWell;
    RRectangle * marbleContainerHole;
    
};

#endif /* defined(__Relief2__MarbleMachine__) */
