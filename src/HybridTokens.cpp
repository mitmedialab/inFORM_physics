//
//  HybridTokens.cpp
//  Relief2
//
//  Created by Daniel Windham on 3/18/15.
//
//

#include "HybridTokens.h"

HybridTokens::HybridTokens(KinectTracker *tracker) {
    kinectTracker = tracker;
    pinHeightMapImage.allocate(RELIEF_PROJECTOR_SIZE_X, RELIEF_PROJECTOR_SIZE_X, GL_RGBA);

    // swords schema default
    useStaticSecondSword = true;
    intersectSwords = false;
    blockadeSword = true;
}

void HybridTokens::drawHeightMap() {
    pinHeightMapImage.draw(0, 0);
}

void HybridTokens::drawGraphics() {
}

void HybridTokens::update(float dt) {
    pinHeightMapImage.begin();
    ofBackground(0);
    ofSetColor(255);
    drawAngleSwordsHeightMap(RELIEF_PROJECTOR_SIZE_X);
    pinHeightMapImage.end();
}

void HybridTokens::drawAngleSwordsHeightMap(float lengthScale) {
    // known width and height of our cubes
    float width = 4.0 / 30;
    float height = 4.0 / 30;

    // sword attributes
    ofSetColor(140);
    int left, right, top, bottom;
    left = -0.07 * lengthScale;
    right = 0.07 * lengthScale;
    top = (-0.07 - 3 * height) * lengthScale;
    bottom = (-0.07 - 0.3 * height) * lengthScale;
    
    for (vector<Cube>::iterator cube = kinectTracker->redCubes.begin(); cube < kinectTracker->redCubes.end(); cube++) {
        // draw sword appropriately rotated
        glPushMatrix();
        glTranslatef(cube->center.x * lengthScale, cube->center.y * lengthScale, 0.0f);
        glRotatef(-cube->theta, 0.0f, 0.0f, 1.0f);
        ofRect(left, top, right - left, bottom - top);
        glPopMatrix();
    }
}

void HybridTokens::drawSwordsHeightMap(float lengthScale) {
    // for now, assume a ready cube is flat and aligned to the coordinate axes
    if (kinectTracker->redCubes.size() == 1) {
        // known width and height of our cubes
        float width = 4.0 / 30;
        float height = 4.0 / 30;
        
        // dynamic cube
        Cube cube = kinectTracker->redCubes[0];
        
        // for now, hardcode a description of the static block
        ofPoint fixedCenter(0.17, 0.34);
        
        // draw dynamic sword up
        ofSetColor(140);
        int left, right, top, bottom;
        left = (cube.center.x - 0.07) * lengthScale;
        right = (cube.center.x + 0.07) * lengthScale;
        top = (cube.center.y - 0.07 - 3 * height) * lengthScale;
        bottom = (cube.center.y - 0.07 - 0.3 * height) * lengthScale;
        ofRect(left, top, right - left, bottom - top);

        if (useStaticSecondSword) {
            // draw static sword right
            ofSetColor(140);
            int fixedLeft, fixedRight, fixedTop, fixedBottom;
            fixedLeft = (fixedCenter.x + 0.07 + 0.3 * height) * lengthScale;
            fixedRight = (fixedCenter.x + 0.07 + 3 * height) * lengthScale;
            fixedTop = (fixedCenter.y - 0.07) * lengthScale;
            fixedBottom = (fixedCenter.y + 0.07) * lengthScale;
            ofRect(fixedLeft, fixedTop, fixedRight - fixedLeft, fixedBottom - fixedTop);
            
            // draw sword intersections
            if (intersectSwords) {
                ofSetColor(255);
                if (fixedLeft < right && fixedRight > left && fixedTop < bottom && fixedBottom > top) {
                    int overlapLeft, overlapRight, overlapTop, overlapBottom;
                    overlapLeft = left < fixedLeft ? fixedLeft : left;
                    overlapRight = right < fixedRight ? right : fixedRight;
                    overlapTop = top < fixedTop ? fixedTop : top;
                    overlapBottom = bottom < fixedBottom ? bottom : fixedBottom;
                    ofRect(overlapLeft, overlapTop, overlapRight - overlapLeft, overlapBottom - overlapTop);
                }
            }
            
            // draw blockade
            if (blockadeSword) {
                ofSetColor(140);
                int closeDistance = 0.07 * lengthScale;
                if (left < fixedRight + closeDistance) {
                    int adjWidth = width * lengthScale;
                    int adjHeight = height * lengthScale;
                    int cubeBottom = (cube.center.y + 0.07) * lengthScale;
                    ofRect(fixedRight - adjWidth / 4, cubeBottom - adjHeight, adjWidth / 4, adjHeight);
                }
            }
        }
    }
}

void HybridTokens::keyPressed(int key) {
    if(key == 's') {
        useStaticSecondSword = !useStaticSecondSword;
    }

    if(key == 'i') {
        intersectSwords = !intersectSwords;
    }

    if(key == 'b') {
        blockadeSword = !blockadeSword;
    }
}
