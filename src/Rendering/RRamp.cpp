//
//  RRamp.cpp
//  Relief2
//
//  Created by Daniel Leithinger on 3/24/13.
//
//

#include "RRamp.h"

RRamp::RRamp() {
    this->renderRampRails = true;
    this->render = true;
}

void RRamp::drawHeightMap() {
    
    if(render) {
    ofVec2f highPoint2dPos(highPoint.getCurrentPosition().x, highPoint.getCurrentPosition().y);
    ofVec2f lowPoint2dPos(lowPoint.getCurrentPosition().x, lowPoint.getCurrentPosition().y);
    
    float rotation2d = ofVec2f(highPoint2dPos - lowPoint2dPos).angle(ofVec2f(1,0));
    
    //printf("rot: %f \n", rotation2d);
    float distance2d = highPoint2dPos.distance(lowPoint2dPos);
    float distanceHeight = highPoint.getCurrentPosition().z - lowPoint.getCurrentPosition().z;
    float stepSize = distance2d / (distanceHeight + 1);
    
    ofFill();
    ofPushMatrix();
        ofTranslate(lowPoint2dPos.x, lowPoint2dPos.y);
        ofRotate(-rotation2d, 0, 0, 1);
        for (int i = 0; i <= distanceHeight; i++) {
            ofSetColor(lowPoint.getCurrentPosition().z + i + 30);
            if (renderRampRails) ofRect(i * stepSize, - 60, stepSize, 120);
            ofSetColor(lowPoint.getCurrentPosition().z + i);
            ofRect(i * stepSize, - 30, stepSize, 60);
        }
    ofPopMatrix();
    }
    
    //ofRect(position.getCurrentPosition().x, position.getCurrentPosition().y, size.getCurrentPosition().x, size.getCurrentPosition().y);
}

void RRamp::drawGraphics() {
    if(render) {
        ofVec2f highPoint2dPos(highPoint.getCurrentPosition().x, highPoint.getCurrentPosition().y);
        ofVec2f lowPoint2dPos(lowPoint.getCurrentPosition().x, lowPoint.getCurrentPosition().y);
        
        float rotation2d = ofVec2f(highPoint2dPos - lowPoint2dPos).angle(ofVec2f(1,0));
        
        //printf("rot: %f \n", rotation2d);
        float distance2d = highPoint2dPos.distance(lowPoint2dPos);
        float distanceHeight = highPoint.getCurrentPosition().z - lowPoint.getCurrentPosition().z;
        float stepSize = distance2d / (distanceHeight + 1);
        
        ofFill();
        ofPushMatrix();
        ofTranslate(lowPoint2dPos.x, lowPoint2dPos.y);
        ofRotate(-rotation2d, 0, 0, 1);
        for (int i = 0; i <= distanceHeight; i++) {
            
            ofSetColor(0,0,lowPoint.getCurrentPosition().z + i + 30);
            if (renderRampRails) ofRect(i * stepSize, - 60, stepSize, 120);
            ofSetColor(0,0,lowPoint.getCurrentPosition().z + i);
            ofRect(i * stepSize, - 30, stepSize, 60);
        }
        ofPopMatrix();
    }
    
}

void RRamp::update(float dt) {
    position.update(dt);
    size.update(dt);
    highPoint.update(dt);
    lowPoint.update(dt);
}