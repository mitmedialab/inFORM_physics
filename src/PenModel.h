//
//  PenModel.h
//  Relief2
//
//  Created by Sean Follmer on 3/31/13.
//
//


#include "ofMain.h"
#include "ofxSimpleGuiToo.h"
#include "ReliefIOManager.h"
#include "RenderableObject.h"
#include "RWell.h"
#include "RRectangle.h"
#include "AnsweringMachine.h"
#include "MarbleMachine.h"
#include "RBitmap.h"
#include "KinectTracker.h"
#include "ofx3DModelLoader.h"
#include "R3DModel.h"
#include <memory>

enum penModelState {geometryMode, flattenMode, paintMode, menuMode, noneMode};


class PenModel: public ofBaseApp {
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
    
    void determineCurrentState(vector<ofPoint> ballLocations);
    bool pointIsCloseToPoint(ofPoint p1, ofPoint p2, int minDist);
    bool pointIsFarFromPoint(ofPoint p1, ofPoint p2, int maxDist);
    bool pointIsInsideTable(ofPoint p1);
    bool pointIsInsideRectangle(ofPoint p1, int x, int y, int width, int height);
    
    void sendHeightToRelief();
    float getAngle(float _x, float _y);
    
    
    ReliefIOManager * mIOManager;
    unsigned char mPinHeightToRelief [RELIEF_SIZE_X][RELIEF_SIZE_Y];
    ofPoint lastFinger;
    bool stillDown = false;
    
    ofFbo pinDisplayImage; //FBO where we render graphics for pins
    ofFbo pinHeightMapImage; //FBO where we render height map
    ofFbo pinHeightMapImageSmall; //FBO where we render height map
    
    
    int projectorOffsetX;
    
    vector< R3DModel *> models;
    int current_model = 0;
    
    KinectTracker kinectTracker;
    
    penModelState currentState= geometryMode;
    
    float penDrawing[RELIEF_SIZE_X][RELIEF_SIZE_Y];
    
    ofTrueTypeFont verdana;
    
    ofShader mHeightMapShader;
    ofx3DModelLoader squirrelModel;
        
    ofPoint contextMenuLocation;
    
    ofxAnimatableFloat galleryAnim;
    short menuSelection = -1;
    
    vector< ofPolyline > paintLines;
};