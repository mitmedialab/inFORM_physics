#include "RObjectHole.h"
#include "Constants.h"


void RObjectHole::drawHeightMap() {
    if (render) {
        ofFill();
        ofSetColor(position.getCurrentPosition().z);
        ofRect(position.getCurrentPosition().x, position.getCurrentPosition().y, size.getCurrentPosition().x, size.getCurrentPosition().y);
    }
}

void RObjectHole::drawGraphics() {
    if (render) {
        ofFill();
        if(containsObject) {
            ofSetColor(0, 0, 255);
        }
        else {
            ofSetColor(0, 255, 0);
        }
        ofRect(position.getCurrentPosition().x, position.getCurrentPosition().y, size.getCurrentPosition().x, size.getCurrentPosition().y);
        ofSetColor(255);
        ofRect(position.getCurrentPosition().x + 2, position.getCurrentPosition().y + 2, size.getCurrentPosition().x - 4, size.getCurrentPosition().y - 4);
        ofFill();
    }
}

void RObjectHole::update(float dt) {
    position.update(dt);
    size.update(dt);
}

void RObjectHole::objectTest(vector<ofPoint> & objectCentroids) {
    containsObject = false;
    for (int i =0; i < objectCentroids.size(); i++){
        //printf("x: %f, y: %f, z:%f", touchPoints[i].x, touchPoints[i].y, touchPoints[i].z);
        if(objectCentroids[i].z < 2) { //touching the surface
            if(objectCentroids[i].x>position.getCurrentPosition().x && objectCentroids[i].x<(position.getCurrentPosition().x+ size.getCurrentPosition().x) && objectCentroids[i].y>position.getCurrentPosition().y && objectCentroids[i].y<(position.getCurrentPosition().y+ size.getCurrentPosition().y)){
                //has object
                containsObject = true;
                //value.x = (objectCentroids[i].x - position.getCurrentPosition().x)/(size.getCurrentPosition().x);
                //value.y = (objectCentroids[i].y - position.getCurrentPosition().y)/(size.getCurrentPosition().y);
                //printf("touched:  \n");
            }
        }
    }
}