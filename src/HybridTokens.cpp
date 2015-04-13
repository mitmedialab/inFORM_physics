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
    SwordsSchema outputType = SUM;
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
    drawSwordsHeightMap(RELIEF_PROJECTOR_SIZE_X);

    // life cubes slightly off the surface for a smooth dragging experience
    setAllCubeHeights(40, RELIEF_PROJECTOR_SIZE_X);

    pinHeightMapImage.end();
}

// lift cubes slightly above neighboring pins to facilitate smooth sliding
void HybridTokens::setCubeHeight(Cube *cube, int height, float lengthScale, float edgeLengthMultiplier) {
    ofSetColor(height);

    // draw cube footprint
    glPushMatrix();
    glTranslatef(cube->center.x * lengthScale, cube->center.y * lengthScale, 0.0f);
    glRotatef(-cube->theta, 0.0f, 0.0f, 1.0f);
    float scaledEdgeLength = cubeEdgeLength * lengthScale * edgeLengthMultiplier;
    ofRect(-scaledEdgeLength / 2, -scaledEdgeLength / 2, scaledEdgeLength, scaledEdgeLength);
    glPopMatrix();
}

// lift cubes slightly above neighboring pins to facilitate smooth sliding
void HybridTokens::setAllCubeHeights(int height, float lengthScale, float edgeLengthMultiplier) {
    for (vector<Cube>::iterator cube = kinectTracker->redCubes.begin(); cube < kinectTracker->redCubes.end(); cube++) {
        setCubeHeight(&(*cube), height, lengthScale, edgeLengthMultiplier);
    }
}

// lift cubes slightly above neighboring pins to facilitate smooth sliding
void HybridTokens::drawCubeRisers(float lengthScale) {
    ofSetColor(40);
    for (vector<Cube>::iterator cube = kinectTracker->redCubes.begin(); cube < kinectTracker->redCubes.end(); cube++) {
        int left = (cube->center.x - pinSize) * lengthScale;
        int top = (cube->center.y - pinSize) * lengthScale;
        ofRect(left, top, pinSize * 2 * lengthScale, pinSize * 2 * lengthScale);
    }
}

// height value default is 140, the height of our cubes
void HybridTokens::drawSword(float lengthScale, int height) {
    // sword attributes
    ofSetColor(height);
    int left, right, top, bottom;
    left = -0.07 * lengthScale;
    right = 0.07 * lengthScale;
    top = (-0.07 - 3 * cubeEdgeLength) * lengthScale;
    bottom = (-0.07 - 0.3 * cubeEdgeLength) * lengthScale;

    // draw sword
    ofRect(left, top, right - left, bottom - top);
}

void HybridTokens::drawSwordsHeightMap(float lengthScale) {
    if (!kinectTracker->redCubes.size()) {
        return;
    }

    // buffer repository for drawing a single sword into; associated pixels object for manipulating the result
    ofFbo swordBuffer;
    ofPixels swordPixels;
    swordBuffer.allocate(RELIEF_PROJECTOR_SIZE_X, RELIEF_PROJECTOR_SIZE_X, GL_RGBA);

    // pixels objects used to calculate the swords' intersection and union
    ofPixels swordsIntersection, swordsUnion;
    swordsIntersection.allocate(RELIEF_PROJECTOR_SIZE_X, RELIEF_PROJECTOR_SIZE_X, 1);
    swordsUnion.allocate(RELIEF_PROJECTOR_SIZE_X, RELIEF_PROJECTOR_SIZE_X, 1);
    swordsIntersection.set(255);
    swordsUnion.set(0);

    // draw each cube's sword and update the swords' intersection and union
    for (int i = 0; i < kinectTracker->redCubes.size(); i++) {
        Cube *cube = &kinectTracker->redCubes[i];

        // open sword buffer and transition to the cube's reference frame
        swordBuffer.begin();
        ofBackground(0);
        glPushMatrix();
        glTranslatef(cube->center.x * lengthScale, cube->center.y * lengthScale, 0.0f);
        glRotatef(-cube->theta, 0.0f, 0.0f, 1.0f);

        // draw sword
        drawSword(lengthScale);

        // reset coordinate system and close buffer target
        glPopMatrix();
        swordBuffer.end();

        // extract sword data as grayscale pixels
        swordBuffer.readToPixels(swordPixels);
        swordPixels.setNumChannels(1);

        for (int j = 0; j < swordPixels.size(); j++) {
            // update intersection pixel
            if (swordsIntersection[j]) {
                swordsIntersection[j] = swordPixels[j];
            }
            // update union pixel
            if (swordPixels[j]) {
                swordsUnion[j] = swordPixels[j];
            }
        }
    }

    // calculate appropriate output drawing given the active schema
    ofPixels swordsOutput;

    // union of swords
    if (swordsSchema == UNION) {
        swordsOutput = swordsUnion;

    // intersection of swords
    } else if (swordsSchema == INTERSECTION) {
        swordsOutput = swordsIntersection;

    // sum of swords (add intersection on top of union)
    } else if (swordsSchema == SUM) {
        swordsOutput = swordsUnion;
        if (kinectTracker->redCubes.size() > 1) {
            for (int j = 0; j < swordsOutput.size(); j++) {
                if (swordsIntersection[j]) {
                    swordsOutput[j] = 255; //swordsOutput[j] += swordsIntersection[j];
                }
            }
        }

    // xor of swords (subtract intersection from union)
    } else if (swordsSchema == XOR) {
        swordsOutput = swordsUnion;
        if (kinectTracker->redCubes.size() > 1) {
            for (int j = 0; j < swordsOutput.size(); j++) {
                if (swordsIntersection[j]) {
                    swordsOutput[j] = 0;
                }
            }
        }
    }

    // draw the swords
    ofSetColor(255);
    ofImage(swordsOutput).draw(0,0);

    // but don't draw anything under the cubes
    setAllCubeHeights(0, lengthScale, 1.5);
}

void HybridTokens::keyPressed(int key) {
    if(key == 'a') {
        swordsSchema = UNION;
    }

    if(key == 's') {
        swordsSchema = INTERSECTION;
    }

    if(key == 'd') {
        swordsSchema = SUM;
    }

    if(key == 'f') {
        swordsSchema = XOR;
    }
}
