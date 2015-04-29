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
    pinHeightMapImage.allocate(RELIEF_SIZE_X, RELIEF_SIZE_Y, GL_RGBA);
    pinHeightMapContentPixels.allocate(RELIEF_SIZE_X, RELIEF_SIZE_Y, GL_RGBA);
    pinGraphicsPixels.allocate(RELIEF_SIZE_X, RELIEF_SIZE_Y, GL_RGBA);

    mode = BOOLEAN_SWORDS;

    // swords schema default
    booleanSwordsSchema = SUM;

    // flexible swords extension parameter
    flexibleExtensionSize = 0.8;
}

void HybridTokens::setMode(HybridTokensMode mode) {
    this->mode = mode;
}

void HybridTokens::drawHeightMap() {
    pinHeightMapImage.draw(0, 0);
}

void HybridTokens::drawGraphics() {
    int minX = RELIEF_PROJECTOR_SIZE_X * projectorMinXCoordinateBoundaryAtCubeHeight;
    int minY = RELIEF_PROJECTOR_SIZE_Y * projectorMinYCoordinateBoundaryAtCubeHeight;
    ofImage(reprojectedPinGraphicsPixels).draw(minX, minY);
}

void HybridTokens::update(float dt) {
    // draw height map content
    pinHeightMapImage.begin();
    ofBackground(0);
    ofSetColor(255);

    if (mode == BOOLEAN_SWORDS) {
        drawBooleanSwords(RELIEF_SIZE_X);

    } else if (mode == FLEXIBLE_SWORDS) {
        drawFlexibleSwords(RELIEF_SIZE_X);
    }
    pinHeightMapImage.end();

    // store height map content for graphics computations
    pinHeightMapImage.readToPixels(pinHeightMapContentPixels);

    // lift touched cubes slightly off the surface for a smooth dragging experience
    pinHeightMapImage.begin();
    setCubeHeights(40, RELIEF_SIZE_X, 1.0, TOUCHED);
    pinHeightMapImage.end();

    updateGraphics();
}

// paint pins according to their heights
void HybridTokens::updateGraphics() {
    // some bug in ofPixels causes it to break without this initialization; only necessary once
    if (!initializedInUpdate) {
        pinHeightMapImage.readToPixels(pinGraphicsPixels);
        initializedInUpdate = true;
    }

    // paint pins for 2D display
    int numChannels = pinGraphicsPixels.getNumChannels(); // this should be 4
    for (int i = 0; i < pinGraphicsPixels.size(); i += numChannels) {
        // since the height map rgba pixels are grayscale, the first channel is sufficient
        // to distinguish pin height
        if (pinHeightMapContentPixels[i] > 150) {
            pinGraphicsPixels[i + 0] = pinColorIfHigh[0];
            pinGraphicsPixels[i + 1] = pinColorIfHigh[1];
            pinGraphicsPixels[i + 2] = pinColorIfHigh[2];
        } else if (pinHeightMapContentPixels[i] > 0) {
            pinGraphicsPixels[i + 0] = pinColorIfOn[0];
            pinGraphicsPixels[i + 1] = pinColorIfOn[1];
            pinGraphicsPixels[i + 2] = pinColorIfOn[2];
        } else {
            pinGraphicsPixels[i + 0] = pinColorIfOff[0];
            pinGraphicsPixels[i + 1] = pinColorIfOff[1];
            pinGraphicsPixels[i + 2] = pinColorIfOff[2];
        }
    }

    // paint pins for projection onto 2.5D surface
    reprojectProjectorPixelsForHeightMap(pinGraphicsPixels, pinHeightMapContentPixels, reprojectedPinGraphicsPixels);
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
void HybridTokens::setCubeHeights(int height, float lengthScale, float edgeLengthMultiplier, TouchCondition touchCondition) {
    for (vector<Cube>::iterator cube = kinectTracker->redCubes.begin(); cube < kinectTracker->redCubes.end(); cube++) {
        if (touchCondition == UNDEFINED || (touchCondition == TOUCHED && cube->isTouched) ||
                (touchCondition == NOT_TOUCHED && !cube->isTouched)) {
            setCubeHeight(&(*cube), height, lengthScale, edgeLengthMultiplier);
        }
    }
}

// height value default is 140, the height of our cubes
void HybridTokens::drawSword(float lengthScale, int height) {
    // sword attributes: sword is the cube footprint extended three extra cube lengths up
    ofSetColor(height);
    int left, right, top, bottom;
    left = -0.5 * cubeEdgeLength * lengthScale;
    right = 0.5 * cubeEdgeLength * lengthScale;
    top = (-0.5 - 3) * cubeEdgeLength * lengthScale;
    bottom = 0.5 * cubeEdgeLength * lengthScale;

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
    if (booleanSwordsSchema == UNION) {
        swordsOutput = swordsUnion;

    // intersection of swords
    } else if (booleanSwordsSchema == INTERSECTION) {
        swordsOutput = swordsIntersection;

    // sum of swords (add intersection on top of union)
    } else if (booleanSwordsSchema == SUM) {
        swordsOutput = swordsUnion;
        if (kinectTracker->redCubes.size() > 1) {
            for (int j = 0; j < swordsOutput.size(); j++) {
                if (swordsIntersection[j]) {
                    swordsOutput[j] = 255; //swordsOutput[j] += swordsIntersection[j];
                }
            }
        }

    // xor of swords (subtract intersection from union)
    } else if (booleanSwordsSchema == XOR) {
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
    setCubeHeights(0, lengthScale, 1.5);
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
    setCubeHeights(0, lengthScale, 1.5);
}

void HybridTokens::keyPressed(int key) {
    if(key == 'a') {
        booleanSwordsSchema = UNION;
    }

    if(key == 's') {
        booleanSwordsSchema = INTERSECTION;
    }

    if(key == 'd') {
        booleanSwordsSchema = SUM;
    }

    if(key == 'f') {
        booleanSwordsSchema = XOR;
    }

    if(key == '-') {
        flexibleExtensionSize -= 0.1;
    }

    if(key == '=' || key == '+') {
        flexibleExtensionSize += 0.1;
    }
}
