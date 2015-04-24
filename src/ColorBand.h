//
//  ColorBand.h
//  Relief2
//
//  Created by Daniel Windham on 4/24/15.
//
//

#include "Constants.h"
#include "ofxOpenCv.h"
//#include "ofxKCore.h"


#ifndef __Relief2__ColorBand__
#define __Relief2__ColorBand__


class ColorBand {
public:
    ColorBand(int _hueTarget, int _hueTolerance, int _saturationThreshold);

    int hueTarget;
    int hueTolerance;
    int saturationThreshold;

};


#endif /* defined(__Relief2__ColorBand__) */