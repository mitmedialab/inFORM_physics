//
//  ColorBand.h
//  Relief2
//
//  Created by Daniel Windham on 4/24/15.
//
//

#include "Constants.h"
#include "ofxOpenCv.h"


#ifndef __Relief2__ColorBand__
#define __Relief2__ColorBand__


class ColorBand {
public:
    // each hsv input parameter is automatically adjusted to fit its allowed range. wrap-around is supported.
    ColorBand(int _minHue, int _maxHue, int _minSat=0, int _maxSat=255, int _minBri=0, int _maxBri=255);
    void hsvThreshold(ofxCvGrayscaleImage &hue, ofxCvGrayscaleImage &sat, ofxCvGrayscaleImage &bri, ofxCvGrayscaleImage &dst);

    int minHue, maxHue, minSat, maxSat, minBri, maxBri;
    const int allowedHueRange[2] = {0, 181}; // [0, 181)
    const int allowedSatRange[2] = {0, 256}; // [0, 256)
    const int allowedBriRange[2] = {0, 256}; // [0, 256)

private:
    int useMinHue, useMaxHue, useMinSat, useMaxSat, useMinBri, useMaxBri;
    ofxCvGrayscaleImage hueThreshHigh;          // hue high boundary threshold
    ofxCvGrayscaleImage hueThreshLow;           // hue low boundary threshold
    ofxCvGrayscaleImage satThreshHigh;          // sat high boundary threshold
    ofxCvGrayscaleImage satThreshLow;           // sat low boundary threshold
    ofxCvGrayscaleImage briThreshHigh;          // bri high boundary threshold
    ofxCvGrayscaleImage briThreshLow;           // bri low boundary threshold

};


#endif /* defined(__Relief2__ColorBand__) */