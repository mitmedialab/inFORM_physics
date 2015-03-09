//
//  RenderableObject.h
//  Relief2
//
//  Created by Sean Follmer on 3/15/13.
//
//

#ifndef __Relief2__RenderableObject__
#define __Relief2__RenderableObject__

#include "ofxAnimatableFloat.h"
#include "ofxAnimatableOfPoint.h"
#include "ofxAnimatableOfColor.h"
#include "Constants.h"

class RenderableObject {
public:
    ofShader heightMapShader;
    ofxAnimatableOfPoint position;
    ofxAnimatableOfPoint size;
    ofColor color;
    bool useColor = false;
    bool render = false;
    bool isTouched = false;
    bool isHovered = false;
    bool containsObject = false;

    virtual void update(float dt) {};
    virtual void drawHeightMap(){};
    virtual void drawGraphics(){};
    
    virtual void touchTest(vector<ofPoint> & touchPoints){};
    virtual void objectTest(vector<ofPoint> & objectCentroids){};
    

    void loadShader() {heightMapShader.load("shaders/heightMapShader");}

    void setSize(ofPoint size) {this->size.setPosition(size);};
    void setPosition(ofPoint position) {this->position.setPosition(position);};
    void setColor(ofColor color) {this->color = color; useColor = true;};
    
    virtual void keyPressed(int key){};
};

#endif /* defined(__Relief2__RenderableObject__) */
