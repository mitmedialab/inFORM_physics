//
//  RTouchElement.h
//  Relief2
//
//  Created by Sean Follmer on 3/28/13.
//
//

#ifndef __Relief2__RTouchElement__
#define __Relief2__RTouchElement__

#include "RenderableObject.h"

enum TouchElementType { horizontal,vertical, pad};

class RTouchElement : public RenderableObject {
public:
    void drawHeightMap();
    void drawGraphics();
    void update(float dt);
    void touchTest(vector<ofPoint> & touchPoints);
    ofPoint getValue();
    
    ofPoint value;
    
    TouchElementType type = pad;
    
};

#endif /* defined(__Relief2__RTouchElement__) */
