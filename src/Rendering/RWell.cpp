//
//  RWell.cpp
//  Relief2
//
//  Created by Daniel Leithinger on 3/16/13.
//
//

#include "RWell.h"
#include "Constants.h"

RWell::RWell() {
    wellRenderSteps = WELL_RENDER_STEPS;
}

void RWell::drawHeightMap() {
    if (render) {
    
        ofFill();
        
        int zStepSize = size.getCurrentPosition().z / wellRenderSteps;
        int circleHeight = position.getCurrentPosition().z + zStepSize;
        
        for (int i = wellRenderSteps; i > 0; i -= 1) {
            ofSetColor(circleHeight);
            ofCircle(ofPoint((position.getCurrentPosition().x), (position.getCurrentPosition().y)), size.getCurrentPosition().x / wellRenderSteps * i);
            circleHeight += zStepSize;
        }
    }
}

void RWell::drawGraphics() {
    if (render) {
    
        ofFill();
        if(this->useColor)
            ofSetColor(color);
        else
            ofSetColor(255);
        ofCircle(ofPoint((position.getCurrentPosition().x), (position.getCurrentPosition().y)), size.getCurrentPosition().x);
        
    }
}

void RWell::update(float dt) {
    position.update(dt);
    size.update(dt);
}