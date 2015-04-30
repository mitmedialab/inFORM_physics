//
//  Utils.h
//  Relief2
//
//  Created by Daniel Windham on 4/28/15.
//
//


#ifndef __Relief2__Utils__
#define __Relief2__Utils__


#include <ctime>
#include "ofMain.h"


class Rectangle {
public:
    Rectangle(float left=0, float top=0, float width=0, float height=0);
    bool containsPoint(ofPoint &point);
    void findNearestPointOnPerimeter(ofPoint &src, ofPoint &dst);

    float left;
    float top;
    float width;
    float height;
};


double clockInSeconds();
bool pointIsInsideRect(ofPoint &point, Rectangle &rect);

#endif /* defined(__Relief2__Utils__) */