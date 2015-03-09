//
//  GuiApplication.h
//  Relief2
//
//  Created by Daniel Leithinger on 3/30/13.
//
//

#ifndef __Relief2__GuiApplication__
#define __Relief2__GuiApplication__

#include "ofMain.h"
#include "ofxSimpleGuiToo.h"
#include "ReliefIOManager.h"
#include "RenderableObject.h"
#include "RWell.h"
#include "RRectangle.h"
#include "MarbleMachine.h"
#include "RBitmap.h"
#include "KinectTracker.h"
#include "ofx3DModelLoader.h"
#include "RButton.h"
#include "RTouchElement.h"
#include "RObjectHole.h"
#include "ofxObjLoader.h"
#include "RRipple.h"

#include <memory>

class GuiApplication : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    void exit();
    
    int probe_x;
    int probe_y;
    
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
    
    ofFbo pinDisplayImage; //FBO where we render graphics for pins
    ofFbo pinHeightMapImage; //FBO where we render height map
    ofFbo pinHeightMapImageSmall; //FBO where we render height map //TODO: I THink this can just be an image
    
    int projectorOffsetX;
    
    KinectTracker kinectTracker;
    
    vector< RenderableObject *> renderableObjects;
};

#endif /* defined(__Relief2__GuiApplication__) */