#pragma once

#include "ofMain.h"
#include "ReliefIOManager.h"
#include "RenderableObject.h"
#include "KinectTracker.h"
#include "HybridTokens.h"
#include "StaticManipulator.h"

class ReliefApplication : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
    void exit();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    //void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    void sendHeightToRelief();
    
    ReliefIOManager * mIOManager;
	unsigned char mPinHeightToRelief [RELIEF_SIZE_X][RELIEF_SIZE_Y];

    bool paused = false;
    bool drawPins = true;
    bool paintGraphics = true;

    ofFbo colorInputImage;                      // color from camera
    ofFbo depthInputImage;                      // depth from camera
    ofFbo detectedObjectsImage;                 // objects detected in input images

    // debugging images: write to these temporarily to display image data you're Q/A'ing.
    // don't push commits that write to them, however; leave them clean for others
    ofFbo debugImage1, debugImage2, debugImage3, debugImage4;

    ofFbo pinDisplayImage;                      // render graphics for projection onto pins here
    ofFbo pinHeightMapImageForPins;             // render height map for setting pin height here

    int projectorOffsetX;

    // pin height map generators
    vector< RenderableObject *> renderableObjects;
    RenderableObject * myCurrentRenderedObject;
    HybridTokens * myHybridTokens;
    StaticManipulator * myStaticManipulator;

    KinectTracker kinectTracker;
};
