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
#include "Constants.h"
#include "ColorBand.h"
#include "Cube.h"


#ifndef __Relief2__KinectTracker__
#define __Relief2__KinectTracker__

#include <iostream>


// construct a predicate for testing whether a blob has the specified id; useful with find_if
struct blobHasId {
    blobHasId(int id) : id(id) {}
    bool operator()(const Blob &blob) { return blob.id == id; }
private:
    int id;
};


class KinectTracker {
public:
    ofxKinect kinect;
    
    void setup();
    void exit();
    void drawColorImage(int x, int y, int width, int height);
    void drawDepthImage(int x, int y, int width, int height);
    void drawDetectedObjects(int x, int y, int width, int height);
    void drawDepthThresholdedColorImage(int x, int y, int width, int height);
    void drawCornerLikelihoods(int x, int y, int width, int height);
    void update();

    void findCubes(ColorBand cubeColor, ColorBand markerColor, ColorBand cubePlusHandColor, vector<Cube>& cubes);
    void findBlobs(ColorBand blobColor, float minArea, float maxArea, vector<Blob>& blobs, bool dilateHue=false);
    void findFingers(vector<ofPoint>& points);
    void findFingersAboveSurface(vector<ofPoint>& points);

    void saveDepthImage();
    void loadDepthBackground();

    vector<Cube> redCubes;                      // cube objects using red blobs
    vector<ofPoint> fingers;                    // fingers detected (z is relative above height map)
    vector<ofPoint> absFingers;                 // fingers detected (z is absolute)

    ofPoint src[4], dst[4];

    int frameWidth = 190;
    int frameHeight = 190;

    // unclear why, but the natural calculation for pinArea significantly overestimates pin sizes, so multiply by 0.8
    float pinArea = 0.8 * frameWidth * frameHeight / (RELIEF_SIZE_X * RELIEF_SIZE_Y);

    ofxCvColorImage colorImgRaw;                // color straight from kinect
    ofxCvColorImage colorImg;                   // color restricted to inFORM ROI
    ofxCvColorImage depthThresholdC;            // depth threshold as a color image
    ofxCvColorImage depthThresholdDilatedC;     // dilated depth threshold as a color image
    ofxCvColorImage dThresholdedColor;          // depth-thresholded color
    ofxCvColorImage dThresholdedColorDilated;   // dilated depth-thresholded color

    ofxCvGrayscaleImage depthImgRaw;            // depth straight from kinect
    ofxCvGrayscaleImage depthImg;               // depth restricted to inFORM ROI
    ofxCvGrayscaleImage depthNearThreshold;     // helper for removing depths that are too close
    ofxCvGrayscaleImage depthThreshold;         // threshold rejecting pixels of uninteresting depth
    ofxCvGrayscaleImage depthThresholdDilated;  // dilated depth threshold
    ofxCvGrayscaleImage dThresholdedColorDilatedG; // depth-thresholded color as a grayscale image
    ofxCvGrayscaleImage depthBG;                // used by finger tracking
    ofxCvGrayscaleImage depthBGPlusSurface;     // used by finger tracking
    ofxCvGrayscaleImage depthFiltered;          // used by finger tracking
    ofxCvGrayscaleImage cornerLikelihoods;      // map of each pixel's probability of being a corner

    ofxCvFloatImage depthThresholdF;            // depth threshold as a float image
    ofxCvFloatImage cornerLikelihoodsRawF;      // pre-normalization corner likelihoods map

    ofImage depthDisplayImage;
    ofFbo detectedObjectsDisplayFbo;
    ofImage cornerLikelihoodsDisplayImage;

    vector<ofPoint> corners;

    // blob tracking images
    ofxCvColorImage hsvImage;                   // input image converted to hsv
    ofxCvGrayscaleImage hue;                    // hue component
    ofxCvGrayscaleImage sat;                    // saturation component
    ofxCvGrayscaleImage bri;                    // brilliance component
    ofxCvGrayscaleImage colorThreshold;         // combined hue and saturation threshold

    // tracking objects
    ContourFinder finger_contourFinder;
    ContourFinder ball_contourFinder;
    BlobTracker finger_tracker;
    BlobTracker ball_tracker;
    Calibration calib;
    
    ofTrueTypeFont verdana;
    
private:
    void updateInputImages();
    void updateDepthThresholds();
    void generateBlobDescriptors(vector<Cube> cubes);
    void detectCorners(ofxCvGrayscaleImage &imageIn, vector<ofPoint>& cornersOut);

    // cube detection colors
    ColorBand redColor;
    ColorBand yellowColor;
    ColorBand excludePaintedPinsColor;

    int nextCubeId = 0; // assign cube tracking ids from this value

};

#endif /* defined(__Relief2__KinectTracker__) */
