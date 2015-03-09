//
//  RRectangle.cpp
//  Relief2
//
//  Created by Daniel Leithinger on 3/18/13.
//
//

#include "RRectangle.h"

void RRectangle::drawHeightMap() {
    
    if (render) {
        ofFill();
        ofSetColor(position.getCurrentPosition().z);
        ofRect(position.getCurrentPosition().x, position.getCurrentPosition().y, size.getCurrentPosition().x, size.getCurrentPosition().y);
    }
}

void RRectangle::drawGraphics() {
    if (render) {
        ofFill();
        if (useColor) {
            ofSetColor(color);
        }
        else {
            ofSetColor(position.getCurrentPosition().z);
        }
        ofRect(position.getCurrentPosition().x, position.getCurrentPosition().y, size.getCurrentPosition().x, size.getCurrentPosition().y);
    }
}

void RRectangle::update(float dt) {
    position.update(dt);
    size.update(dt);
}