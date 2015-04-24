//
//  ColorBand.cpp
//  Relief2
//
//  Created by Daniel Windham on 4/24/15.
//
//

#include "ColorBand.h"

ColorBand::ColorBand(int _hueTarget, int _hueTolerance, int _saturationThreshold) {
    hueTarget = min(_hueTarget, 255);
    hueTolerance = min(_hueTolerance, 128);
    saturationThreshold = min(_saturationThreshold, 255);
};


void ColorBand::hsThreshold(ofxCvGrayscaleImage &hue, ofxCvGrayscaleImage &sat, ofxCvGrayscaleImage &dst) {
    if (hue.width != sat.width || hue.height != sat.height) {
        cout << "Error: hsThreshold input dimensions do not match" << endl;
        return;
    }

    int width = hue.width;
    int height = hue.height;

    // allocate or resize images if they are not already set up from a previous use
    hueThreshHigh.allocate(width, height);
    hueThreshLow.allocate(width, height);
    satThresh.allocate(width, height);
    hueThresh.allocate(width, height);

    // thresholds on hue, allowing for zero-boundary wrap-around
    hueThreshHigh = hue;
    hueThreshLow = hue;
    hueThreshHigh.threshold((hueTarget + hueTolerance) % 256, CV_THRESH_BINARY_INV);
    hueThreshLow.threshold((hueTarget - hueTolerance + 256) % 256 - 1, CV_THRESH_BINARY); // the "- 1" makes the low boundary inclusive

    // if thresholds wrap around zero, take their union instead of their intersection
    bool wrapAround = (hueTarget - hueTolerance < 0) || (hueTarget + hueTolerance > 255);
    if (wrapAround) {
        cvOr(hueThreshHigh.getCvImage(), hueThreshLow.getCvImage(), hueThresh.getCvImage(), NULL);
    } else {
        cvAnd(hueThreshHigh.getCvImage(), hueThreshLow.getCvImage(), hueThresh.getCvImage(), NULL);
    }

    // threshold saturation
    satThresh = sat;
    satThresh.threshold(saturationThreshold);

    cvAnd(hueThresh.getCvImage(), satThresh.getCvImage(), dst.getCvImage(), NULL);
}