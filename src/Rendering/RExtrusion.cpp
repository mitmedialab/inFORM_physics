//
//  RExtrusion.cpp
//  Relief2
//
//  Created by TMG on 11/9/13.
//
//

#include "RExtrusion.h"

RExtrusion::RExtrusion(ofColor color) {
    this->color = color;
    this->height = 0;
    this->points.clear();
    loadShader();
}

void RExtrusion::update(float dt) {
    return;
}

void RExtrusion::addPoint(ofVec3f point) {
    points.push_back(point);
}

void RExtrusion::updateHeight(float height) {
    this->height = 0.9f * this->height + 0.1f * height;
}

void RExtrusion::setColor(ofColor color) {
    this->color = color;
}

void RExtrusion::drawHeightMap() {
    heightMapShader.begin();
    glPushMatrix();
    glBegin(GL_POLYGON);
    
    for(std::vector<ofVec3f>::iterator itr = points.begin(); itr < points.end(); itr++) {
        glVertex3f(itr->x, itr->y, -256 + height);
    }
    
    glEnd();
    glPopMatrix();
    heightMapShader.end();
}

void RExtrusion::drawGraphics() {
    ofSetColor(this->color);
    glPushMatrix();
    glBegin(GL_POLYGON);
    
    for(std::vector<ofVec3f>::iterator itr = points.begin(); itr < points.end(); itr++) {
        glVertex2f(itr->x, itr->y);
    }
    
    glEnd();
    glPopMatrix();
    
}