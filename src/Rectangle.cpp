//
//  Rectangle.cpp
//  Relief2
//
//  Created by Daniel Windham on 5/04/15.
//
//

#include "Rectangle.h"


Rectangle::Rectangle(float left, float top, float width, float height) :
        left(left), top(top), width(width), height(height) {
}

bool Rectangle::containsPoint(ofPoint &point) {
    return left < point.x && point.x < left + width && top < point.y && point.y < top + height;
}

void Rectangle::findNearestPointOnPerimeter(ofPoint &src, ofPoint &dst) {
    bool inXDomain = left < src.x && src.x < left + width;
    bool inYDomain = top < src.y && src.x < top + height;

    ofPoint candidates[4];

    // if the point shares an x or y coordinate with a perimeter point, we are only interested in
    // points vertically or horizontally offset from the src point
    if (inXDomain || inYDomain) {
        candidates[0] = ofPoint(src.x, top);
        candidates[1] = ofPoint(src.x, top + height);
        candidates[2] = ofPoint(left, src.y);
        candidates[3] = ofPoint(left + width, src.y);

    // else, we are only interested in the corner points
    } else {
        candidates[0] = ofPoint(left, top);
        candidates[1] = ofPoint(left + width, top);
        candidates[2] = ofPoint(left + width, top + height);
        candidates[3] = ofPoint(left, top + height);
    }

    // cycle through candidates to find the closest point
    dst.set(candidates[0]);
    float squareDistance = src.squareDistance(dst);
    for (int i = 1; i < 4; i++) {
        if (src.squareDistance(candidates[i]) < squareDistance) {
            dst.set(candidates[i]);
            squareDistance = src.squareDistance(dst);
        }
    }
}

Rectangle Rectangle::withInvertedTopAndBottom(){
    return Rectangle(left, top + height, width, -height);
}

Rectangle Rectangle::withInvertedLeftAndRight() {
    return Rectangle(left + width, top, -width, height);
}