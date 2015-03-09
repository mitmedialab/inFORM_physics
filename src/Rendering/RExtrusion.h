//
//  RExtrusion.h
//  Relief2
//
//  Created by TMG on 11/9/13.
//
//

#ifndef __Relief2__RExtrusion__
#define __Relief2__RExtrusion__

#include "RenderableObject.h"

#include <vector>

class RExtrusion : public RenderableObject {
    std::vector<ofVec3f> points;
    float height;
    ofColor color;
public:
    RExtrusion(ofColor color);

    virtual void drawHeightMap();
    virtual void drawGraphics();
    virtual void update(float dt);

    void addPoint(ofVec3f point);
    void updateHeight(float height);
    void setColor(ofColor color);
};

#endif /* defined(__Relief2__RExtrusion__) */
