//
//  Utils.cpp
//  Relief2
//
//  Created by Daniel Windham on 4/28/15.
//
//

#include "Utils.h"



Rectangle::Rectangle(float left, float top, float width, float height) :
        left(left), top(top), width(width), height(height) {
}

bool Rectangle::containsPoint(ofPoint &point) {
    return left < point.x && point.x < left + width && top < point.y && point.y < top + height;
}


double clockInSeconds() {
    return clock() / (double) CLOCKS_PER_SEC;
}