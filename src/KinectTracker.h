//
//  KinectTracker.h
//  Relief2
//
//  Created by Sean Follmer on 3/24/13.
//
//

#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ofxKCore.h"


#ifndef __Relief2__KinectTracker__
#define __Relief2__KinectTracker__

#include <iostream>

class KinectTracker {
public:
    ofxKinect kinect;
    
    void setup();
    void exit();
    void draw(int x, int y, int width, int height, int probe_x=NULL, int probe_y=NULL);
    void drawColorImage(int x, int y, int width, int height);
    void drawDepthImage(int x, int y, int width, int height);
    void drawDetectedObjects(int x, int y, int width, int height);
    void update();
    
    void findBlobs(int hue_target, int hue_tolerance, int sat_limit, vector<Blob>& blobs);
    void findFingers(vector<ofPoint>& points);
    void findFingersAboveSurface(vector<ofPoint>& points);
    
    void setPinHeightMap(ofPixels & tempPixels);
    
    void saveDepthImage();
    void loadDepthBackground();

    void drawCount(int width, int height);
    
    vector<Blob>  redBlobs;
    vector<Blob>  blueBlobs;
    vector<ofPoint>  fingers; //z is relative above height map
    vector<ofPoint>  absFingers; //z is absolute (not vodka)
    
    ofPixels pinHeightMap;
    int pinHeightMapWidth = 30;
    int pinHeightMapHeight = 30;
    
    ofPoint src[4],dst[4];

    int frameWidth = 190;
    int frameHeight = 190;

    ofxCvColorImage colorImgRaw, colorImg, depthThreshold, thresholdedColorImg, scaledColorImg;
    ofxCvGrayscaleImage depthImgRaw, depthImg, depthImgBG, depthImgBGPlusSurface, depthImgFiltered, pinHeightMapImage;
    ofImage depthDisplayImage;
    ofImage colorDisplayImage;
    ofImage detectedObjectsDisplayImage;

    int frame = 0;
    int size;
    ostringstream pointLocationsText;

    int cubeMinX, cubeMaxX, cubeMinY, cubeMaxY, cubeCenterX, cubeCenterY;
    ofPoint cubeLeftCorner, cubeRightCorner, cubeTopCorner, cubeBottomCorner;
    
	ofxCvGrayscaleImage grayThreshNear, grayThreshFar;

    ofxCvColorImage hsvImage;
    ofxCvGrayscaleImage hueThreshNear, hueThreshFar, hueThresh, satThresh; // the near thresholded image
    ofxCvGrayscaleImage hue,sat,bri,filtered;

    ofFbo kinectView;
    
    
    ContourFinder finger_contourFinder;
    ContourFinder ball_contourFinder;
    BlobTracker finger_tracker;
    BlobTracker ball_tracker;
    Calibration calib;
    
    ofTrueTypeFont verdana;


};

#endif /* defined(__Relief2__KinectTracker__) */
