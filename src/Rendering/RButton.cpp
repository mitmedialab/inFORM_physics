//
//  RButton.cpp
//  Relief2
//
//  Created by Daniel Leithinger on 3/16/13.
//
//

#include "RButton.h"

RButton::RButton(RButtonShapes shape, bool animated) {
    this->animated = animated;
    this->myShape = shape;
    /*scaleFactor.setDuration(0.3f);
    scaleFactor.animateFromTo(1.0, 0.0);
    scaleFact = 0;*/
}

void RButton::drawHeightMap() {
    
    if (render) {
        switch (myShape) {
            case square:
                drawSquareHeightMap();
                break;
                
            case circle:
                drawCircleHeightMap();
                break;
        }
    }
}

void RButton::drawGraphics() {
    if (render) {
        switch (myShape) {
            case square:
                drawSquareGraphics();
                break;
                
            case circle:
                drawCircleGraphics();
                break;
        }
    }
}

void RButton::drawCircleHeightMap() {
    ofFill();

    float circleScale = 0.5 + (scaleFact / 2);
    int wellRenderSteps = 2;
    
    int zStepSize = (size.getCurrentPosition().z * circleScale) / wellRenderSteps;
    int circleHeight = position.getCurrentPosition().z + zStepSize;
    
    for (int i = wellRenderSteps; i > 0; i -= 1) {
        ofSetColor(circleHeight);
        ofCircle(ofPoint((position.getCurrentPosition().x), (position.getCurrentPosition().y)), size.getCurrentPosition().x / wellRenderSteps * i  * circleScale);
        circleHeight += zStepSize;
    }
}

void RButton::drawCircleGraphics() {
    
    float circleScale = 0.5 + (scaleFact / 2);
    
    ofFill();
    //ofSetColor(50,50,50);
    if(isTouched){
        ofSetColor(0,255,0);
    }
    else {
        ofSetColor(50,50,255 * circleScale);
    }
    
    ofCircle(ofPoint((position.getCurrentPosition().x), (position.getCurrentPosition().y)), size.getCurrentPosition().x * circleScale);
}

void RButton::drawSquareHeightMap() {
    ofFill();
    
    //ofSetColor(position.getCurrentPosition().z * scaleFact);
    //ofRect(position.getCurrentPosition().x - PIN_PIXEL_SIZE, position.getCurrentPosition().y- PIN_PIXEL_SIZE, size.getCurrentPosition().x + (PIN_PIXEL_SIZE * 2), size.getCurrentPosition().y + (PIN_PIXEL_SIZE * 2));
    
    ofSetColor(position.getCurrentPosition().z);
    //ofSetColor((position.getCurrentPosition().z / 2) + position.getCurrentPosition().z / (2 * scaleFact));
    ofRect(position.getCurrentPosition().x, position.getCurrentPosition().y, size.getCurrentPosition().x, size.getCurrentPosition().y);
}

void RButton::drawSquareGraphics() {
    ofFill();
    ofSetColor(50,50,50);
    if(isTouched){
        ofSetColor(0,255,0);
    } else if (isHovered) {
        ofSetColor(0,0,255);
    }
    
    ofRect(position.getCurrentPosition().x, position.getCurrentPosition().y, size.getCurrentPosition().x, size.getCurrentPosition().y);
}

void RButton::update(float dt) {
    position.update(dt);
    size.update(dt);
    //scaleFactor.update(dt);
}

void RButton::touchTest(vector<ofPoint> &touchPoints){
    if(animated) {
        singleTouchTest2(touchPoints);
    }
    else {
        singleTouchTest(touchPoints);
    }
}

void RButton::singleTouchTest(vector<ofPoint> &touchPoints) {
    scaleFact = 1;
    
    isTouched = false;
    
    for (int i =0; i < touchPoints.size(); i++){
        //printf("x: %f, y: %f, z:%f", touchPoints[i].x, touchPoints[i].y, touchPoints[i].z);
        if(touchPoints[i].x > position.getCurrentPosition().x && touchPoints[i].x < (position.getCurrentPosition().x+ size.getCurrentPosition().x ) && touchPoints[i].y > position.getCurrentPosition().y && touchPoints[i].y < (position.getCurrentPosition().y + size.getCurrentPosition().y)){
            if(touchPoints[i].z < 2) { //touched
                isTouched = true;
            }
            else {
                isHovered = true;
            }
        }
    }
}

void RButton::singleTouchTest2(vector<ofPoint> &touchPoints) {
    
    float smallestDistance = 300;
    
    isTouched = false;
    isHovered = false;
    
    for (int i =0; i < touchPoints.size(); i++){
        
        // calculate the distance from the center
        ofPoint centerPosition = ofPoint(position.getCurrentPosition().x + (size.getCurrentPosition().x/2), position.getCurrentPosition().y + (size.getCurrentPosition().y/2));
        ofPoint touchPoint2D = ofPoint(touchPoints[i].x, touchPoints[i].y);
        float distance = centerPosition.distance(touchPoint2D);
        if (distance < smallestDistance) {
            touchPoint = touchPoints[i];
            smallestDistance = distance;
        }
        
        scaleFact = 0;
        if (distance < 300) {
            scaleFact = 1.0f - ((distance - 100) / 200.0f);
            if (distance < 100) {
                scaleFact = 1.0;
                if(touchPoint.z < 2) { //touched
                    isTouched = true;
                }
                else {
                    isHovered = true;
                }
            }
            
        }
        
        //printf("x: %f, y: %f, z:%f", touchPoints[i].x, touchPoints[i].y, touchPoints[i].z);
        /*if(touchPoint.x > position.getCurrentPosition().x - PIN_PIXEL_SIZE && touchPoint.x < (position.getCurrentPosition().x+ size.getCurrentPosition().x +  PIN_PIXEL_SIZE) && touchPoint.y > position.getCurrentPosition().y - PIN_PIXEL_SIZE && touchPoint.y < (position.getCurrentPosition().y+ size.getCurrentPosition().y + PIN_PIXEL_SIZE)){
            if(touchPoint.z < 2) { //touched
                isTouched = true;
            }
            else {
                isHovered = true;
            }
        }*/
    }
}

void RButton::multiTouchTest(vector<ofPoint> &touchPoints) {
    touchedLastFrame = isTouched;
    hoveredLastFrame = isHovered;
    isTouched = false;
    isHovered = false;
    
    for (int i =0; i < touchPoints.size(); i++){
        //printf("x: %f, y: %f, z:%f", touchPoints[i].x, touchPoints[i].y, touchPoints[i].z);
        if(touchPoints[i].x > position.getCurrentPosition().x - PIN_PIXEL_SIZE && touchPoints[i].x < (position.getCurrentPosition().x+ size.getCurrentPosition().x +  PIN_PIXEL_SIZE) && touchPoints[i].y > position.getCurrentPosition().y - PIN_PIXEL_SIZE && touchPoints[i].y < (position.getCurrentPosition().y+ size.getCurrentPosition().y + PIN_PIXEL_SIZE)){
            if(touchPoints[i].z < 2) { //touched
                isTouched = true;
            }
            else {
                isHovered = true;
            }
        }
    }
    
    if ((hoveredLastFrame || touchedLastFrame) != (isHovered || isTouched)) {
        float newValue = (isHovered || isTouched) ? 1.0 : 0.0;
        scaleFactor.animateTo(newValue);
    }
}

