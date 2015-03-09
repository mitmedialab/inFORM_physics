//
//  RRipple.cpp
//  Relief2
//
//  Created by Daniel Leithinger on 3/16/13.
//
//

#include "RRipple.h"
#include "Constants.h"

RRipple::RRipple() {
    dx = (TWO_PI / 1500.0) * RIPPLE_STEPS;
    tetha = 0.0f;
}

void RRipple::drawHeightMap() {
    if (render) {
        
        ofFill();
        
        x = tetha;
        
        for (int i = RIPPLE_STEPS - 1; i >= 0; i--) {
            ofSetColor(80 + (sin(x) * 40));
            ofCircle(this->position.getCurrentPosition().x, this->position.getCurrentPosition().y, this->size.getCurrentPosition().x / RIPPLE_STEPS * i);
            x += dx;
        }
    }
}

void RRipple::drawGraphics() {
    if (render) {
        
        ofFill();
        
    }
}

void RRipple::update(float dt) {
    position.update(dt);
    size.update(dt);
    tetha += (dt * 5);
}