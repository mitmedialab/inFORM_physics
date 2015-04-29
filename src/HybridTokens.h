//
//  HybridTokens.h
//  Relief2
//
//  Created by Daniel Windham on 3/18/15.
//
//

#ifndef __Relief2__HybridTokens__
#define __Relief2__HybridTokens__

#include "RenderableObject.h"
#include "KinectTracker.h"
#include "CameraCalibration.h"
#include "ofFbo.h"


enum HybridTokensMode {BOOLEAN_SWORDS, FLEXIBLE_SWORDS};
enum BooleanSwordsSchema {SUM, XOR, UNION, INTERSECTION};
enum TouchCondition {TOUCHED, NOT_TOUCHED, UNDEFINED};

class HybridTokens : public RenderableObject {
public:
    
    HybridTokens(KinectTracker *tracker);
    
    void drawHeightMap();
    void drawGraphics();
    void update(float dt);

    void keyPressed(int key);

    void setMode(HybridTokensMode mode);

    // application mode
    HybridTokensMode mode;

    // boolean swords schema specifier
    BooleanSwordsSchema booleanSwordsSchema;

private:
    KinectTracker *kinectTracker;
    ofFbo pinHeightMapImage;                  // FBO where we render height map
    ofPixels pinHeightMapContentPixels;       // pixels object for computing on pin height map content
    ofPixels pinGraphicsPixels;               // pixels object where we render graphics
    ofPixels reprojectedPinGraphicsPixels;    // pixels object whose graphics account for the underlying pin heights

    void updateGraphics();
    void setCubeHeight(Cube &cube, int height, float lengthScale, float edgeLengthMultiplier = 1.0);
    void setCubeHeights(int height, float lengthScale, float edgeLengthMultiplier = 1.0, TouchCondition touchCondition=UNDEFINED);
    void drawSword(float lengthScale, int height=140);
    void drawSwordForCube(Cube &cube, float lengthScale, int height=140);
    void getSwordsIntersectionAndUnion(ofPixels &swordsIntersection, ofPixels &swordsUnion, float lengthScale);
    void drawBooleanSwords(float lengthScale);
    void drawFlexibleSwords(float lengthScale, int height=140);

    char pinColorIfHigh[3] = {255, 135, 0}; // yellow
    char pinColorIfOn[3] = {255, 12, 16}; // red
    char pinColorIfOff[3] = {0, 0, 0}; // black

    const float pinSize = 1.0 / RELIEF_SIZE_X;
    const float cubeEdgeLength = 4 * pinSize;

    float flexibleExtensionSize;

    bool initializedInUpdate = false; // hack to fix an ofPixels bug. see .cpp for details

};

#endif /* defined(__Relief2__HybridTokens__) */