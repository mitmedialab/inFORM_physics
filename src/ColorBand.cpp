//
//  ColorBand.cpp
//  Relief2
//
//  Created by Daniel Windham on 4/24/15.
//
//

#include "ColorBand.h"

// each hsv input parameter is automatically adjusted to fit its allowed range. wrap-around is supported.
ColorBand::ColorBand(int _minHue, int _maxHue, int _minSat, int _maxSat, int _minBri, int _maxBri) {
    set(_minHue, _maxHue, _minSat, _maxSat, _minBri, _maxBri);
};

void ColorBand::set(int _minHue, int _maxHue, int _minSat, int _maxSat, int _minBri, int _maxBri) {
    minHue = _minHue % allowedHueRange[1];
    maxHue = _maxHue % allowedHueRange[1];
    minSat = _minSat % allowedSatRange[1];
    maxSat = _maxSat % allowedSatRange[1];
    minBri = _minBri % allowedBriRange[1];
    maxBri = _maxBri % allowedBriRange[1];

    minHue >= 0 || (minHue += allowedHueRange[1]);
    maxHue >= 0 || (maxHue += allowedHueRange[1]);
    minSat >= 0 || (minSat += allowedSatRange[1]);
    maxSat >= 0 || (maxSat += allowedSatRange[1]);
    minBri >= 0 || (minBri += allowedBriRange[1]);
    maxBri >= 0 || (maxBri += allowedBriRange[1]);

    useMinHue = (minHue != allowedHueRange[0]);
    useMaxHue = (maxHue != allowedHueRange[1]);
    useMinSat = (minSat != allowedSatRange[0]);
    useMaxSat = (maxSat != allowedSatRange[1]);
    useMinBri = (minBri != allowedBriRange[0]);
    useMaxBri = (maxBri != allowedBriRange[1]);
}

void ColorBand::hsvThreshold(ofxCvGrayscaleImage &hue, ofxCvGrayscaleImage &sat, ofxCvGrayscaleImage &bri, ofxCvGrayscaleImage &dst) {
    if (hue.width != sat.width || hue.height != sat.height) {
        cout << "Error: hsThreshold input dimensions do not match" << endl;
        return;
    }

    int width = hue.width;
    int height = hue.height;

    // allocate or resize destination image if it's not already set up; start it high
    dst.allocate(width, height);
    dst.set(255);

    // threshold hue
    if (useMinHue) {
        // low threshold for hue image; the `-1` makes the low boundary inclusive
        hueThreshLow.allocate(width, height);
        hueThreshLow = hue;
        hueThreshLow.threshold(minHue - 1, CV_THRESH_BINARY);

        // unless max hue threshold causes wraparound hue boundaries, immediately intersect this with threshold
        if (!(useMaxHue && maxHue < minHue)) {
            cvAnd(hueThreshLow.getCvImage(), dst.getCvImage(), dst.getCvImage(), NULL);
        }
    }
    if (useMaxHue) {
        // high threshold for hue image
        hueThreshHigh.allocate(width, height);
        hueThreshHigh = hue;
        hueThreshHigh.threshold(maxHue, CV_THRESH_BINARY_INV);

        // if thresholds wrap around zero, threshold the destination with their union
        if (useMinHue && maxHue < minHue) {
            cvOr(hueThreshHigh.getCvImage(), hueThreshLow.getCvImage(), hueThreshHigh.getCvImage(), NULL);
        }

        // apply threshold to destination
        cvAnd(hueThreshHigh.getCvImage(), dst.getCvImage(), dst.getCvImage(), NULL);
    }

    // threshold saturation
    if (useMinSat) {
        // low threshold for sat image; the `-1` makes the low boundary inclusive
        satThreshLow.allocate(width, height);
        satThreshLow = sat;
        satThreshLow.threshold(minSat - 1, CV_THRESH_BINARY);

        // unless max sat threshold causes wraparound sat boundaries, immediately intersect this with threshold
        if (!(useMaxSat && maxSat < minSat)) {
            cvAnd(satThreshLow.getCvImage(), dst.getCvImage(), dst.getCvImage(), NULL);
        }
    }
    if (useMaxSat) {
        // high threshold for sat image
        satThreshHigh.allocate(width, height);
        satThreshHigh = sat;
        satThreshHigh.threshold(maxSat, CV_THRESH_BINARY_INV);

        // if thresholds wrap around zero, threshold the destination with their union
        if (useMinSat && maxSat < minSat) {
            cvOr(satThreshHigh.getCvImage(), satThreshLow.getCvImage(), satThreshHigh.getCvImage(), NULL);
        }

        // apply threshold to destination
        cvAnd(satThreshHigh.getCvImage(), dst.getCvImage(), dst.getCvImage(), NULL);
    }

    // threshold brilliance
    if (useMinBri) {
        // low threshold for bri image; the `-1` makes the low boundary inclusive
        briThreshLow.allocate(width, height);
        briThreshLow = bri;
        briThreshLow.threshold(minBri - 1, CV_THRESH_BINARY);

        // unless max bri threshold causes wraparound bri boundaries, immediately intersect this with threshold
        if (!(useMaxBri && maxBri < minBri)) {
            cvAnd(briThreshLow.getCvImage(), dst.getCvImage(), dst.getCvImage(), NULL);
        }
    }
    if (useMaxBri) {
        // high threshold for bri image
        briThreshHigh.allocate(width, height);
        briThreshHigh = bri;
        briThreshHigh.threshold(maxBri, CV_THRESH_BINARY_INV);

        // if thresholds wrap around zero, threshold the destination with their union
        if (useMinBri && maxBri < minBri) {
            cvOr(briThreshHigh.getCvImage(), briThreshLow.getCvImage(), briThreshHigh.getCvImage(), NULL);
        }

        // apply threshold to destination
        cvAnd(briThreshHigh.getCvImage(), dst.getCvImage(), dst.getCvImage(), NULL);
    }
}