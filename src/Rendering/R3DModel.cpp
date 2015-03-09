//
//  R3DModel.cpp
//  Relief2
//
//  Created by Sean Follmer on 3/31/13.
//
//

#include "R3DModel.h"

void R3DModel::build_stuff() {
    loadShader();
    
    currentManipulationState = idleMode;
    
    //squirrel->model_scale(0.03f);
    //squirrel->model_rotate(0,90,0,0,1);
    
    ofxAnimatableFloat transition;
    transition.animateFromTo(0,0);
    transition.setDuration(1.0f);
    
    ofPoint position = ofPoint(0,0,0);
    ofPoint size = ofPoint(900, 900);
    
    menuSlot = new RBitmap();
    menuSlot->setPosition(position);
    menuSlot->setSize(size);
    menuSlot->render = true;
    menuSlot->setImages("3DModel-Handles-home.png", "3DModel-Handles-select.png", ofxAnimatableFloat());
    modelHandles.push_back(menuSlot);
    
    translateSlot = new RBitmap();
    translateSlot->setPosition(position);
    translateSlot->setSize(size);
    translateSlot->render = false;
    translateSlot->setImages("3DModel-Handles-select.png", "3DModel-Handles-home.png", ofxAnimatableFloat());
    modelHandles.push_back(translateSlot);
    
    rotateSlot = new RBitmap();
    rotateSlot->setPosition(position);
    rotateSlot->setSize(size);
    rotateSlot->render = false;
    rotateSlot->setImages("3DModel-Handles-select.png", "3DModel-Handles-ring.png", ofxAnimatableFloat());
    modelHandles.push_back(rotateSlot);
    
    scaleSlot = new RBitmap();
    scaleSlot->setPosition(position);
    scaleSlot->setSize(size);
    scaleSlot->render = false;
    scaleSlot->setImages("3DModel-Handles-select.png", "3DModel-Handles-scale.png", ofxAnimatableFloat());
    modelHandles.push_back(scaleSlot);
    
    degrees =0;
    scaleFactor =15;
}

R3DModel::R3DModel(primative prim){
    this->prim = prim;
    build_stuff();
}

R3DModel::R3DModel(char * filename){
    prim = PRIM_MODEL;
    model.loadModel(filename);
    model.setPosition(0,0,0);
    build_stuff();

}

void R3DModel::drawHeightMap() {
    
    if (render) {
        
        for(int i=0; i < modelHandles.size(); i++){
            modelHandles[i]->drawHeightMap();
        }
        
        glEnable(GL_DEPTH_TEST);
        heightMapShader.begin();

        ofPushMatrix();
        //glTranslatef(0,0,-256);
        ofTranslate(450 + translation.x, 450 + translation.y, translation.z);
        ofRotate(degrees, 0, 0, 1);
        ofRotate(90, 1, 0, 0);
        ofScale(scaleFactor,scaleFactor,scaleFactor);

        switch(prim) {
            case PRIM_MODEL:
                model.draw();
                break;
            case PRIM_CHEESE:
                glBegin(GL_QUADS);
                ofSetColor(ofColor::red);
                glVertex3f(-12,10, -12);
                ofSetColor(ofColor::blue);
                glVertex3f(-12,10,  12);
                ofSetColor(ofColor::green);
                glVertex3f( 12,3,  12);
                ofSetColor(ofColor::violet);
                glVertex3f( 12,3, -12);
                glEnd();
                break;
            case PRIM_CONE:
                ofCone(0,0,0,10,20);
                break;
        }
        ofPopMatrix();

        heightMapShader.end();
        glDisable(GL_DEPTH_TEST);
    }
}

void R3DModel::drawGraphics() {
    if (render) {
        for(int i=0; i < modelHandles.size(); i++){
            modelHandles[i]->drawGraphics();
        }
        
        //draw squirel
        glEnable(GL_DEPTH_TEST);

        ofPushMatrix();
        ofTranslate(450 + translation.x, 450 + translation.y, translation.z);
        ofRotate(degrees, 0, 0, 1);
        ofRotate(90, 1, 0, 0);
        ofScale(scaleFactor,scaleFactor,scaleFactor);

        switch(prim) {
            case PRIM_MODEL:
                model.draw();
                break;
            case PRIM_CHEESE:
                glBegin(GL_QUADS);
                ofSetColor(ofColor::red);
                glVertex3f(-12,10, -12);
                ofSetColor(ofColor::blue);
                glVertex3f(-12,10,  12);
                ofSetColor(ofColor::green);
                glVertex3f( 12,3,  12);
                ofSetColor(ofColor::violet);
                glVertex3f( 12,3, -12);
                glEnd();
                break;
            case PRIM_CONE:
                ofCone(0,0,0,10,20);
                break;
        }
        ofPopMatrix();
        glDisable(GL_DEPTH_TEST);
    }
}

void R3DModel::update(float dt) {
    for(int i=0; i < modelHandles.size(); i++){
        modelHandles[i]->update(dt);
    }
    
    position.update(dt);
    size.update(dt);
}


void R3DModel::determineState(vector<ofPoint> ballLocations){
    /*
    switch(currentManipulationState) {
        case idleMode:
            for (int i =0; i < ballLocations.size(); i++ ){
                if(pointIsCloseToPoint(ballLocations[i], HOME_BUTTON, 60)) {
                    switchState(selectMode);
                }
            }
            break;
        case selectMode:
            for (int i =0; i < ballLocations.size(); i++ ){
                if(pointIsCloseToPoint(ballLocations[i], TRANSLATE_BUTTON, 60)) {
                    transStart = ballLocations[i] - translation;
                    switchState(translateMode);
                }
                else if(pointIsCloseToPoint(ballLocations[i], SCALE_BUTTON, 60)) {
                    switchState(scaleMode);
                }
                else if(pointIsCloseToPoint(ballLocations[i], ROTATION_BUTTON, 60)) {
                    switchState(rotateMode);
                }
            }
            break;
        case translateMode:
            for (int i =0; i < ballLocations.size(); i++ ){
                if(pointIsCloseToPoint(ballLocations[i], HOME_BUTTON, 60)) {
                    switchState(selectMode);
                } else {
                    translation = 0.5f * translation + 0.5f * (ballLocations[i] - transStart);
                }
            }
            break;
        case rotateMode:
            for (int i =0; i < ballLocations.size(); i++ ){
                if(pointIsCloseToPoint(ballLocations[i], HOME_BUTTON, 60)) {
                    switchState(selectMode);
                }
                else {
                    if(pointIsFarFromPoint(ballLocations[i], ofPoint(450,450,0), 300)){
                        float degRot = getAngle(ballLocations[i].x-450,ballLocations[i].y-450);
                        degrees=degRot-135;
                    }
                }
                
            }
            break;
        case scaleMode:
            for (int i =0; i < ballLocations.size(); i++ ){
                if(pointIsCloseToPoint(ballLocations[i], HOME_BUTTON, 60)) {
                    switchState(selectMode);
                }
                else{
                    cout<<ballLocations[i].x << " " << ballLocations[i].y<< endl;
                    if(pointIsonXline(ballLocations[i], 834, 30) && ballLocations[i].x>200){
                        
                        float posScale = ballLocations[i].x/23.0;
                        scaleFactor=posScale;
                    }
                }
            }
            break;            
    }*/
    
}

void R3DModel::switchState(manipulationState newState) {
    menuSlot->render = false;
    scaleSlot->render = false;
    rotateSlot->render = false;
    translateSlot->render = false;
    
    switch (newState) {
        case idleMode:
            rotateSlot->imageBlendAnim.animateFromTo(1, 0);
            menuSlot->render = true;
        case rotateMode:
            rotateSlot->imageBlendAnim.animateFromTo(0, 1);
            rotateSlot->render = true;
            break;
        case scaleMode:
            scaleSlot->imageBlendAnim.animateFromTo(0, 1);
            scaleSlot->render = true;
            break;
        case translateMode:
            translateSlot->imageBlendAnim.animateFromTo(0, 1);
            translateSlot->render = true;
            break;
        case selectMode:
            switch (currentManipulationState) {
                case idleMode:
                    menuSlot->imageBlendAnim.animateFromTo(0, 1);
                    menuSlot->render = true;
                    break;
                    
                case scaleMode:
                    scaleSlot->imageBlendAnim.animateFromTo(1, 0);
                    scaleSlot->render = true;
                    
                case rotateMode:
                    rotateSlot->imageBlendAnim.animateFromTo(1, 0);
                    rotateSlot->render = true;
                    break;
                    
                case translateMode:
                    translateSlot->imageBlendAnim.animateFromTo(1, 0);
                    translateSlot->render = true;
                    break;
                default:
                    break;
            }
            break;
    }
    currentManipulationState = newState;
}

/////--------------------------
bool R3DModel::pointIsCloseToPoint(ofPoint p1, ofPoint p2, int minDist){
    
    if (p1.distance(p2) < minDist) {
        return true;
    }
    return false;
}

//------------------------------
bool R3DModel::pointIsFarFromPoint(ofPoint p1, ofPoint p2, int maxDist){
    if (p1.distance(p2) > maxDist) {
        return true;
    }
    return false;
}

bool R3DModel::pointIsonYline(ofPoint p1, int xPos, int maxDist){
    if (abs( p1.x - xPos ) < maxDist) {
        return true;
    }
    return false;
}

bool R3DModel::pointIsonXline(ofPoint p1, int yPos, int maxDist){
    if (abs( p1.y - yPos ) < maxDist) {
        return true;
    }
    return false;
}

//-------------------------------
float R3DModel::getAngle(float _x, float _y) {
    float  GRAD_PI = 180.0f / 3.14159f;
    if (_x == 0.0) {
        if(_y < 0.0)
            return 270;
        else
            return 90;
    } else if (_y == 0) {
        if(_x < 0)
            return 180;
        else
            return 0;
    }
    
    if ( _y > 0.0)
        if (_x > 0.0)
            return atan(_y/_x) * GRAD_PI;
        else
            return 180.0 - (atan(_y/-_x) * GRAD_PI);
        else
            if (_x > 0.0)
                return 360.0 - (atan(-_y/_x) * GRAD_PI);
            else
                return 180.0 + (atan(-_y/-_x) * GRAD_PI);
}

void R3DModel::model_scale(float x, float y, float z) {model.setScale(x,y,z);}
void R3DModel::model_rotate(int which, float degrees, float x, float y, float z) {model.setRotation(which, degrees, x, y, z);}
void R3DModel::model_translate(float x, float y, float z) {translation = ofVec3f(x,y,z);}