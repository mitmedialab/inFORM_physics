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
#include "RWell.h"
#include "RRectangle.h"
#include "RRamp.h"
#include "RBitmap.h"
#include "KinectTracker.h"
#include "ofFbo.h"


enum SwordsSchema {SUM, XOR, UNION, INTERSECTION};

class HybridTokens : public RenderableObject {
public:
    
    HybridTokens(KinectTracker *tracker);
    
    void drawHeightMap();
    void drawGraphics();
    void update(float dt);
    
    void keyPressed(int key);

    // swords schema specifier
    SwordsSchema swordsSchema;

private:
    KinectTracker *kinectTracker;
    ofFbo pinHeightMapImage; //FBO where we render height map

    void setCubeHeight(Cube *cube, int height, float lengthScale, float edgeLengthMultiplier = 1.0);
    void setAllCubeHeights(int height, float lengthScale, float edgeLengthMultiplier = 1.0);
    void drawSword(float lengthScale, int height=140);
    void drawBooleanSwords(float lengthScale);
    void drawFlexibleSwords(float lengthScale, int height=140);

    const float pinSize = 1.0 / RELIEF_SIZE_X;
    const float cubeEdgeLength = 4 * pinSize;

    float flexibleExtensionSize;

};

#endif /* defined(__Relief2__HybridTokens__) */