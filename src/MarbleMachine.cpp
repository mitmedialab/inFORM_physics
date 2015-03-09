//
//  MarbleMachine.cpp
//  Relief2
//
//  Created by Daniel Leithinger on 3/23/13.
//
//

#include "MarbleMachine.h"

MarbleMachine::MarbleMachine() {
    
    
    mamBase = new RBitmap();
    mamBase->setPosition(ofPoint(0,0));
    mamBase->setSize(ofPoint(900,900));
    mamBase->setImages("black.png", "text_test.png", ofxAnimatableFloat());
    mamBase->render = true;
    
    myMamElements.push_back(mamBase);
    
}

void MarbleMachine::drawHeightMap() {
    for(int i=0; i < myMamElements.size(); i++){
        myMamElements[i]->drawHeightMap();
    }

}

void MarbleMachine::drawGraphics() {
    for(int i=0; i < myMamElements.size(); i++){
        myMamElements[i]->drawGraphics();
    }
}

void MarbleMachine::update(float dt) {
    for(int i=0; i < myMamElements.size(); i++){
        myMamElements[i]->update(dt);
    }
    
}

void MarbleMachine::moveToNextState() {

}

void MarbleMachine::keyPressed(int key) {
    if (key == 'q') {
        ofxAnimatableFloat blendFloat;
        blendFloat.animateFromTo( 0.0f, 1.0f );
        blendFloat.setDuration(5);
        blendFloat.setRepeatType(PLAY_ONCE);
        mamBase->imageBlendAnim = blendFloat;
    }
    
    
    if (key == 'w') {
        ofxAnimatableFloat blendFloat;
        blendFloat.animateFromTo( 1.0f, 0.0f );
        blendFloat.setDuration(1);
        blendFloat.setRepeatType(PLAY_ONCE);
        mamBase->imageBlendAnim = blendFloat;
    }
}

