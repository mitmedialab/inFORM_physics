//
//  R3DModel.h
//  Relief2
//
//  Created by Sean Follmer on 3/31/13.
//
//

#ifndef __Relief2__R3DModel__
#define __Relief2__R3DModel__

#include "RenderableObject.h"
#include "ofx3DModelLoader.h"
#include "RWell.h"
#include "RBitmap.h"
#include <iostream>

const ofPoint HOME_BUTTON(450,705,0);
const ofPoint TRANSLATE_BUTTON(760,705,0);
const ofPoint SCALE_BUTTON(449,821,0);
const ofPoint ROTATION_BUTTON(194,705,0);


enum manipulationState {translateMode, rotateMode, scaleMode, selectMode, idleMode};
enum primative {PRIM_MODEL, PRIM_CHEESE, PRIM_CONE};

class R3DModel : public RenderableObject {
public:
    R3DModel(char * filename);
    R3DModel(primative prim);
    void build_stuff();
    
    primative prim;
    
    void drawHeightMap();
    void drawGraphics();
    void update(float dt);
    
    void determineState(vector<ofPoint> ballLocations);
    
    void switchState(manipulationState newState);
    
    ofx3DModelLoader model;
    
    RBitmap * gallerySlot;
    RBitmap * menuSlot;
    RBitmap * scaleSlot;
    RBitmap * rotateSlot;
    RBitmap * translateSlot;
    
    manipulationState currentManipulationState = idleMode;
    
    bool pointIsCloseToPoint(ofPoint p1, ofPoint p2, int minDist);
    bool pointIsFarFromPoint(ofPoint p1, ofPoint p2, int maxDist);
    bool pointIsonYline(ofPoint p1, int xPos, int maxDist);
    bool pointIsonXline(ofPoint p1, int yPos, int maxDist);
    float getAngle(float _x, float _y);
    
    vector< RenderableObject *> modelHandles;
    
    float degrees;
    float scaleFactor;
    ofVec3f translation;
    ofVec3f transStart;
        
    void model_scale(float x, float y, float z);
    void model_rotate(int which, float degrees, float x, float y, float z);
    void model_translate(float x, float y, float z);
};


#endif /* defined(__Relief2__R3DModel__) */
