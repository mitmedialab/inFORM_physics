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
    swordsSchema = SUM;

    // flexible swords extension parameter
    flexibleExtensionSize = 0.8;
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
    drawBooleanSwords(RELIEF_PROJECTOR_SIZE_X);

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

void HybridTokens::drawBooleanSwords(float lengthScale) {
    if (!kinectTracker->redCubes.size()) {
        return;
    }

    // buffer repository for drawing a single sword into; associated pixels object for manipulating the result
    ofFbo swordBuffer;
    ofPixels swordPixels;
    swordBuffer.allocate(lengthScale, lengthScale, GL_RGBA);

    // pixels objects used to calculate the swords' intersection and union
    ofPixels swordsIntersection, swordsUnion;
    swordsIntersection.allocate(lengthScale, lengthScale, 1);
    swordsUnion.allocate(lengthScale, lengthScale, 1);
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
    swordsOutput.allocate(lengthScale, lengthScale, 1);

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


void HybridTokens::drawFlexibleSwords(float lengthScale, int height) {
    if (!kinectTracker->redCubes.size() || kinectTracker->redCubes.size() > 2) {
        return;
    }

    // if there's just one cube, draw a standard sword
    if (kinectTracker->redCubes.size() == 1) {
        drawBooleanSwords(lengthScale);
        return;
    }

    // construct control points for the bezier curve from the cubes
    ofPoint controlPoints[4];
    controlPoints[0].set(kinectTracker->redCubes[0].center * lengthScale);
    controlPoints[3].set(kinectTracker->redCubes[1].center * lengthScale);
    float angle0 = kinectTracker->redCubes[0].thetaRadians;
    float angle1 = kinectTracker->redCubes[1].thetaRadians;
    int cubeSeparation = controlPoints[0].distance(controlPoints[3]);
    controlPoints[1].set(controlPoints[0] + ofPoint(-sin(angle0), -cos(angle0)) * cubeSeparation * flexibleExtensionSize);
    controlPoints[2].set(controlPoints[3] + ofPoint(-sin(angle1), -cos(angle1)) * cubeSeparation * flexibleExtensionSize);

    // calculate points along the 1D bezier curve and corresponding boundary vertices for the 2D shape
    const int sampleSize = 1000;
    ofPoint interpolationPoints[sampleSize];
    ofPoint boundaryVertices[2 * sampleSize];
    ofPoint *previousPoint = &controlPoints[0];
    for (int i = 0; i < sampleSize; i++) {
        // calculate the cubic bezier interpolation point
        float t = 1.0 * (i + 1) / sampleSize;
        float c0 = (1 - t) * (1 - t) * (1 - t);
        float c1 = (1 - t) * (1 - t) * t * 3;
        float c2 = (1 - t) * t * t * 3;
        float c3 = t * t * t;
        interpolationPoints[i] = c0 * controlPoints[0] + c1 * controlPoints[1] + c2 * controlPoints[2] + c3 * controlPoints[3];

        // find the normalized vectors tangent to the interpolation path at this point
        ofPoint offsetDirection((interpolationPoints[i] - *previousPoint).getNormalized());
        ofPoint leftDirection(offsetDirection.y, -offsetDirection.x);
        ofPoint rightDirection(-offsetDirection.y, offsetDirection.x);

        // add vertex points at the boundaries of a cube tracing the interpolation points
        boundaryVertices[i].set(interpolationPoints[i] + leftDirection * cubeEdgeLength * lengthScale / 2);
        boundaryVertices[2 * sampleSize - 1 - i].set(interpolationPoints[i] + rightDirection * cubeEdgeLength * lengthScale / 2);

        // update the previous point pointer
        previousPoint = &interpolationPoints[i];
    }

    // draw the 2D curve
    ofSetColor(height);
    ofSetLineWidth(0);
    ofFill();
    ofBeginShape();
    ofVertices(vector<ofPoint>(boundaryVertices, boundaryVertices + 2 * sampleSize));
    ofEndShape();

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

    if(key == '-') {
        flexibleExtensionSize -= 0.1;
    }

    if(key == '=' || key == '+') {
        flexibleExtensionSize += 0.1;
    }
}
