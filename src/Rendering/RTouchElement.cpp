//
//  RTouchElement.cpp
//  Relief2
//
//  Created by Sean Follmer on 3/28/13.
//
//

#include "RTouchElement.h"


void RTouchElement::drawHeightMap() {
    
    if (render) {
        ofFill();
        ofSetColor(position.getCurrentPosition().z);
        ofRect(position.getCurrentPosition().x, position.getCurrentPosition().y, size.getCurrentPosition().x, size.getCurrentPosition().y);
        /*
        if (isTouched){
            //ofSetColor(position.getCurrentPosition().z+20);
            if(type==horizontal) {
                ofRect( position.getCurrentPosition().x + value.x*size.getCurrentPosition().x -23, position.getCurrentPosition().y, 46, size.getCurrentPosition().y);
            }
            if(type == vertical){
                
                ofRect( position.getCurrentPosition().x, position.getCurrentPosition().y + value.y*size.getCurrentPosition().y -23, size.getCurrentPosition().x, 46);
            }
            if(type==pad) {
                ofRect( position.getCurrentPosition().x + value.x*size.getCurrentPosition().x -23, position.getCurrentPosition().y + value.y*size.getCurrentPosition().y -23, 46, 46);
            }
        }*/
    }
}

void RTouchElement::drawGraphics() {
    if (render) {
        ofFill();
        
        ofSetColor(0,0,255);
        ofRect(position.getCurrentPosition().x, position.getCurrentPosition().y, size.getCurrentPosition().x, size.getCurrentPosition().y);
        if(isTouched){
            ofSetColor(0,255,0);
        } else {
            ofSetColor(255,255,255);
        }
        
        if(type==horizontal) {
            ofRect( position.getCurrentPosition().x + value.x*size.getCurrentPosition().x -23, position.getCurrentPosition().y, 46, size.getCurrentPosition().y);
        }
        if(type == vertical){
            
            ofRect( position.getCurrentPosition().x, position.getCurrentPosition().y + value.y*size.getCurrentPosition().y -23, size.getCurrentPosition().x, 46);
        }
        if(type==pad) {
            ofRect( position.getCurrentPosition().x + value.x*size.getCurrentPosition().x -23, position.getCurrentPosition().y + value.y*size.getCurrentPosition().y -23, 46, 46);
        }
    }
}

void RTouchElement::update(float dt) {
    position.update(dt);
    size.update(dt);
}

void RTouchElement::touchTest(vector<ofPoint> &touchPoints){
    isTouched = false;
    for (int i =0; i < touchPoints.size(); i++){
        //printf("x: %f, y: %f, z:%f", touchPoints[i].x, touchPoints[i].y, touchPoints[i].z);
        if(touchPoints[i].z < 2) { //touching the plane
            if(touchPoints[i].x>position.getCurrentPosition().x && touchPoints[i].x<(position.getCurrentPosition().x+ size.getCurrentPosition().x) && touchPoints[i].y>position.getCurrentPosition().y && touchPoints[i].y<(position.getCurrentPosition().y+ size.getCurrentPosition().y)){
                //touched
                isTouched = true;
                value.x = (touchPoints[i].x - position.getCurrentPosition().x)/(size.getCurrentPosition().x);
                value.y = (touchPoints[i].y - position.getCurrentPosition().y)/(size.getCurrentPosition().y);
                //printf("touched:  \n");
            }
        }
    }
}

ofPoint RTouchElement::getValue(){
    return value;
}
