//
//  RMarbleSlot.cpp
//  Relief2
//
//  Created by Daniel Leithinger on 3/26/13.
//
//

#include "RMarbleSlot.h"

RMarbleSlot::RMarbleSlot(ofPoint position, ofPoint size) {
    this->position.setPosition(position);
    this->size.setPosition(size);
    this->render = true;
    
    playMessageSlot = new RRectangle();
    playMessageSlot->render = false;
    
    //playMoveWell = new RWell();
    
    playMoveRamp1 = new RRamp();
    playMoveRamp1->renderRampRails = false;
    playMoveRamp1->render = false;
    
    playMoveRamp2 = new RRamp();
    playMoveRamp2->renderRampRails = false;
    playMoveRamp2->render = false;
}

void RMarbleSlot::showStartWell(float delay, float duration) {
    playMessageSlot->render = true;
    playMessageSlot->setPosition(this->position.getCurrentPosition());
    playMessageSlot->position.animateToAfterDelay(ofPoint(this->position.getCurrentPosition().x, this->position.getCurrentPosition().y, this->position.getCurrentPosition().z - 30), delay);
    playMessageSlot->position.setDuration(duration);
    playMessageSlot->position.setRepeatType(PLAY_ONCE);
    playMessageSlot->size.setPosition(ofPoint(60,60));
}

void RMarbleSlot::expandPlaySlot(float delay, float duration) {
    playMessageSlot->render = true;
    playMessageSlot->setPosition(ofPoint(this->position.getCurrentPosition().x, this->position.getCurrentPosition().y, this->position.getCurrentPosition().z - 20));
    playMessageSlot->size.setPosition(ofPoint(60,60));
    playMessageSlot->size.animateToAfterDelay(this->size.getCurrentPosition(), delay);
    playMessageSlot->size.setDuration(duration);
    playMessageSlot->size.setRepeatType(PLAY_ONCE);
}

void RMarbleSlot::retractPlaySlot(float delay, float duration) {
    playMessageSlot->render = true;
    playMessageSlot->setPosition(ofPoint(this->position.getCurrentPosition().x, this->position.getCurrentPosition().y, this->position.getCurrentPosition().z - 20));
    playMessageSlot->size.setPosition(this->size.getCurrentPosition());
    playMessageSlot->size.animateToAfterDelay(ofPoint(60,60), delay);
    playMessageSlot->size.setDuration(duration);
    playMessageSlot->size.setRepeatType(PLAY_ONCE);
}

void RMarbleSlot::startPlayBack(float delay, float duration, float startPercentPosition) {
    int highOffset = 20;
    int lowOffset = 60;
    
    float remainingPercent = 1.0 - startPercentPosition;
    float animationLength = this->size.getCurrentPosition().x - 60;
    
    ofPoint startPosition = this->position.getCurrentPosition() + ofPoint(30 + (animationLength * startPercentPosition), 0, 0);
    
    ofPoint startPositionR1low = startPosition;
    startPositionR1low.y += 30;
    startPositionR1low.z -= lowOffset;
    ofPoint goalPositionR1low = startPositionR1low;
    goalPositionR1low.x += (animationLength * remainingPercent);
    
    ofPoint startPositionR2low = startPositionR1low;
    ofPoint goalPositionR2low = goalPositionR1low;
    
    ofPoint startPositionR1high = startPosition;
    startPositionR1high.x -= 90;
    startPositionR1high.y += 30;
    startPositionR1high.z += highOffset;
    ofPoint goalPositionR1high = startPositionR1high;
    goalPositionR1high.x += (animationLength * remainingPercent);
    
    
    ofPoint startPositionR2high = startPosition;
    startPositionR2high.x += 90;
    startPositionR2high.y += 30;
    startPositionR2high.z += highOffset;
    ofPoint goalPositionR2high = startPositionR2high;
    goalPositionR2high.x += (animationLength * remainingPercent);
    
    
    duration = duration * remainingPercent;
    
    // place the ramps
    playMoveRamp1->lowPoint.setPosition(startPositionR1low);
    playMoveRamp1->lowPoint.animateToAfterDelay(goalPositionR1low, delay);
    playMoveRamp1->lowPoint.setDuration(duration);
    playMoveRamp1->lowPoint.setCurve(LINEAR);
    playMoveRamp1->highPoint.setPosition(startPositionR1high);
    playMoveRamp1->highPoint.animateToAfterDelay(goalPositionR1high, delay);
    playMoveRamp1->highPoint.setDuration(duration);
    playMoveRamp1->highPoint.setCurve(LINEAR);
    playMoveRamp1->render = true;
    
    // place the ramps
    playMoveRamp2->lowPoint.setPosition(startPositionR2low);
    playMoveRamp2->lowPoint.animateToAfterDelay(goalPositionR2low, delay);
    playMoveRamp2->lowPoint.setDuration(duration);
    playMoveRamp2->lowPoint.setCurve(LINEAR);
    playMoveRamp2->highPoint.setPosition(startPositionR2high);
    playMoveRamp2->highPoint.animateToAfterDelay(goalPositionR2high, delay);
    playMoveRamp2->highPoint.setDuration(duration);
    playMoveRamp2->highPoint.setCurve(LINEAR);
    playMoveRamp2->render = true;
}



void RMarbleSlot::drawHeightMap() {
    if (render) {
        playMessageSlot->drawHeightMap();
        playMoveRamp1->drawHeightMap();
        playMoveRamp2->drawHeightMap();
    }
}

void RMarbleSlot::drawGraphics() {
    if (render) {
        playMessageSlot->drawHeightMap();
        playMoveRamp1->drawHeightMap();
        playMoveRamp2->drawHeightMap();
    }
}

void RMarbleSlot::update(float dt) {
    playMessageSlot->update(dt);
    playMoveRamp1->update(dt);
    playMoveRamp2->update(dt);
    
    //printf("%f",playMoveRamp1->position.getPercentDone());
    
    if(playMoveRamp1->lowPoint.getPercentDone() == 1.0) {
        playMoveRamp1->render = false;
        playMoveRamp2->render = false;
    }
}